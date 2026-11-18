#!/usr/bin/env python3
"""
Exporta /dataset.csv del ESP32 al PC.

Uso:
    pip install pyserial
    python exportar_csv.py              (detecta el puerto solo)
    python exportar_csv.py COM5         (o /dev/ttyACM0 en Linux/Mac)
    python exportar_csv.py -d mis_datos (carpeta de salida)

IMPORTANTE: cierra antes el monitor serie de PlatformIO (Ctrl+C),
porque el puerto solo lo puede usar un programa a la vez.

Genera dos archivos en la carpeta de salida (por defecto "datasets"):
    dataset_YYYYmmdd_HHMMSS.csv   -> copia con fecha (respaldo)
    dataset_ultimo.csv            -> siempre la ultima exportacion
"""
import argparse
import os
import sys
import time
from datetime import datetime

try:
    import serial
    from serial.tools import list_ports
except ImportError:
    sys.exit("Falta pyserial. Instalalo con:  pip install pyserial")

INICIO = ">>>CSV"
FIN = "<<<CSV"


def detectar_puertos():
    puertos = list_ports.comports()
    # 0x303A = Espressif (USB nativo del ESP32-S3)
    esp = [p.device for p in puertos if p.vid == 0x303A]
    if esp:
        return esp
    claves = ("usb", "serial", "cdc", "uart", "ch340", "cp210")
    return [p.device for p in puertos
            if any(k in (p.description or "").lower() for k in claves)]


def pedir_dump(puerto, baud, espera_inicio=20, espera_datos=120):
    ser = serial.Serial()
    ser.port = puerto
    ser.baudrate = baud
    ser.timeout = 0.2
    ser.dtr = False   # evita que abrir el puerto reinicie la placa
    ser.rts = False
    ser.open()

    lineas = []
    dentro = False
    t0 = time.time()
    t_envio = 0.0
    try:
        ser.reset_input_buffer()
        while True:
            ahora = time.time()
            limite = espera_datos if dentro else espera_inicio
            if ahora - t0 > limite:
                return None
            # Reintenta "dump" cada 3 s hasta que empiece a responder
            if not dentro and ahora - t_envio > 3:
                ser.write(b"dump\n")
                t_envio = ahora
            raw = ser.readline()
            if not raw:
                continue
            linea = raw.decode("utf-8", errors="replace").strip()
            if linea == INICIO:
                dentro, lineas, t0 = True, [], time.time()
            elif linea == FIN and dentro:
                return lineas
            elif dentro and linea:
                lineas.append(linea)
    finally:
        ser.close()


def main():
    ap = argparse.ArgumentParser(description="Exporta el CSV del ESP32 al PC")
    ap.add_argument("puerto", nargs="?", help="ej. COM5 o /dev/ttyACM0 (opcional)")
    ap.add_argument("-d", "--dir", default="datasets", help="carpeta de salida")
    ap.add_argument("--baud", type=int, default=115200)
    args = ap.parse_args()

    puerto = args.puerto
    if not puerto:
        candidatos = detectar_puertos()
        if len(candidatos) == 1:
            puerto = candidatos[0]
        elif not candidatos:
            sys.exit("No encontre la placa. Conectala o indica el puerto: python exportar_csv.py COM5")
        else:
            sys.exit("Hay varios puertos: " + ", ".join(candidatos) +
                     "\nIndica cual: python exportar_csv.py <puerto>")

    print(f"Conectando a {puerto} ...")
    try:
        lineas = pedir_dump(puerto, args.baud)
    except serial.SerialException as e:
        sys.exit(f"No pude abrir {puerto}: {e}\n"
                 "Cierra el monitor serie de PlatformIO y vuelve a intentar.")

    if not lineas:
        sys.exit("La placa no respondio al comando 'dump' (o el dataset esta vacio).")

    # Verificacion basica: todas las filas con el mismo numero de columnas que el encabezado
    columnas = lineas[0].count(",")
    malas = [i for i, l in enumerate(lineas[1:], start=1) if l.count(",") != columnas]
    if malas:
        print(f"AVISO: {len(malas)} fila(s) con distinto numero de columnas: {malas[:10]}")

    os.makedirs(args.dir, exist_ok=True)
    marca = datetime.now().strftime("%Y%m%d_%H%M%S")
    ruta_fecha = os.path.join(args.dir, f"dataset_{marca}.csv")
    ruta_ultimo = os.path.join(args.dir, "dataset_ultimo.csv")
    for ruta in (ruta_fecha, ruta_ultimo):
        with open(ruta, "w", encoding="utf-8", newline="\n") as f:
            f.write("\n".join(lineas) + "\n")

    print(f"OK: {len(lineas) - 1} fila(s), {columnas + 1} columnas")
    print(f"  -> {ruta_fecha}")
    print(f"  -> {ruta_ultimo}")


if __name__ == "__main__":
    main()