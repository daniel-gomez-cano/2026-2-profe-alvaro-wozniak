# Guía rápida: calibrar y crear el dataset de bananos

Nodo ESP32-S3 con un sensor de color TCS34725 y un sensor de gas MQ-135. Funciona sin internet: el dataset se guarda en la memoria flash de la placa y se exporta al PC como CSV.

## 1. Estructura del repo

```
proyecto/
├─ platformio.ini
├─ src/
│  └─ main.cpp            <- copia de main_dataset.cpp
├─ exportar_csv.py        <- exporta el CSV de la placa al PC
├─ .vscode/
│  └─ tasks.json          <- (opcional) atajo de teclado para exportar
└─ datasets/              <- aquí caen los CSV exportados
```

`main.cpp` ya incluye los comandos de calibración, así que no necesitas un firmware aparte. Si guardas el firmware de solo calibración, déjalo **fuera de `src/`** (por ejemplo en `extras/`), porque PlatformIO compila todo lo que hay en `src/`.

## 2. Requisitos

- VS Code con la extensión **PlatformIO**.
- **Python 3** y la librería `pyserial` (solo para exportar): `pip install pyserial`
- Placa ESP32-S3 DevKitC-1, TCS34725 y MQ-135.

### platformio.ini

```ini
[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
board_upload.flash_size = 16MB
board_build.partitions = default_16MB.csv
board_build.arduino.memory_type = qio_opi
board_build.filesystem = littlefs
build_flags =
	-DBOARD_HAS_PSRAM
	-DARDUINO_USB_CDC_ON_BOOT=1
lib_deps =
	adafruit/Adafruit TCS34725
lib_ldf_mode = deep
monitor_speed = 115200
monitor_filters = send_on_enter
monitor_echo = yes
```

`send_on_enter` es obligatorio: los comandos se escriben y se envían con **Enter**.

## 3. Conexiones

| Componente | Pin ESP32-S3 |
|---|---|
| TCS34725 SDA | GPIO 8 |
| TCS34725 SCL | GPIO 9 |
| TCS34725 VIN / GND | 3V3 / GND |
| LED de iluminación | GPIO 5 |
| MQ-135 salida analógica (AO) | GPIO 4 |
| MQ-135 VCC / GND | 3V3 / GND |

El MQ-135 va alimentado a **3.3 V**, así que su salida nunca supera 3.3 V y no necesita divisor. A 3.3 V el sensor es menos sensible: usa `mq_ratio` como tendencia y ignora los ppm.

## 4. Subir el firmware y abrir el monitor

1. Conecta la placa por USB.
2. En PlatformIO: **Upload** (o `pio run -t upload`).
3. Abre **Monitor** (o `pio device monitor`). Verás el estado de los sensores, la calibración guardada y la lista de comandos.

## 5. Calibración (una sola vez, y de nuevo si cambian las condiciones)

Antes de calibrar, deja encendido el MQ-135: mínimo 15 min, y idealmente varias horas la primera vez.

| Paso | Comando | Qué hacer |
|---|---|---|
| Oscuro | `k` | Tapa el sensor con algo opaco y negro mate. El resultado debe quedar cerca de 10–30 por canal. |
| Blanco | `w` | Pon una tarjeta blanca a la **misma distancia y con la misma luz** con que medirás el banano. Si `C` pasa de 60000, está saturado. |
| Aire limpio | `b` | Con el sensor ya precalentado y en aire limpio, guarda R0 del MQ-135. |
| Revisar | `p` | Muestra la calibración guardada. |

Los valores quedan en la memoria de la placa y se cargan solos al reiniciar.

**Cuándo repetir `k` y `w`:** si cambias la distancia, el LED, la ganancia o el tiempo de integración del sensor.

**Prueba de validación:** con `v` (vista en vivo), la tarjeta blanca debe dar `V ≈ 1` y `S ≈ 0`.

## 6. Crear el dataset

Cada fila del dataset es **una toma de un banano**, con 3 puntos de medición: parte superior (`top`), central (`mid`) e inferior (`bot`).

Los **días** y el **estado de maduración** se ingresan a mano. El **id** se genera solo.

### Flujo normal

```
n          nuevo banano (id automático: B001, B002, ...)
d 0        días desde la cosecha
e 1        estado de maduración (e solo, lista la escala)
m          capturar
```

Al escribir `m`, el programa te guía:

1. Te pide la parte **top**: coloca el sensor y presiona Enter.
2. Muestra el resultado de esa parte. **Enter** acepta, `r` repite, `x` cancela.
3. Repite con **mid** y con **bot**.
4. Lee el MQ-135 y guarda la fila.

### Al día siguiente, con el mismo banano

```
d +        suma 1 día   (o d 2 para fijar el valor)
e 2        actualiza el estado si cambió
m          nueva captura
```

No uses `n` otra vez: el id se mantiene. Solo se usa `n` para un banano nuevo.

### Todos los comandos

