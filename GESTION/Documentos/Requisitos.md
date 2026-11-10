**Requisitos funcionales y no funcionales (versión 2: incorpora el sensor MQ-135 y se alinea con la EDT)**

Sistema IoT de apoyo a la decisión para el destino del banano según su estado de maduración

Esta versión se alinea con el acta de constitución del 5 de octubre de 2026 (versión con el sensor MQ-135) y con la EDT (Historial, Dashboard y ChatBot). Todos los requisitos tienen criterio de verificación. Los valores marcados como "Propuesta" no están en el acta ni en la EDT y deben ser confirmados por el equipo.

# 1. Decisiones adoptadas

| Tema | Decisión | Base en el acta / EDT |
| :---- | :---- | :---- |
| **Sensores del nodo** | El nodo tiene dos sensores: TCS34725 (color RGB) y MQ-135 (índice relativo de COV). Se mantiene un solo nodo, sin cámara. | Acta §2, §3.1, §4.1; EDT 2.1.1. |
| **Papel del MQ-135** | El índice de COV complementa al color y no lo reemplaza. Es un valor relativo (sin conversión a ppm ni identificación de compuestos). Es un indicio, no un diagnóstico de deterioro. | Acta §2, §4 (definiciones) y §4.2. |
| **Aporte del segundo sensor** | Se compara el clasificador solo con color frente a color + MQ-135. Si el MQ-135 no aporta, el resultado se reporta igualmente. | Acta OE3, §5, §7; EDT 2.1.2. |
| **Escala de madurez** | Se adopta la escala de 4 etapas (E1–E4). Se descarta la de 7 etapas del planteamiento. | Acta §4, §5, §7. |
| **Clasificación vs. regresión** | Solo clasificación de etapa. No hay regresión de días restantes. | Acta §4.2. |
| **Días restantes** | Los días restantes se leen de la tabla de contexto según la etapa. No son una predicción del modelo. | Acta §4.2. |
| **Moneda y costo de sensores** | Los costos se expresan en COP: sensor RGB de 20.000 a 80.000 y módulo MQ-135 de 10.000 a 25.000 (estimado, por confirmar con el proveedor). | Acta §8. |
| **Umbral de kappa** | Kappa se calcula y se reporta, sin umbral de aprobación. | Acta §4.1. |
| **Prioridades** | RF05, RF09 y RF16 son Must. RF22 y RF23 (MQ-135) también son Must, porque el acta exige lecturas de ambos sensores y la comparación de aporte. | Acta §3, §4.1, OE1, OE3. |
| **Alcance de software** | Además del dashboard, el sistema incluye una pantalla de Historial (lista, filtros y descarga) y un ChatBot que consulta los datos almacenados. | EDT paquetes 1 y 3. |
| **Uso del COV en el LLM** | El índice de COV se entrega al LLM como contexto. La capa de reglas decide con la tabla de contexto y no con el COV. (Propuesta) | Acta §4 (definiciones). |

# 2. Glosario y prioridades

| Término | Definición |
| :---- | :---- |
| **Toma** | Una medición puntual del nodo: valores crudos del TCS34725 (canales R, G, B y clear, de 16 bits) y valor analógico crudo del MQ-135. |
| **Sesión** | Conjunto de tomas de un mismo banano en un mismo momento. Tiene un ID de sesión y una marca de tiempo. |
| **Etapa (E1–E4)** | Etapa de maduración según el color de la cáscara, en orden creciente (E1 la menos madura, E4 la más madura). La guía de etiquetado (RNF16) fija el criterio visual de cada etapa y cuál se marca como "deteriorado". |
| **Índice de COV** | Lectura relativa del MQ-135: valor de la sesión respecto a su línea base en aire limpio. No es una concentración en ppm y no identifica compuestos. Se usa como señal complementaria y como contexto para el LLM. |
| **Línea base (MQ-135)** | Lectura del MQ-135 en aire limpio, con la cámara ventilada y el sensor ya precalentado, registrada al inicio de cada sesión. |
| **Cámara de medición** | Contenedor oscuro con LED propio, ventilable, donde se mide el banano. La ventilación permite obtener la línea base del MQ-135. |
| **Deteriorado** | Etiqueta operativa del protocolo de etiquetado (criterio visual). No se mide y no implica conclusión sobre inocuidad. |
| **Vida útil restante** | Días que la tabla de contexto asigna a cada etapa. No es salida del modelo. |
| **Etiquetador** | Persona que asigna una etapa a cada sesión sin ver la etiqueta de la otra persona. |
| **Prioridad** | MoSCoW: Must (indispensable para cumplir el acta), Should (importante, con alternativa temporal), Could (deseable, solo si hay tiempo). |

