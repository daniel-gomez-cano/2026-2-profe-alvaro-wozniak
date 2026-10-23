## Estructura de desglose del trabajo (EDT) y criterios de aceptación

*Los pesos son porcentajes del proyecto total. Cada paquete se acepta solo si cumple su criterio.*

| Código | Paquete de trabajo | Peso | Criterio de aceptación |
| --- | --- | :-: | --- |
| **0** | **Bananin** | 100 % | Demo en vivo completa de punta a punta (sensores → nube → modelo → LLM → dashboard) antes del 16 dic 2026; el cliente confirma que la recomendación le sirve para decidir el destino de un lote. |
| **1** | **Historial** | 30 % | |
| 1.1 | Lista de datos | 10 % | Muestra las lecturas almacenadas con fecha y hora, valores de color y COV, etapa y destino, con la más reciente primero. |
| 1.2 | Descargar informe | 10 % | Genera un archivo descargable (PDF o CSV) con los datos del historial que se está viendo, respetando el filtro activo. |
| 1.3 | Filtrar datos | 5 % | Filtra por rango de fechas y por etapa; la lista muestra solo los registros que cumplen el filtro. |
| 1.4 | Interfaz | 5 % | Pantalla del historial accesible desde el dashboard, con lista, filtros y descarga funcionando. |
| **2** | **Dashboard** | 60 % | |
| **2.1** | **Información del último dato** | 40 % | |
| 2.1.1 | Sensor | 25 % | Nodo ESP32-S3 + TCS34725 + MQ-135 calibrado (TCS34725 con blanco/gris; MQ-135 con precalentamiento y línea base en aire limpio), transmitiendo ≥ 7 días continuos con ≥ 95 % de lecturas recibidas de ambos sensores. |
| 2.1.2 | Modelo determinista | 10 % | Al menos 2 modelos de etapa (E1–E4) validados por banano, con matriz de confusión, comparación solo color vs. color + MQ-135 y ≥ 85 % de coincidencia (±1 etapa) en bananos no vistos. |
| 2.1.3 | Interfaz con los valores | 5 % | Muestra el último dato (color, índice de COV y etapa estimada) y se actualiza cuando llega una lectura nueva. |
| **2.2** | **Justificación del último dato** | 20 % | |
| 2.2.1 | Modelo no determinista | 15 % | LLM con salida en JSON válido de esquema fijo (destino y valor de mercado estimado), tabla de contexto validada por el cliente y capa de reglas que bloquea decisiones absurdas; misma acción en ≥ 90 % de 20 repeticiones. |
| 2.2.2 | Interfaz con texto | 5 % | Muestra el destino recomendado, el valor estimado y la justificación en texto legible, correspondientes al último dato. |
| **3** | **ChatBot** | 10 % | |
| 3.1 | Interfaz | 5 % | Chat con entrada de texto y respuestas visibles, accesible desde el dashboard. |
| 3.2 | LLM | 5 % | Responde con base en los datos almacenados en un conjunto de preguntas de prueba definido por el equipo, sin inventar valores que no estén en el sistema. |

## Matriz RACI

**R** = Responsable (ejecuta) · **A** = Aprobador (rinde cuentas, uno solo por fila) · **C** = Consultado · **I** = Informado. **A/R** = ejecuta y rinde cuentas.

Un mismo integrante puede cubrir más de un rol; el rol de frontend se agregó porque el EDT tiene varias interfaces.

| Persona | Patrocinador | Gerente | Resp. hardware | Resp. datos/ML | Resp. backend/LLM | Resp. frontend | Cliente | Docente |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **Nombre** | Edward S. Pinto G. | Daniel Gómez C. | *(asignar al revisar)* | *(asignar al revisar)* | *(asignar al revisar)* | *(asignar al revisar)* | Isabella Agudelo L. | Álvaro Salazar V. |

| Código | Paquete de trabajo | Patrocinador | Gerente | Hardware | Datos/ML | Backend/LLM | Frontend | Cliente | Docente |
| --- | --- | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| 0 | Bananin (integración, demo y entrega) | C | A/R | R | R | R | R | C | I |
| 1.1 | Lista de datos | I | I | I | I | A/R | R | I | I |
| 1.2 | Descargar informe | I | I | I | I | A/R | R | C | I |
| 1.3 | Filtrar datos | I | I | I | I | A/R | R | I | I |
| 1.4 | Interfaz (historial) | I | I | I | I | C | A/R | C | I |
| 2.1.1 | Sensor | I | I | A/R | I | C | I | I | I |
| 2.1.2 | Modelo determinista | C | I | C | A/R | C | I | I | I |
| 2.1.3 | Interfaz con los valores | I | I | I | I | C | A/R | C | I |
| 2.2.1 | Modelo no determinista | I | I | I | C | A/R | I | C | I |
| 2.2.2 | Interfaz con texto | I | I | I | I | C | A/R | C | I |
| 3.1 | Interfaz (ChatBot) | I | I | I | I | C | A/R | C | I |
| 3.2 | LLM (ChatBot) | I | I | I | C | A/R | I | I | I |