| Comando | Función |
|---|---|
| `m` | Captura una fila |
| `n` | Banano nuevo (id automático, días en 0, estado mínimo) |
| `id B007` | Fija el id a mano |
| `d 3` / `d 2.5` / `d +` | Fija los días / suma 1 día |
| `e 4` | Fija el estado de maduración (`e` sin número lista la escala) |
| `s` | Muestra la sesión actual y el total de filas |
| `l` / `l 10` | Muestra las últimas filas (5 por defecto) |
| `edit 12 estado 5` | Corrige la fila 12 (campos: `id`, `dias`, `estado`) |
| `u` | Borra la última fila |
| `del 12` | Borra la fila 12 |
| `dump` | Imprime todo el CSV |
| `borrar si` | Vacía el dataset (no toca la calibración) |
| `v` | Vista en vivo on/off, para colocar el sensor |
| `k`, `w`, `b`, `p` | Calibración: oscuro, blanco, R0 y ver calibración |
| `h` | Ayuda |

La sesión (id, días y estado) queda guardada, aunque reinicies la placa.

## 7. Escala de maduración

Por defecto es la escala de von Loesecke del 1 al 7. Si tu guía de etiquetado usa otra, edita estas líneas al inicio de `main.cpp`:

```cpp
const int ESTADO_MIN = 1;
const int ESTADO_MAX = 7;
const char* ESTADOS[] = { "1 verde", "2 ...", ... };
```

## 8. Exportar el CSV al PC

El dataset vive en la flash de la placa (`/dataset.csv`), no en tu computador. Para sacarlo:

1. **Cierra el monitor serie** (Ctrl+C), porque el puerto solo lo puede usar un programa.
2. Ejecuta `python exportar_csv.py` (o `python exportar_csv.py COM5` si hay varios puertos).
3. Los archivos quedan en `datasets/`:
   - `dataset_AAAAMMDD_HHMMSS.csv`: copia con fecha, como respaldo.
   - `dataset_ultimo.csv`: la última exportación.

**Atajo con una tecla (opcional):** crea `.vscode/tasks.json`:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Exportar CSV del ESP32",
      "type": "shell",
      "command": "python exportar_csv.py",
      "problemMatcher": []
    }
  ]
}
```

Y en los atajos de teclado de VS Code (`Ctrl+Shift+P` → *Preferences: Open Keyboard Shortcuts (JSON)*):

```json
{
  "key": "ctrl+alt+e",
  "command": "workbench.action.tasks.runTask",
  "args": "Exportar CSV del ESP32"
}
```

**Alternativa manual:** escribe `dump` en el monitor, copia lo que aparece entre `>>>CSV` y `<<<CSV` y guárdalo como `.csv`.

## 9. Columnas del CSV

```
id_banano, dias, estado,
R_top, R_mid, R_bot,   G_top, G_mid, G_bot,   B_top, B_mid, B_bot,
H_top, H_mid, H_bot,   S_top, S_mid, S_bot,   V_top, V_mid, V_bot,
C_top, C_mid, C_bot,
mq_mv, mq_rs, mq_ratio, valido
```

| Columna | Significado |
|---|---|
| `R, G, B` | Reflectancia calibrada de 0 a 1 (usa el blanco y el oscuro) |
| `H, S, V` | Matiz (0–360°), saturación y brillo (0–1), calculados desde R, G, B |
| `C` | Valor crudo del canal clear, para auditar saturación |
| `mq_mv` | Voltaje del MQ-135 en mV |
| `mq_rs` | Resistencia del sensor en kΩ |
| `mq_ratio` | Rs / R0: baja cuando hay más gases |
| `valido` | 1 si las 3 partes no están saturadas ni demasiado oscuras (`C < 60000`, `V ≥ 0.1`) y el MQ-135 ya terminó el precalentamiento |

## 10. Buenas prácticas

- **Mantén la geometría fija:** misma distancia, ángulo y luz que usaste al calibrar el blanco. Lo ideal es una carcasa o cámara oscura con el LED propio.
- **Mide siempre las partes en el mismo orden:** top, mid, bot.
- **Descarta las filas con `valido = 0`** al entrenar, o repite la toma.
- **Con `V` menor a 0.1 el matiz `H` es ruido.** No lo interpretes.
- **Haz respaldo:** exporta el CSV al terminar cada sesión de captura. El dataset se conserva al subir firmware nuevo, pero se pierde con un `erase flash`.
- **No cambies `GAIN` ni el tiempo de integración** sin repetir `k` y `w`.
- **Si cambias las columnas del CSV** (por ejemplo, al editar el código), exporta primero lo que tengas y luego usa `borrar si`. Si no, el archivo conserva el encabezado viejo.

## 11. Problemas comunes

| Síntoma | Qué revisar |
|---|---|
| Los comandos con argumento no funcionan (`d 3`) | Falta `monitor_filters = send_on_enter` en `platformio.ini`; reabre el monitor |
| `TCS34725: NO detectado` | Revisa SDA (GPIO 8), SCL (GPIO 9), 3V3 y GND |
| El script no abre el puerto | Cierra el monitor serie de PlatformIO |
| El script no encuentra la placa | Pasa el puerto a mano: `python exportar_csv.py COM5` |
| Muchas filas con `valido = 0` | `C` saturado (baja `GAIN`), `V` muy bajo (acerca el sensor) o MQ-135 aún precalentando |
| `mq_ratio` lejos de 1 en aire limpio | Repite `b` con el sensor bien precalentado |