# 3. Requisitos funcionales

## Nodo sensor (ESP32-S3 + TCS34725 + MQ-135)

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF01** | El nodo debe leer los valores RGB crudos (R, G, B y clear) del sensor TCS34725 en cada toma, dentro de un contenedor oscuro con LED propio encendido durante la lectura. | Prueba de banco: cada toma entrega 4 canales con valor mayor que 0 y el LED se enciende durante la lectura. |
| **RF02** | Antes de iniciar una sesión, el operador debe poder lanzar la calibración del TCS34725 con referencia blanca/gris (pulsador o comando serial). El nodo debe guardar los valores de referencia junto con la sesión y rechazar sesiones si no hay una calibración del mismo día (Propuesta). | Demostración: sin calibración del día, el nodo rechaza iniciar sesión; con calibración, los valores de referencia aparecen en el registro de la sesión. |
| **RF03** | El nodo debe tomar 5 lecturas por banano en cada sesión, separadas al menos 2 s y en 3 posiciones distintas de la cáscara, todas con el mismo ID de sesión. Cada toma incluye la lectura del TCS34725 y la del MQ-135 (valores propuestos, ajustables en la configuración). | Cada sesión registrada en el backend contiene 5 tomas con el mismo ID de sesión, marcas de tiempo separadas ≥ 2 s y ambas lecturas en cada toma. |
| **RF04** | El nodo debe enviar cada toma al backend por WiFi con HTTPS POST en JSON, con los campos: ID de banano, ID de sesión, ID de toma, marca de tiempo UTC del nodo (sincronizada por NTP), R, G, B, clear, valores de referencia del TCS34725, valor analógico crudo del MQ-135 y línea base del MQ-135. El backend responde con código 2xx como confirmación. | Prueba de integración: una toma enviada aparece en el backend con todos los campos y el nodo recibe el 2xx. |
| **RF05** | Si no hay conexión o el backend no confirma, el nodo debe guardar las tomas (con ambas lecturas) en un buffer local de hasta 500 tomas (Propuesta), reenviarlas en orden cronológico al reconectar y borrar cada una solo tras recibir confirmación. El backend debe descartar duplicados por ID de toma. Si el buffer se llena, se descarta la toma más antigua y se incrementa un contador de pérdidas. | Prueba: desconectar el WiFi 10 min durante una sesión; al reconectar llegan todas las tomas, en orden y sin duplicados. |
| **RF22** | El nodo debe leer el MQ-135 por una entrada analógica y enviar en cada toma el valor crudo del ADC (promedio de varias muestras, Propuesta). El nodo no calcula el índice de COV ni lo convierte a ppm: lo calcula el backend (RF11). | Prueba de banco: cada toma trae un valor del MQ-135 mayor que 0; al acercar un banano maduro, el valor sube respecto a la línea base. |
| **RF23** | Antes de cada sesión, el nodo debe: (a) esperar el precalentamiento del MQ-135 (mínimo 10 min, Propuesta); (b) registrar la línea base en aire limpio con la cámara ventilada; (c) cerrar la cámara con el banano y esperar un tiempo fijo de acumulación (60 s, Propuesta) antes de las tomas. El nodo debe rechazar iniciar una sesión sin línea base de esa sesión y guardar la línea base junto con ella. | Demostración: sin precalentamiento o sin línea base, el nodo rechaza iniciar la sesión; con ambos, la línea base aparece en el registro de la sesión y el tiempo de acumulación es el mismo en todas las sesiones. |

