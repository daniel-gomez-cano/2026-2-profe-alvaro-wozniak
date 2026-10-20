**Requisitos funcionales y no funcionales (versión corregida)**

Sistema IoT de apoyo a la decisión para el destino del banano según su estado de maduración

Esta versión se alinea con el acta de constitución del 5 de octubre de 2026\. Todos los requisitos tienen criterio de verificación. Los valores marcados como "Propuesta" no están en el acta y deben ser confirmados por el equipo.

# **1\. Decisiones adoptadas**

| Tema | Decisión | Base en el acta |
| :---- | :---- | :---- |
| **Escala de madurez** | Se adopta la escala de 4 etapas (E1–E4). Se descarta la de 7 etapas del planteamiento. | Acta §4 (definiciones), §5 (el bodeguero valida la escala de 4 etapas), §7 (supuesto de 4 etapas). |
| **Clasificación vs. regresión** | Solo clasificación de etapa. Se elimina la regresión de días restantes. | Acta §4.2 excluye expresamente la regresión de días; la vida útil sale de la tabla de contexto. |
| **Días restantes** | Los días restantes se leen de la tabla de contexto según la etapa. No son una predicción del modelo. | Acta §4.2. |
| **Moneda y costo del sensor** | El costo se expresa en COP (20.000 a 80.000), no en USD. | Acta §8 (presupuesto). |
| **Umbral de kappa** | Kappa se calcula y se reporta, sin umbral de aprobación. | Acta §4.1 dice "kappa reportado", sin umbral. |
| **Prioridades** | RF05, RF09 y RF16 pasan a Must. Antes eran Should, Should y Could. | Acta §3 y §4.1 los exigen como parte del objetivo o de los entregables. |

# **2\. Glosario y prioridades**

| Término | Definición |
| :---- | :---- |
| **Toma** | Una medición RGB puntual del sensor TCS34725 (canales R, G, B y clear, valores crudos de 16 bits). |
| **Sesión** | Conjunto de tomas de un mismo banano en un mismo momento. Tiene un ID de sesión y una marca de tiempo. |
| **Etapa (E1–E4)** | Etapa de maduración según el color de la cáscara, en orden creciente (E1 la menos madura, E4 la más madura). La guía de etiquetado (RNF18) fija el criterio visual de cada etapa y cuál se marca como "deteriorado". |
| **Deteriorado** | Etiqueta operativa del protocolo de etiquetado (criterio visual). No se mide y no implica conclusión sobre inocuidad. |
| **Vida útil restante** | Días que la tabla de contexto asigna a cada etapa. No es salida del modelo. |
| **Etiquetador** | Persona que asigna una etapa a cada sesión sin ver la etiqueta de la otra persona. |
| **Prioridad** | MoSCoW: Must (indispensable para cumplir el acta), Should (importante, con alternativa temporal), Could (deseable, solo si hay tiempo). |

# **3\. Requisitos funcionales**

## **Nodo sensor (ESP32-S3 \+ TCS34725)**

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF01** | El nodo debe leer los valores RGB crudos (R, G, B y clear) del sensor TCS34725 en cada toma, dentro de un contenedor oscuro con LED propio encendido durante la lectura. | Prueba de banco: cada toma entrega 4 canales con valor mayor que 0 y el LED se enciende durante la lectura. |
| **RF02** | Antes de iniciar una sesión, el operador debe poder lanzar la calibración con referencia blanca/gris (pulsador o comando serial). El nodo debe guardar los valores de referencia junto con la sesión y rechazar sesiones si no hay una calibración del mismo día (Propuesta). | Demostración: sin calibración del día, el nodo rechaza iniciar sesión; con calibración, los valores de referencia aparecen en el registro de la sesión. |
| **RF03** | El nodo debe tomar 5 lecturas por banano en cada sesión, separadas al menos 2 s y en 3 posiciones distintas de la cáscara, todas con el mismo ID de sesión (valores propuestos, ajustables en la configuración). | Cada sesión registrada en el backend contiene 5 tomas con el mismo ID de sesión y marcas de tiempo separadas ≥ 2 s. |
| **RF04** | El nodo debe enviar cada toma al backend por WiFi con HTTPS POST en JSON, con los campos: ID de banano, ID de sesión, ID de toma, marca de tiempo UTC del nodo (sincronizada por NTP), R, G, B, clear y valores de referencia. El backend responde con código 2xx como confirmación. | Prueba de integración: una toma enviada aparece en el backend con todos los campos y el nodo recibe el 2xx. |
| **RF05** | Si no hay conexión o el backend no confirma, el nodo debe guardar las tomas en un buffer local de hasta 500 tomas (Propuesta), reenviarlas en orden cronológico al reconectar y borrar cada una solo tras recibir confirmación. El backend debe descartar duplicados por ID de toma. Si el buffer se llena, se descarta la toma más antigua y se incrementa un contador de pérdidas. | Prueba: desconectar el WiFi 10 min durante una sesión; al reconectar llegan todas las tomas, en orden y sin duplicados. |

