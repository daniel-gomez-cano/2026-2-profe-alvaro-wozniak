# Bananin — Guía de etiquetado E1–E4 y protocolo de sesión

> **BORRADOR para validar.** Cubre la actividad **A05** del cronograma (debía estar lista el 29-oct y es requisito de **RNF16**: guía escrita *antes* de etiquetar, con criterios y fotos de ejemplo, revisada por el bodeguero o acopiador). Los criterios visuales parten de la escala comercial de color de cáscara del banano (7 niveles, de verde a amarillo con pecas marrones) condensada a las 4 etapas del acta. **Es conocimiento general mío, no una fuente del repositorio: está marcado como *supuesto* hasta que lo confirme el bodeguero.**
>
> Este documento y los archivos de esta carpeta están pensados para el dataset que ya están generando. Si ya etiquetaron sesiones sin guía, conviene **revisar esas etiquetas contra esta guía** o documentar el criterio que usaron.

Archivos de esta carpeta: `plantilla_sesiones.csv` (una fila por toma) · `plantilla_etiquetas.csv` (una por etiquetador y sesión) · `concordancia_etiquetadores.py` (+ `test_concordancia.py`).

---

## 1. Escala de maduración (basada en el color de la cáscara)

| Etapa | Criterio visual *(supuesto)* | Equivale a la escala comercial de 7 niveles |
|---|---|---|
| **E1** | Cáscara **toda verde**; a lo sumo trazas muy leves de amarillo. Fruta firme. | 1–2 |
| **E2** | **Transición**: verde y amarillo conviven; predomina el verde o están parejos. | 3–4 |
| **E3** | **Amarillo dominante**: amarillo con puntas/cuello aún verdosos, hasta todo amarillo sin pecas marrones. | 5–6 |
| **E4** | **Amarillo con pecas o manchas marrones** visibles (maduración avanzada). | 7 |

**Etiqueta aparte «deteriorado» (sí/no)** — Acta §4: es una etiqueta operativa por criterio visual, **no** una medición ni una conclusión sobre inocuidad.

| Se marca «deteriorado = sí» cuando… *(supuesto, a confirmar)* |
|---|
| Las manchas marrones/negras cubren una parte grande de la cáscara visible (propuesta: **más de la mitad**), o la cáscara está rota, hundida o con líquido. |

Un banano deteriorado sigue teniendo una etapa (normalmente E4); la marca es adicional.

**Reglas de decisión en la frontera:**

- Si dudan entre dos etapas, elijan la **más verde** *(supuesto: regla fija para que ambos etiquetadores desempaten igual)* y anoten «duda» en `notas`.
- Se etiqueta **la sesión completa** (el aspecto general del banano ese día), no una toma ni una zona de la cáscara.

## 2. Fotos de referencia (las toman ustedes)

Sin fotos no se cumple RNF16. Una foto por etapa, con luz y fondo iguales, y una por «deteriorado»:

| Etapa | Foto de referencia | Banano / sesión |
|---|---|---|
| E1 | ☐ `ref_E1.jpg` | ☐ |
| E2 | ☐ `ref_E2.jpg` | ☐ |
| E3 | ☐ `ref_E3.jpg` | ☐ |
| E4 | ☐ `ref_E4.jpg` | ☐ |
| Deteriorado | ☐ `ref_deteriorado.jpg` | ☐ |

Recomendación *(supuesto)*: foto de **cada sesión** con las mismas condiciones, para que los dos etiquetadores etiqueten desde la foto, cada uno por su lado y sin necesidad de estar presentes.

## 3. Reglas del etiquetado independiente (RF07, RNF16, RF09)

1. **Dos etiquetadores, dos archivos.** Cada uno llena su propia copia de `plantilla_etiquetas.csv`, con su nombre en `etiquetador`.
2. **Sin ver la etiqueta del otro.** No se comparan hasta que ambos terminen.
3. **Sin ver el índice de COV ni los datos del sensor** (RNF16, *propuesta* para no contaminar la etiqueta). La etiqueta sale solo del color que se ve.
4. **Una etiqueta por sesión**, no por toma (RF07). `id_sesion` no se repite en un archivo.
5. **Sin consultar al equipo** mientras etiquetan: si la guía no alcanza para decidir, la guía está mal y se corrige (RNF16: los dos deben terminar sin preguntar).
6. Registrar `fecha_etiquetado` de cada fila.
7. Cuando terminen: `python concordancia_etiquetadores.py --a etiquetas_A.csv --b etiquetas_B.csv --salida informe_concordancia.md`
   Meta: **≥ 90 % dentro de ±1 etapa** (RNF04). El kappa ponderado se reporta sin umbral (RF09).