## Dataset y etiquetado

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF06** | El sistema debe registrar y almacenar las tomas de color y de COV de 25 bananos, cada uno con ID único (B01 a B25), con al menos 1 sesión por día (Propuesta) desde un estado inicial común o conocido hasta alcanzar la etapa final o el cierre de la recolección. | La base de datos contiene 25 IDs únicos, cada uno con al menos 1 sesión por día durante su seguimiento, con tomas de ambos sensores. |
| **RF07** | Dos etiquetadores independientes deben asignar una etapa E1–E4 a cada sesión, cada uno en su propio dataset y sin ver la etiqueta del otro. La etiqueta es por sesión, no por toma, y se registra en la herramienta del proyecto (hoja o formulario definido por el equipo). | Dos datasets separados, ambos con etiqueta para el 100 % de las sesiones; cada registro guarda el etiquetador y la fecha. |
| **RF08** | El sistema puede permitir registrar datos complementarios opcionales por sesión: °Brix (número ≥ 0) y días desde la compra (entero ≥ 0). Este requisito no está en el acta; se implementa solo si sobra tiempo. | Si se implementa: los campos aceptan solo valores válidos y se guardan asociados a la sesión. |
| **RF09** | El sistema debe calcular la concordancia entre los dos etiquetadores: porcentaje de sesiones con diferencia ≤ 1 etapa y kappa ponderado cuadrático (escala ordinal). Debe incluir el resultado en el informe de métricas. | Existe un informe con el % ±1 y el kappa ponderado, calculados con un script del repositorio. |

## Backend y preprocesamiento

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF10** | El backend debe guardar cada toma como serie de tiempo, con ID de banano, ID de sesión, ID de toma, marca de tiempo, valores RGB/clear, valores de referencia del TCS34725, valor crudo del MQ-135 y línea base del MQ-135, y permitir consultarlas por banano y rango de fechas. | Consulta por banano y rango de fechas devuelve las tomas en orden cronológico con todos los campos. |
| **RF11** | Por cada sesión completa, el backend debe calcular: (a) el promedio de las tomas por canal de color; (b) las variables HSV a partir de ese promedio; (c) la velocidad de cambio de color como variación de H por hora entre sesiones consecutivas del mismo banano; (d) las horas acumuladas desde la primera sesión de ese banano; (e) el índice de COV de la sesión, como promedio del valor crudo del MQ-135 dividido entre la línea base de la sesión (fórmula Propuesta); (f) la velocidad de cambio del índice de COV por hora entre sesiones consecutivas (Propuesta). | Prueba unitaria con datos de ejemplo: los valores calculados coinciden con un cálculo manual. |

## IA determinística

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF12** | El sistema debe clasificar cada sesión en una etapa E1–E4 a partir de las variables de RF11 (color y, según la comparación de RF14, el índice de COV) y guardar la etapa estimada. La inferencia se ejecuta en el backend al completarse la sesión. No se estiman días restantes. Si el MQ-135 no mejora el resultado, se usa el modelo solo color y se reporta igual la comparación. | Una sesión nueva recibe etapa estimada en el backend sin intervención manual. |
| **RF13** | El entrenamiento y la validación deben dividir los datos por banano (validación cruzada agrupada por ID, por ejemplo dejando un banano fuera), nunca al azar entre tomas o sesiones. | Revisión del código: ningún ID de banano aparece a la vez en entrenamiento y validación. |
| **RF14** | El sistema debe comparar al menos 2 modelos (por ejemplo regresión logística y bosque aleatorio, Propuesta). Cada modelo se evalúa en dos configuraciones: solo color y color + MQ-135. Para cada combinación se reporta: matriz de confusión, porcentaje de coincidencia exacta y porcentaje dentro de ±1 etapa en bananos no vistos. | El informe de métricas muestra los 2 modelos, en las 2 configuraciones, con las tres métricas, y una conclusión sobre el aporte del MQ-135. |

