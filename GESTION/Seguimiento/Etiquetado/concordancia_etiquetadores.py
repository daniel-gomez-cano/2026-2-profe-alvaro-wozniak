#!/usr/bin/env python3
"""Concordancia entre dos etiquetadores (Bananin, RF09 / RNF04).

Calcula, sobre las sesiones etiquetadas por las dos personas:
  - % de coincidencia exacta
  - % de sesiones con diferencia <= 1 etapa      (RNF04: meta >= 90 %)
  - kappa ponderado cuadratico (escala ordinal)  (RF09: se reporta, sin umbral)
  - matriz de confusion

Solo usa la biblioteca estandar de Python 3.8+.

Uso:
  python concordancia_etiquetadores.py --a etiquetas_A.csv --b etiquetas_B.csv [--salida informe.md]

Cada CSV debe tener al menos las columnas: id_sesion, etapa   (etapa = E1..E4 o 1..4).
"""
import argparse
import csv
import sys

ETAPAS = (1, 2, 3, 4)


def parse_etapa(valor, fila, archivo):
    v = str(valor).strip().upper().replace("E", "")
    try:
        n = int(v)
    except ValueError:
        raise SystemExit(f"{archivo}, fila {fila}: etapa '{valor}' no es E1..E4 ni 1..4")
    if n not in ETAPAS:
        raise SystemExit(f"{archivo}, fila {fila}: etapa {n} fuera de E1..E4")
    return n


def leer(ruta):
    with open(ruta, newline="", encoding="utf-8-sig") as f:
        lector = csv.DictReader(f)
        faltan = {"id_sesion", "etapa"} - set(lector.fieldnames or [])
        if faltan:
            raise SystemExit(f"{ruta}: faltan columnas {sorted(faltan)}")
        datos = {}
        for i, fila in enumerate(lector, start=2):
            sid = (fila["id_sesion"] or "").strip()
            if not sid:
                continue
            if sid in datos:
                raise SystemExit(f"{ruta}, fila {i}: id_sesion '{sid}' repetido (la etiqueta es por sesion, no por toma)")
            datos[sid] = parse_etapa(fila["etapa"], i, ruta)
    return datos


def matriz_confusion(a, b):
    m = [[0] * len(ETAPAS) for _ in ETAPAS]
    for x, y in zip(a, b):
        m[x - 1][y - 1] += 1
    return m


def kappa_cuadratico(a, b):
    """Kappa ponderado cuadratico. Devuelve None si no esta definido."""
    k = len(ETAPAS)
    n = len(a)
    obs = matriz_confusion(a, b)
    fa = [sum(fila) for fila in obs]                                   # marginales de A
    fb = [sum(obs[i][j] for i in range(k)) for j in range(k)]          # marginales de B
    num = den = 0.0
    for i in range(k):
        for j in range(k):
            w = ((i - j) ** 2) / ((k - 1) ** 2)
            num += w * obs[i][j] / n
            den += w * (fa[i] * fb[j]) / (n * n)
    if den == 0:
        return None
    return 1.0 - num / den


def calcular(a, b):
    n = len(a)
    exacta = sum(1 for x, y in zip(a, b) if x == y) / n
    pm1 = sum(1 for x, y in zip(a, b) if abs(x - y) <= 1) / n
    return {"n": n, "exacta": exacta, "pm1": pm1, "kappa": kappa_cuadratico(a, b), "matriz": matriz_confusion(a, b)}


def informe(r, solo_a, solo_b):
    L = []
    L.append("# Informe de concordancia entre etiquetadores\n")
    L.append(f"Sesiones comparadas: **{r['n']}**\n")
    if solo_a or solo_b:
        L.append(f"> ⚠️ Sesiones sin pareja: solo en A = {len(solo_a)}, solo en B = {len(solo_b)}. RF07 exige 100 % de sesiones etiquetadas por ambos.\n")
    L.append("| Métrica | Valor | Meta |\n|---|---:|---|")
    L.append(f"| Coincidencia exacta | {r['exacta']:.1%} | (se reporta) |")
    ok = "✅ cumple" if r["pm1"] >= 0.90 else "❌ no cumple"
    L.append(f"| Diferencia ≤ 1 etapa | {r['pm1']:.1%} | ≥ 90 % (RNF04) — {ok} |")
    kap = "no definido (una de las dos personas usó una sola etapa)" if r["kappa"] is None else f"{r['kappa']:.3f}"
    L.append(f"| Kappa ponderado cuadrático | {kap} | se reporta, sin umbral (RF09) |\n")
    L.append("Matriz de confusión (filas = etiquetador A, columnas = etiquetador B):\n")
    L.append("| A \\ B | E1 | E2 | E3 | E4 |\n|---|---:|---:|---:|---:|")
    for i, fila in enumerate(r["matriz"], 1):
        L.append(f"| **E{i}** | " + " | ".join(str(x) for x in fila) + " |")
    return "\n".join(L) + "\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--a", required=True, help="CSV del etiquetador A")
    ap.add_argument("--b", required=True, help="CSV del etiquetador B")
    ap.add_argument("--salida", help="archivo .md de salida (opcional)")
    args = ap.parse_args()
    A, B = leer(args.a), leer(args.b)
    comunes = sorted(set(A) & set(B))
    if not comunes:
        raise SystemExit("No hay id_sesion en comun entre los dos archivos.")
    r = calcular([A[s] for s in comunes], [B[s] for s in comunes])
    texto = informe(r, sorted(set(A) - set(B)), sorted(set(B) - set(A)))
    if args.salida:
        with open(args.salida, "w", encoding="utf-8", newline="\n") as f:
            f.write(texto)
    sys.stdout.buffer.write(texto.encode("utf-8"))


if __name__ == "__main__":
    main()