## **Dataset y etiquetado**

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF06** | El sistema debe registrar y almacenar las tomas de 25 bananos, cada uno con ID único (B01 a B25), con al menos 1 sesión por día (Propuesta) desde un estado inicial común o conocido hasta alcanzar la etapa final o el cierre de la recolección. | La base de datos contiene 25 IDs únicos, cada uno con al menos 1 sesión por día durante su seguimiento. |
| **RF07** | Dos etiquetadores independientes deben asignar una etapa E1–E4 a cada sesión, cada uno en su propio dataset y sin ver la etiqueta del otro. La etiqueta es por sesión, no por toma, y se registra en la herramienta del proyecto (hoja o formulario definido por el equipo). | Dos datasets separados, ambos con etiqueta para el 100 % de las sesiones; cada registro guarda el etiquetador y la fecha. |
| **RF08** | El sistema puede permitir registrar datos complementarios opcionales por sesión: °Brix (número ≥ 0\) y días desde la compra (entero ≥ 0). Este requisito no está en el acta; se implementa solo si sobra tiempo. | Si se implementa: los campos aceptan solo valores válidos y se guardan asociados a la sesión. |
| **RF09** | El sistema debe calcular la concordancia entre los dos etiquetadores: porcentaje de sesiones con diferencia ≤ 1 etapa y kappa ponderado cuadrático (escala ordinal). Debe incluir el resultado en el informe de métricas. | Existe un informe con el % ±1 y el kappa ponderado, calculados con un script del repositorio. |

## **Backend y preprocesamiento**

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF10** | El backend debe guardar cada toma como serie de tiempo, con ID de banano, ID de sesión, ID de toma, marca de tiempo, valores RGB/clear y valores de referencia, y permitir consultarlas por banano y rango de fechas. | Consulta por banano y rango de fechas devuelve las tomas en orden cronológico. |
| **RF11** | Por cada sesión completa, el backend debe calcular: (a) el promedio de las tomas por canal; (b) las variables HSV a partir de ese promedio; (c) la velocidad de cambio de color como variación de H por hora entre sesiones consecutivas del mismo banano; (d) las horas acumuladas desde la primera sesión de ese banano. | Prueba unitaria con datos de ejemplo: los valores calculados coinciden con un cálculo manual. |

## **IA determinística**

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF12** | El sistema debe clasificar cada sesión en una etapa E1–E4 a partir de las variables de RF11 y guardar la etapa estimada. La inferencia se ejecuta en el backend al completarse la sesión. No se estiman días restantes. | Una sesión nueva recibe etapa estimada en el backend sin intervención manual. |
| **RF13** | El entrenamiento y la validación deben dividir los datos por banano (validación cruzada agrupada por ID, por ejemplo dejando un banano fuera), nunca al azar entre tomas o sesiones. | Revisión del código: ningún ID de banano aparece a la vez en entrenamiento y validación. |
| **RF14** | El sistema debe comparar al menos 2 modelos (por ejemplo regresión logística y bosque aleatorio, Propuesta) y reportar para cada uno: matriz de confusión, porcentaje de coincidencia exacta y porcentaje dentro de ±1 etapa en bananos no vistos. | El informe de métricas muestra los 2 modelos con las tres métricas. |

## **LLM y decisión**

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF15** | El LLM debe recibir la etapa estimada, la vida útil restante y la tabla de contexto, y recomendar un destino de un catálogo cerrado: vender\_local, enviar\_lejos, procesar, descartar. | En los escenarios de prueba, la acción devuelta pertenece siempre al catálogo. |
| **RF16** | El LLM debe devolver un valor de mercado estimado en COP por kg (Propuesta), calculado con los precios de referencia de la tabla de contexto para la etapa y el destino. La capa de reglas debe verificar que el valor esté entre 0 y el precio máximo de la tabla para esa etapa. | En los escenarios de prueba, el 100 % de los valores está dentro del rango de la tabla. |
| **RF17** | La respuesta del LLM debe ser un JSON con esquema fijo y estos campos: accion (enum del catálogo), justificacion (texto de hasta 300 caracteres), confianza (número de 0 a 1\) y valor\_estimado\_cop (número ≥ 0). Si la respuesta no cumple el esquema, el sistema reintenta una vez; si vuelve a fallar, no muestra destino y registra el error. | Validación contra el esquema JSON: las respuestas inválidas se rechazan y el dashboard muestra un mensaje de error. |
| **RF18** | Una capa de reglas debe validar la decisión del LLM y bloquear las absurdas, como mínimo: (a) enviar\_lejos con vida útil restante menor que el tiempo de transporte; (b) un destino cuya calidad mínima no cumple la etapa; (c) una acción fuera del catálogo. Al bloquear, el sistema debe sustituirla por la acción que la tabla de contexto define para esa etapa, marcarla como "corregida por reglas" y guardar la decisión original del LLM. | Casos de prueba para las reglas (a), (b) y (c): cada uno se bloquea y se sustituye por la acción de la tabla. |
| **RF19** | El sistema debe mantener una tabla de contexto versionada en el repositorio (CSV o JSON) con, por etapa y destino: calidad mínima, tiempo de transporte, vida útil en días y precio de referencia, cada fila con su fuente o la marca "supuesto". Debe ser validada por el cliente. | El archivo existe en el repositorio, no tiene celdas vacías y hay constancia de validación del cliente. |