## LLM y decisión

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF15** | El LLM debe recibir la etapa estimada, la vida útil restante, el índice de COV (como contexto, Propuesta) y la tabla de contexto, y recomendar un destino de un catálogo cerrado: vender_local, enviar_lejos, procesar, descartar. | En los escenarios de prueba, la acción devuelta pertenece siempre al catálogo. |
| **RF16** | El LLM debe devolver un valor de mercado estimado en COP por kg (Propuesta), calculado con los precios de referencia de la tabla de contexto para la etapa y el destino. La capa de reglas debe verificar que el valor esté entre 0 y el precio máximo de la tabla para esa etapa. | En los escenarios de prueba, el 100 % de los valores está dentro del rango de la tabla. |
| **RF17** | La respuesta del LLM debe ser un JSON con esquema fijo y estos campos: accion (enum del catálogo), justificacion (texto de hasta 300 caracteres), confianza (número de 0 a 1) y valor_estimado_cop (número ≥ 0). Si la respuesta no cumple el esquema, el sistema reintenta una vez; si vuelve a fallar, no muestra destino y registra el error. | Validación contra el esquema JSON: las respuestas inválidas se rechazan y el dashboard muestra un mensaje de error. |
| **RF18** | Una capa de reglas debe validar la decisión del LLM y bloquear las absurdas, como mínimo: (a) enviar_lejos con vida útil restante menor que el tiempo de transporte; (b) un destino cuya calidad mínima no cumple la etapa; (c) una acción fuera del catálogo. Al bloquear, el sistema debe sustituirla por la acción que la tabla de contexto define para esa etapa, marcarla como "corregida por reglas" y guardar la decisión original del LLM. | Casos de prueba para las reglas (a), (b) y (c): cada uno se bloquea y se sustituye por la acción de la tabla. |
| **RF19** | El sistema debe mantener una tabla de contexto versionada en el repositorio (CSV o JSON) con, por etapa y destino: calidad mínima, tiempo de transporte, vida útil en días y precio de referencia, cada fila con su fuente o la marca "supuesto". Debe ser validada por el cliente. | El archivo existe en el repositorio, no tiene celdas vacías y hay constancia de validación del cliente. |

## Visualización: dashboard (EDT paquete 2)

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF20** | El dashboard debe permitir seleccionar un banano y mostrar: la última lectura (R, G, B), el índice de COV, la etapa estimada, la vida útil restante (de la tabla), el destino recomendado, la justificación en texto legible, el valor estimado y un indicador si la decisión fue corregida por reglas. Debe actualizarse sin recargar la página cuando llega una lectura nueva. | Demostración: al llegar una sesión nueva, el dashboard muestra todos los campos sin recargar, dentro del límite de RNF02. |
| **RF21** | El dashboard debe mostrar la serie de tiempo por banano: gráfico de color (H), gráfico del índice de COV y gráfico de etapa en el tiempo, con tabla de sesiones y selector de rango de fechas. | Para un banano con al menos 5 sesiones, los gráficos y la tabla muestran todas las sesiones del rango seleccionado. |

## Visualización: historial (EDT paquete 1)

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF24** | El sistema debe mostrar una lista de las lecturas almacenadas con fecha y hora, valores de color, valor de COV, etapa y destino, ordenadas con la más reciente primero. | Con datos de prueba, la lista muestra todos los campos y la primera fila es la lectura más reciente. |
| **RF25** | El sistema debe generar un archivo descargable (PDF o CSV) con los datos del historial que se está viendo, respetando el filtro activo. | Con un filtro activo, el archivo descargado contiene exactamente los registros que muestra la lista filtrada. |
| **RF26** | El historial debe poder filtrarse por rango de fechas y por etapa. La lista debe mostrar solo los registros que cumplen el filtro. | Con filtros de fecha y de etapa, todos los registros mostrados cumplen ambos filtros y no falta ninguno. |
| **RF27** | La pantalla del historial debe ser accesible desde el dashboard y contener la lista, los filtros y la descarga funcionando. | Demostración: desde el dashboard se llega al historial y se usan lista, filtros y descarga sin errores. |

