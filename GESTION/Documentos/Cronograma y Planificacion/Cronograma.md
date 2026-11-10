# Plan de cronograma y presupuesto del proyecto Bananin
## Cronograma detallado

### Fases del proyecto

| Fase | Periodo propuesto | Actividades principales | Resultado esperado |
|---|---|---|---|
| **1. Regularización del alcance y hardware** | 26 de octubre – 1 de noviembre | Resolver la discrepancia del MQ-135, revisar el montaje, completar la calibración, validar la transmisión y ejecutar una prueba continua de siete días. | Nodo funcional y transmisión verificable. |
| **2. Recolección y etiquetado** | 26 de octubre – 18 de noviembre | Seguir los 25 bananos, registrar sesiones, limpiar datos, documentar el protocolo visual y realizar el doble etiquetado independiente. | Dataset limpio, trazable y etiquetado por dos personas. |
| **3. Clasificación y decisión** | 19 de noviembre – 2 de diciembre | Preprocesar datos, entrenar y comparar modelos, calcular métricas, construir la tabla de contexto y validar el LLM y las reglas. | Clasificador evaluado y módulo de recomendación comprobado. |
| **4. Integración de interfaces** | 3 – 9 de diciembre | Integrar dashboard, actualización de lecturas, historial, filtros, descarga y chatbot. | Módulos conectados al backend y a datos reales. |
| **5. Pruebas, documentación y entrega** | 10 – 16 de diciembre | Ejecutar pruebas de aceptación, corregir fallos, finalizar el README y preparar la demostración de punta a punta. | Entrega evaluable y reproducible. |

### Cronograma operativo por actividad

| ID | Actividad | Inicio | Fin | Dependencia / resultado esperado |
|---|---|---|---|---|
| A01 | Revisar el alcance y decidir formalmente sobre el MQ-135. | 26 oct. | 27 oct. | Acuerdo del patrocinador y del equipo. |
| A02 | Verificar compras, conexiones, calibración y firmware. | 26 oct. | 29 oct. | Nodo listo para pruebas. |
| A03 | Validar el envío al backend, el registro de sesiones y el manejo de desconexiones. | 28 oct. | 1 nov. | Lecturas completas y sin duplicados. |
| A04 | Ejecutar una prueba de transmisión continua durante siete días. | 26 oct. | 1 nov. | Objetivo propuesto: al menos 95 % de lecturas recibidas. |
| A05 | Preparar el protocolo y la guía visual de etiquetado E1–E4. | 26 oct. | 29 oct. | Guía revisada antes del etiquetado. |
| A06 | Adquirir o disponer de los 25 bananos e iniciar el seguimiento. | 26 oct. | 30 oct. | Identificadores únicos y registros iniciales. |
| A07 | Continuar las sesiones, revisar los datos y completar el seguimiento. | 30 oct. | 18 nov. | Dataset de 25 bananos con doble etiqueta. |
| A08 | Calcular el acuerdo entre etiquetadores y el kappa ponderado. | 16 nov. | 18 nov. | Informe de concordancia. |
| A09 | Limpiar datos, construir variables y preparar particiones por banano. | 19 nov. | 22 nov. | Datos preparados sin fuga entre entrenamiento y validación. |
| A10 | Entrenar y comparar dos modelos candidatos. | 23 nov. | 27 nov. | Matrices de confusión y métricas. |
| A11 | Construir y validar la tabla de contexto, el LLM y las reglas. | 19 nov. | 30 nov. | Destinos, precios de referencia y reglas documentados. |
| A12 | Integrar la evaluación del clasificador y la decisión comercial. | 28 nov. | 2 dic. | Hito H4. |
| A13 | Construir el dashboard y la vista del último dato. | 3 dic. | 6 dic. | Datos y recomendación visibles. |
| A14 | Implementar historial, filtros y descarga PDF/CSV. | 3 dic. | 8 dic. | Paquetes EDT de historial implementados. |
| A15 | Integrar el chatbot y las consultas sobre datos almacenados. | 5 dic. | 9 dic. | Respuestas basadas en datos existentes. |
| A16 | Ejecutar pruebas de integración y aceptación. | 10 dic. | 12 dic. | Criterios de la EDT verificados. |
| A17 | Finalizar README, configuración, evidencias y demostración. | 10 dic. | 15 dic. | Instalación reproducible y documentación completa. |
| A18 | Realizar la demostración de punta a punta y la entrega. | 16 dic. | 16 dic. | Hito H5. |

**Advertencia sobre la recolección:** las fechas deben ajustarse al estado real de la muestra. La maduración no debe acelerarse artificialmente si eso altera el experimento. Si los 25 ejemplares no alcanzan las etapas necesarias a tiempo, se debe documentar el riesgo y acordar una respuesta; no se debe simular que se completó el seguimiento.

### Hitos oficiales

| Hito | Fecha establecida | Evidencia esperada |
|---|---|---|
| **H1. Acta y arranque** | 9 de octubre de 2026 | Acta firmada y reunión de inicio. |
| **H2. Nodo sensor** | 21 de octubre de 2026 | Montaje, calibración y transmisión de datos. |
| **H3. Dataset** | 18 de noviembre de 2026 | Datos de 25 bananos, limpieza y doble etiquetado. |
| **H4. Modelos y decisión** | 2 de diciembre de 2026 | Clasificador validado, LLM, tabla de contexto y reglas. |
| **H5. Integración y entrega** | 16 de diciembre de 2026 | Demostración completa, pruebas, documentación y entrega reproducible. |

El cronograma no supone que H1 y H2 ya se hayan cumplido: su estado debe verificarse.

### Distribución propuesta del esfuerzo

La ventana del 26 de octubre al 16 de diciembre equivale aproximadamente a 7,4 semanas. Con una disponibilidad prevista de 40 horas-persona semanales, la capacidad máxima teórica es de alrededor de **296 horas-persona**. Debe ajustarse a la disponibilidad real, incluyendo semanas parciales y compromisos académicos.

Perfecto. Cambiamos Hardware, firmware y conectividad de 32 a 10 horas-persona.

Al hacerlo, el total pasa de 129 a 107 horas-persona. Los porcentajes quedan recalculados así:

### Distribución propuesta del esfuerzo

Perfecto. Conservamos las horas que acabas de indicar. El total correcto es 215 horas-persona, no 296. También recalculé los porcentajes para que correspondan a cada categoría.

### Distribución propuesta del esfuerzo

| Paquete de trabajo                             | Horas propuestas | Porcentaje aproximado |
| ---------------------------------------------- | ------------------------ | --------------------- |
| Hardware, firmware y conectividad              | 40                       | 18,6 %                |
| Dataset, etiquetado y análisis de concordancia | 40                       | 18,6 %                |
| Preprocesamiento y clasificación ML            | 40                       | 18,6 %                |
| Backend, LLM, tabla de contexto y reglas       | 30                       | 14,0 %                |
| Dashboard e historial                          | 25                       | 11,6 %                |
| Chatbot                                        | 20                       | 9,3 %                 |
| Integración, pruebas y documentación           | 20                       | 9,3 %                 |
| Total                                          | 215                      | 100 %                 |


Esta distribución es una estimación de planificación, no un registro de horas trabajadas. La capacidad máxima teórica de 215 hora representa la disponibilidad prevista de cada integrante.