## **Visualización**

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RF20** | El dashboard debe permitir seleccionar un banano y mostrar: la última lectura (R, G, B), la etapa estimada, la vida útil restante (de la tabla), el destino recomendado, la justificación, el valor estimado y un indicador si la decisión fue corregida por reglas. Debe actualizarse sin recargar la página. | Demostración: al llegar una sesión nueva, el dashboard muestra todos los campos sin recargar, dentro del límite de RNF02. |
| **RF21** | El dashboard debe mostrar el historial por banano: gráfico de color (H) y de etapa en el tiempo, con tabla de sesiones y selector de rango de fechas. | Para un banano con al menos 5 sesiones, el gráfico y la tabla muestran todas las sesiones del rango seleccionado. |

# **4\. Requisitos no funcionales**

## **Rendimiento y fiabilidad**

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RNF01** | El nodo debe transmitir de forma continua durante 7 días o más, y el backend debe recibir al menos el 95 % de las tomas programadas (recibidas ÷ programadas). | Prueba de 7 días: el conteo de tomas recibidas frente a las programadas es ≥ 95 %. |
| **RNF02** | El tiempo desde que se completa una sesión hasta que el dashboard muestra etapa y recomendación debe ser menor a 30 s en el 95 % de las sesiones (Propuesta, no está en el acta). | Medición de tiempos en 20 sesiones: al menos 19 cumplen. |

## **Exactitud y calidad del modelo**

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RNF03** | El clasificador debe tener al menos 85 % de coincidencia dentro de ±1 etapa en bananos no vistos. Además se reporta la coincidencia exacta, porque con 4 etapas un acierto al azar ya cae dentro de ±1 el 62,5 % de las veces. | Informe de métricas con validación por banano (RF13): ≥ 85 % dentro de ±1. |
| **RNF04** | La coincidencia entre etiquetadores debe ser ≥ 90 % dentro de ±1 etapa. El kappa ponderado se reporta sin umbral. | Resultado de RF09 sobre el dataset completo. |
| **RNF05** | Con la misma entrada repetida 20 veces, el LLM debe devolver la misma acción en al menos 18 de 20 (90 %), con la temperatura configurada en su valor más bajo. | Script de repetición que registra las 20 respuestas y calcula el porcentaje. |
| **RNF06** | El 100 % de las respuestas del LLM en las pruebas (RNF05 y RNF07) debe ser JSON válido según el esquema de RF17. | Validador de esquema sobre todas las respuestas registradas. |
| **RNF07** | En 12 escenarios de prueba definidos por el equipo, la acción final debe coincidir con la acción esperada según la tabla de reglas en al menos 80 % (10 de 12). | Tabla de 12 escenarios con acción esperada y acción obtenida. |

## **Costo y hardware**

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RNF08** | El sensor RGB debe costar entre 20.000 y 80.000 COP, reutilizando la ESP32-S3 disponible. | Factura o cotización del sensor dentro del rango. |
| **RNF09** | El prototipo debe usar un solo nodo y un solo sensor, sin cámara ni sensores de gas (MQ, SGP30/TVOC) ni de etileno. | Revisión del montaje y de la lista de materiales. |
| **RNF10** | El entrenamiento y la inferencia del modelo deben ejecutarse fuera de la ESP32-S3. | Revisión de la arquitectura: el firmware solo mide y envía. |
| **RNF18** | El sistema debe registrar cada llamada al LLM (fecha, tokens usados) para controlar costo y cuota de la API. | Existe un registro de llamadas y el total no supera la cuota ni el presupuesto. |