## ChatBot (EDT paquete 3)

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF28** | El sistema debe ofrecer un chat con entrada de texto y respuestas visibles, accesible desde el dashboard. | Demostración: desde el dashboard se abre el chat, se envía una pregunta y se ve la respuesta. |
| **RF29** | El LLM del chat debe responder con base en los datos almacenados (lecturas, etapas, destinos) y no debe inventar valores que no estén en el sistema. Si el dato pedido no existe, debe decirlo. | Conjunto de preguntas de prueba definido por el equipo (cantidad por definir, Propuesta: 10): las respuestas coinciden con los datos almacenados y ninguna incluye valores inexistentes. |

# 4. Requisitos no funcionales

## Rendimiento y fiabilidad

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RNF01** | El nodo debe transmitir de forma continua durante 7 días o más, y el backend debe recibir al menos el 95 % de las tomas programadas con la lectura de ambos sensores (recibidas ÷ programadas). | Prueba de 7 días: el conteo de tomas recibidas con ambas lecturas frente a las programadas es ≥ 95 %. |
| **RNF02** | El tiempo desde que se completa una sesión hasta que el dashboard muestra etapa y recomendación debe ser menor a 30 s en el 95 % de las sesiones (Propuesta, no está en el acta). | Medición de tiempos en 20 sesiones: al menos 19 cumplen. |

## Exactitud y calidad del modelo

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RNF03** | El clasificador debe tener al menos 85 % de coincidencia dentro de ±1 etapa en bananos no vistos. Además se reporta la coincidencia exacta, porque con 4 etapas un acierto al azar ya cae dentro de ±1 el 62,5 % de las veces. | Informe de métricas con validación por banano (RF13): ≥ 85 % dentro de ±1 en el modelo desplegado. |
| **RNF04** | La coincidencia entre etiquetadores debe ser ≥ 90 % dentro de ±1 etapa. El kappa ponderado se reporta sin umbral. | Resultado de RF09 sobre el dataset completo. |
| **RNF05** | Con la misma entrada repetida 20 veces, el LLM debe devolver la misma acción en al menos 18 de 20 (90 %), con la temperatura configurada en su valor más bajo. | Script de repetición que registra las 20 respuestas y calcula el porcentaje. |
| **RNF06** | El 100 % de las respuestas del LLM de decisión en las pruebas (RNF05 y RNF07) debe ser JSON válido según el esquema de RF17. | Validador de esquema sobre todas las respuestas registradas. |
| **RNF07** | En 12 escenarios de prueba definidos por el equipo, la acción final debe coincidir con la acción esperada según la tabla de reglas en al menos 80 % (10 de 12). | Tabla de 12 escenarios con acción esperada y acción obtenida. |

## Costo y hardware

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RNF08** | El sensor RGB debe costar entre 20.000 y 80.000 COP, y el módulo MQ-135 entre 10.000 y 25.000 COP (estimado, por confirmar con el proveedor), reutilizando la ESP32-S3 disponible. | Factura o cotización de cada sensor dentro de su rango. |
| **RNF09** | El prototipo debe usar un solo nodo con dos sensores (TCS34725 y MQ-135), sin cámara ni otros sensores de gas (SGP30/TVOC) ni de etileno. | Revisión del montaje y de la lista de materiales. |
| **RNF10** | El entrenamiento y la inferencia del modelo deben ejecutarse fuera de la ESP32-S3. | Revisión de la arquitectura: el firmware solo mide y envía. |
| **RNF18** | El sistema debe registrar cada llamada al LLM, tanto las de decisión como las del ChatBot (fecha, tokens usados), para controlar costo y cuota de la API. | Existe un registro de llamadas y el total no supera la cuota ni el presupuesto. |
| **RNF19** | El MQ-135 debe conectarse a un pin analógico compatible con WiFi (en la ESP32-S3, un pin de ADC1) y con divisor de voltaje para los 3,3 V de la ESP32-S3. | Revisión del esquema y del montaje; el sensor se lee con el WiFi activo sin lecturas inválidas. |