**Piloto antes de empezar (recomendado):** ambos etiquetan las mismas 10 fotos y corren el script. Si el ±1 sale < 90 %, se afina la guía *antes* de etiquetar todo.

## 4. Protocolo de cada sesión (checklist)

Valores marcados *(prop.)* son propuestas del `Requisitos.md`, ajustables en la configuración del nodo.

**Antes de empezar**
- ☐ **Precalentar el MQ-135** mínimo 10 min *(prop., RF23a)*.
- ☐ **Calibrar el TCS34725** con la referencia blanca/gris. Sin calibración del mismo día, el nodo rechaza la sesión (RF02).
- ☐ **Línea base del MQ-135**: cámara **ventilada**, aire limpio, registrar lectura (RF23b). Sin línea base, el nodo rechaza la sesión.

**Medición**
- ☐ Colocar el banano (**B01–B25**) en la cámara oscura con LED propio; cerrar.
- ☐ Esperar el **tiempo fijo de acumulación: 60 s** *(prop., RF23c)*. Debe ser **igual en todas las sesiones**.
- ☐ **5 tomas** separadas **≥ 2 s** en **3 posiciones** distintas de la cáscara, todas con el mismo `id_sesion` *(prop., RF03)*. El LED debe estar encendido durante cada lectura (RF01).

**Al terminar**
- ☐ Confirmar que llegaron **5 tomas** al backend con ambas lecturas y el mismo `id_sesion`.
- ☐ Ventilar la cámara. Tomar la **foto de la sesión**. Anotar cualquier incidencia (se cayó el WiFi, se movió el banano…).

**Frecuencia:** al menos **1 sesión por día por banano** *(prop., RF06)*, desde un estado inicial común o conocido hasta la etapa final o el cierre de la recolección. *No se acelera la maduración artificialmente* (advertencia del Cronograma).

## 5. Identificadores y columnas

| Campo | Formato sugerido *(supuesto)* | Ejemplo |
|---|---|---|
| `id_banano` | `B01`…`B25` (RF06) | `B03` |
| `id_sesion` | `<id_banano>-S<nn>` | `B03-S07` |
| `id_toma` | `<id_sesion>-T<n>` | `B03-S07-T2` |
| `posicion_cascara` | 1, 2 o 3 (RF03) | `2` |
| `timestamp_utc` | UTC por NTP (RF04) | `2026-11-10T14:05:31Z` |

Las columnas de `plantilla_sesiones.csv` siguen **RF04/RF10** (ID banano, ID sesión, ID toma, marca de tiempo UTC, R, G, B, clear, valores de referencia del TCS34725, valor crudo del MQ-135 y línea base del MQ-135) más `posicion_cascara`. **Ojo:** el `Calidad.md` lista otras columnas para el CSV de IoT (`id, estado, dias_estimados, org_vol, og_vol, norm, R, G, B, C, r_norm…`) que no coinciden con los requisitos, y habla de `dias_estimados` aunque no hay regresión de días (Acta §4.2). Conviene unificar el esquema antes de seguir acumulando datos.

## 6. Aprobación (antes de etiquetar el dataset completo)

- ☐ Revisada por el bodeguero/acopiador (Fenibal Botero Giraldo, Acta §6) y escala confirmada **por escrito** (RNF16).
- ☐ Fotos de referencia E1–E4 y «deteriorado» agregadas.
- ☐ Piloto de 10 fotos hecho; ±1 ≥ 90 %.
- ☐ Aprobada por el equipo; fecha: ☐ _____

> Si los dos etiquetadores ya etiquetaron sesiones antes de esta guía, lo honesto es dejar constancia en el informe de métricas de que **la guía se escribió después**, en vez de aparentar que existía antes (RNF16 y A05).