## **Mantenibilidad y reproducibilidad**

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RNF11** | El entrenamiento debe ser reproducible: semilla fija, versiones de dependencias fijadas y mismos datos dan las mismas métricas. | Dos ejecuciones del entrenamiento producen métricas idénticas. |
| **RNF12** | La configuración (credenciales WiFi, API key del LLM, umbrales) debe estar fuera del código fuente, en un archivo no versionado, con un archivo de ejemplo en el repositorio. | Búsqueda en el repositorio: no hay claves ni contraseñas; existe el archivo de ejemplo. |
| **RNF13** | Los supuestos (tabla de contexto, definición de "deteriorado", valores propuestos) deben estar documentados y marcados como tales. | Revisión de la documentación: cada supuesto aparece con la marca "supuesto". |
| **RNF17** | Una persona ajena al equipo debe poder reproducir la instalación y una lectura de punta a punta usando solo el README. | Prueba con una persona externa que no pide ayuda al equipo. |

## **Seguridad**

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RNF14** | La comunicación con el backend y con la API del LLM debe ir cifrada (HTTPS), y el nodo debe autenticarse con un token propio. (Propuesta, no está en el acta.) | Revisión: no hay endpoints HTTP sin cifrar y una petición sin token es rechazada. |

## **Usabilidad**

| ID | Requisito | Criterio de verificación |
| :---- | :---- | :---- |
| **RNF15** | El dashboard debe ser entendible sin conocimientos técnicos y funcionar en el navegador de un PC. Un usuario nuevo debe identificar el destino recomendado y su justificación en menos de 1 minuto, sin ayuda (el minuto es una Propuesta). | Prueba con el cliente: identifica destino y justificación en menos de 1 minuto. |
| **RNF16** | El 100 % de las sesiones debe ser etiquetable con la guía de etiquetado escrita antes de etiquetar, con criterios y fotos de ejemplo para E1–E4. La guía debe ser entendible para etiquetadores sin experiencia y la escala de 4 etapas debe ser revisada por el bodeguero o acopiador. | Los dos etiquetadores terminan sin consultar al equipo y el bodeguero confirma la escala por escrito. |

# **5\. Trazabilidad por hito**

| Hito del acta | Requisitos que lo cubren |
| :---- | :---- |
| **H2. Nodo sensor montado (21 oct)** | RF01–RF05, RF10, RNF01, RNF08–RNF10 |
| **H3. Dataset con doble etiquetado (18 nov)** | RF06–RF09, RF11, RNF04, RNF16 |
| **H4. Clasificador, LLM, tabla y reglas (2 dic)** | RF11–RF19, RNF03, RNF05–RNF07, RNF11 |
| **H5. Integración, pruebas y entrega (16 dic)** | RF20, RF21, RNF02, RNF03, RNF15, RNF17 |

# **6\. Cambios respecto a la versión anterior**

| ID | Cambio |
| :---- | :---- |
| **RF03, RF05** | Se definen número de tomas, intervalo, posiciones y tamaño del buffer. RF05 sube a Must por el 95 % exigido en el acta. |
| **RF04** | Se fijan protocolo, formato del mensaje y confirmación. Se unifica la marca de tiempo (la del nodo, con NTP). |
| **RF09, RNF04** | Kappa ponderado cuadrático y sin umbral. RF09 sube a Must. |
| **RF11** | Se definen las variables derivadas (HSV, velocidad de cambio, horas acumuladas). |
| **RF12** | Se elimina la regresión de días y el "y/o"; solo clasificación E1–E4. |
| **RF13, RF14** | Se precisan el esquema de partición y las métricas. Se elimina el error medio en días. |
| **RF15–RF17** | Se separan responsabilidades: RF15 decisión, RF16 valor, RF17 contrato JSON. RF16 sube a Must por el objetivo del acta. Se fija el catálogo de acciones. |
| **RF18** | Se listan las reglas mínimas y el comportamiento al bloquear. |
| **RF19, RF20, RF21** | Se define el contenido de la tabla, la actualización del dashboard y el contenido del historial. |
| **RNF03** | Se añade la coincidencia exacta, porque ±1 con 4 etapas es poco exigente. |
| **RNF05, RNF07** | Se fijan valores exactos (18 de 20, 12 escenarios en lugar de 10 a 15). |
| **RNF08** | El costo pasa de USD a COP, como en el acta. |
| **RNF17, RNF18** | Nuevos: README reproducible y registro de uso del LLM, derivados de los criterios de éxito del acta. |

# **7\. Valores propuestos por confirmar**

* 5 tomas por sesión, separadas ≥ 2 s, en 3 posiciones (RF03).

* Calibración válida durante el mismo día (RF02).

* Buffer de 500 tomas (RF05).

* Al menos 1 sesión por día por banano (RF06).

* Modelos a comparar: regresión logística y bosque aleatorio (RF14).

* Unidad del valor estimado: COP por kg (RF16).

* Latencia menor a 30 s y prueba de usabilidad de 1 minuto (RNF02, RNF15).