## Mantenibilidad y reproducibilidad

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RNF11** | El entrenamiento debe ser reproducible: semilla fija, versiones de dependencias fijadas y mismos datos dan las mismas métricas. | Dos ejecuciones del entrenamiento producen métricas idénticas. |
| **RNF12** | La configuración (credenciales WiFi, API key del LLM, umbrales) debe estar fuera del código fuente, en un archivo no versionado, con un archivo de ejemplo en el repositorio. | Búsqueda en el repositorio: no hay claves ni contraseñas; existe el archivo de ejemplo. |
| **RNF13** | Los supuestos (tabla de contexto, definición de "deteriorado", valores propuestos, tiempos del MQ-135 y fórmula del índice de COV) deben estar documentados y marcados como tales. | Revisión de la documentación: cada supuesto aparece con la marca "supuesto". |
| **RNF17** | Una persona ajena al equipo debe poder reproducir la instalación (incluido el precalentamiento y la línea base del MQ-135) y una lectura de punta a punta usando solo el README. | Prueba con una persona externa que no pide ayuda al equipo. |

## Seguridad

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RNF14** | La comunicación con el backend y con la API del LLM debe ir cifrada (HTTPS), y el nodo debe autenticarse con un token propio. (Propuesta, no está en el acta.) | Revisión: no hay endpoints HTTP sin cifrar y una petición sin token es rechazada. |

## Usabilidad

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RNF15** | El dashboard debe ser entendible sin conocimientos técnicos y funcionar en el navegador de un PC. Un usuario nuevo debe identificar el destino recomendado y su justificación en menos de 1 minuto, sin ayuda (el minuto es una Propuesta). | Prueba con el cliente: identifica destino y justificación en menos de 1 minuto. |
| **RNF16** | El 100 % de las sesiones debe ser etiquetable con la guía de etiquetado escrita antes de etiquetar, con criterios y fotos de ejemplo para E1–E4, basada en el color de la cáscara. Los etiquetadores no deben ver el índice de COV al etiquetar (Propuesta, para no contaminar la etiqueta). La guía debe ser entendible para etiquetadores sin experiencia y la escala de 4 etapas debe ser revisada por el bodeguero o acopiador. | Los dos etiquetadores terminan sin consultar al equipo y el bodeguero confirma la escala por escrito. |

# 5. Trazabilidad

## 5.1 Por hito del acta

| Hito del acta | Requisitos que lo cubren |
| :---- | :---- |
| **H2. Nodo sensor montado (21 oct)** | RF01–RF05, RF10, RF22, RF23, RNF01, RNF08–RNF10, RNF19 |
| **H3. Dataset con doble etiquetado (18 nov)** | RF06–RF09, RF11, RNF04, RNF16 |
| **H4. Clasificador, LLM, tabla y reglas (2 dic)** | RF11–RF19, RNF03, RNF05–RNF07, RNF11 |
| **H5. Integración, pruebas y entrega (16 dic)** | RF20, RF21, RF24–RF29, RNF02, RNF03, RNF15, RNF17 |

## 5.2 Por paquete de la EDT

| Código EDT | Paquete de trabajo | Requisitos que lo cubren |
| :---- | :---- | :---- |
| 0 | Bananin (integración y demo) | RF20, RNF02, RNF15, RNF17 |
| 1.1 | Lista de datos | RF24 |
| 1.2 | Descargar informe | RF25 |
| 1.3 | Filtrar datos | RF26 |
| 1.4 | Interfaz (historial) | RF27 |
| 2.1.1 | Sensor | RF01–RF05, RF22, RF23, RNF01, RNF08–RNF10, RNF19 |
| 2.1.2 | Modelo determinista | RF06–RF09, RF11–RF14, RNF03, RNF04, RNF11, RNF16 |
| 2.1.3 | Interfaz con los valores | RF20, RF21 |
| 2.2.1 | Modelo no determinista | RF15–RF19, RNF05–RNF07, RNF18 |
| 2.2.2 | Interfaz con texto | RF20 |
| 3.1 | Interfaz (ChatBot) | RF28 |
| 3.2 | LLM (ChatBot) | RF29, RNF18 |

# 6. Cambios de esta revisión (incorporación del MQ-135 y alineación con la EDT)

| ID | Cambio |
| :---- | :---- |
| **Título, §1, §2** | Se agrega el MQ-135 como segundo sensor. Se definen índice de COV, línea base y cámara de medición. Se corrige la referencia de la guía de etiquetado en el glosario (era RNF18, es RNF16). |
| **RF03, RF04, RF05** | Cada toma incluye la lectura del MQ-135 y su línea base; el buffer y el mensaje la transportan. |
| **RF22 (nuevo)** | Lectura del MQ-135 por entrada analógica, enviando el valor crudo. Must. |
| **RF23 (nuevo)** | Precalentamiento, línea base en aire limpio y tiempo fijo de acumulación antes de cada sesión. Must. |
| **RF06** | El dataset incluye lecturas de color y de COV. |
| **RF10** | La serie de tiempo guarda los campos del MQ-135. |
| **RF11** | Se agregan el índice de COV y su velocidad de cambio por hora. |
| **RF12, RF14** | El clasificador usa color y COV según la comparación; se comparan los modelos en las configuraciones solo color y color + MQ-135, como pide OE3. |
| **RF15** | El LLM recibe el índice de COV como contexto. |
| **RF20, RF21** | El dashboard muestra el índice de COV y la serie de tiempo de color y COV. La justificación se muestra en texto legible (EDT 2.2.2). |
| **RF24–RF27 (nuevos)** | Historial: lista, descarga PDF/CSV, filtros por fecha y etapa, y acceso desde el dashboard (EDT paquete 1). |
| **RF28, RF29 (nuevos)** | ChatBot: interfaz y LLM que responde solo con datos almacenados (EDT paquete 3). |
| **RNF01** | El 95 % se mide sobre tomas con la lectura de ambos sensores. |
| **RNF08** | Se agrega el costo del MQ-135 (10.000 a 25.000 COP, por confirmar). |
| **RNF09** | Antes excluía los sensores de gas (MQ); ahora permite el MQ-135 y excluye SGP30/TVOC y etileno, como el acta §4.2. |
| **RNF18** | El registro de uso del LLM incluye las llamadas del ChatBot. |
| **RNF19 (nuevo)** | Pin ADC compatible con WiFi y divisor de voltaje para el MQ-135. |
| **RNF13, RNF16, RNF17** | Los supuestos incluyen los del MQ-135; el etiquetado se hace sin ver el COV; el README cubre precalentamiento y línea base. |
| **§5** | Se actualiza la trazabilidad por hito y se agrega la trazabilidad por paquete de la EDT. |

# 7. Valores propuestos por confirmar

* 5 tomas por sesión, separadas ≥ 2 s, en 3 posiciones, cada una con lectura de ambos sensores (RF03).

* Calibración del TCS34725 válida durante el mismo día (RF02).

* Buffer de 500 tomas (RF05).

* Al menos 1 sesión por día por banano (RF06).

* Modelos a comparar: regresión logística y bosque aleatorio (RF14).

* Unidad del valor estimado: COP por kg (RF16).

* Latencia menor a 30 s y prueba de usabilidad de 1 minuto (RNF02, RNF15).

* Promedio de varias muestras del ADC del MQ-135 por toma (RF22).

* Precalentamiento mínimo de 10 min y acumulación fija de 60 s en la cámara (RF23).

* Índice de COV = promedio del valor crudo ÷ línea base, y su velocidad de cambio por hora (RF11).

* El índice de COV se entrega al LLM solo como contexto y no altera las reglas (RF15).

* Etiquetadores sin acceso al índice de COV (RNF16).

* Número de preguntas de prueba del ChatBot, propuesto 10 (RF29).
