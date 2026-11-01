# Plan de cronograma y presupuesto del proyecto Bananin

**Versión de planificación propuesta:** 26 de octubre de 2026\
**Moneda:** pesos colombianos (COP)

## 1. Criterios de planificación

La planificación se basa en el acta de constitución, los requisitos
funcionales y no funcionales, la EDT, los criterios de aceptación y la
matriz RACI.

| Elemento | Base de planificación |
|---|---|
| Fecha límite | 16 de diciembre de 2026 |
| Equipo | 5 estudiantes, 8 horas por semana por persona |
| Capacidad máxima prevista | 40 horas-persona por semana |
| Muestra | 25 bananos, con seguimiento de maduración y doble etiquetado independiente |
| Hardware contemplado en el acta | ESP32-S3 disponible, TCS34725 y MQ-135 |
| Desarrollo de IA | Al menos dos modelos de clasificación, validación por banano y comparación de las señales disponibles |
| Decisión comercial | LLM, tabla de contexto validada por el cliente y capa de reglas |
| Entrega | Sistema integrado, dashboard, historial, chatbot, pruebas y documentación reproducible |

### Decisión de alcance pendiente

Existe una discrepancia documental: el acta y la EDT incluyen el MQ-135,
mientras que la versión corregida de los requisitos lo excluye. Para
esta propuesta presupuestal se conserva el alcance del acta como base de
planificación, pero la compra o exclusión definitiva debe aprobarse
formalmente por el patrocinador y el equipo.

Los costos que no están definidos en los documentos se marcan como **por
cotizar** o **por verificar**; no se presentan como precios confirmados.

## 2. Cronograma detallado

El cronograma conserva los cinco hitos originales. Las actividades
intermedias son una propuesta de planificación de recuperación, no
fechas que necesariamente estén aprobadas en el acta. A fecha del 26 de
octubre de 2026, los hitos del 9 y 21 de octubre ya han pasado; su
cumplimiento debe verificarse con evidencia.

## 2.1. Fases del proyecto

| Fase | Periodo propuesto | Actividades principales | Resultado esperado |
|---|---|---|---|
| **1. Regularización del alcance y hardware** | 26 de octubre – 1 de noviembre | Resolver la discrepancia del MQ-135, revisar el montaje, completar la calibración, validar la transmisión y ejecutar una prueba continua de siete días. | Nodo funcional y transmisión verificable. |
| **2. Recolección y etiquetado** | 26 de octubre – 18 de noviembre | Seguir los 25 bananos, registrar sesiones, limpiar datos, documentar el protocolo visual y realizar el doble etiquetado independiente. | Dataset limpio, trazable y etiquetado por dos personas. |
| **3. Clasificación y decisión** | 19 de noviembre – 2 de diciembre | Preprocesar datos, entrenar y comparar modelos, calcular métricas, construir la tabla de contexto y validar el LLM y las reglas. | Clasificador evaluado y módulo de recomendación comprobado. |
| **4. Integración de interfaces** | 3 – 9 de diciembre | Integrar dashboard, actualización de lecturas, historial, filtros, descarga y chatbot. | Módulos conectados al backend y a datos reales. |
| **5. Pruebas, documentación y entrega** | 10 – 16 de diciembre | Ejecutar pruebas de aceptación, corregir fallos, finalizar el README y preparar la demostración de punta a punta. | Entrega evaluable y reproducible. |

## 2.2. Cronograma operativo por actividad

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

## 2.3. Hitos oficiales

| Hito | Fecha establecida | Evidencia esperada |
|---|---|---|
| **H1. Acta y arranque** | 9 de octubre de 2026 | Acta firmada y reunión de inicio. |
| **H2. Nodo sensor** | 21 de octubre de 2026 | Montaje, calibración y transmisión de datos. |
| **H3. Dataset** | 18 de noviembre de 2026 | Datos de 25 bananos, limpieza y doble etiquetado. |
| **H4. Modelos y decisión** | 2 de diciembre de 2026 | Clasificador validado, LLM, tabla de contexto y reglas. |
| **H5. Integración y entrega** | 16 de diciembre de 2026 | Demostración completa, pruebas, documentación y entrega reproducible. |

El cronograma no supone que H1 y H2 ya se hayan cumplido: su estado debe
verificarse.

### 2.4. Distribución propuesta del esfuerzo

La ventana del 26 de octubre al 16 de diciembre equivale aproximadamente
a 7,4 semanas. Con una disponibilidad prevista de 40 horas-persona
semanales, la capacidad máxima teórica es de alrededor de **296
horas-persona**. Debe ajustarse a la disponibilidad real, incluyendo
semanas parciales y compromisos académicos.

| Paquete de trabajo | Horas-persona propuestas | Porcentaje aproximado |
|---|---:|---:|
| Hardware, firmware y conectividad | 40 | 13,5 % |
| Dataset, etiquetado y análisis de concordancia | 56 | 18,9 % |
| Preprocesamiento y clasificación ML | 48 | 16,2 % |
| Backend, LLM, tabla de contexto y reglas | 48 | 16,2 % |
| Dashboard e historial | 52 | 17,6 % |
| Chatbot | 20 | 6,8 % |
| Integración, pruebas y documentación | 32 | 10,8 % |
| **Total** | **296** | **100 %** |

Esta distribución es una estimación de planificación, no un registro de
horas trabajadas. Debe revisarse con los responsables de la matriz RACI.
Algunas tareas pueden ejecutarse en paralelo, pero la integración final
depende de la disponibilidad del hardware, backend y dataset.

## 3. Presupuesto del proyecto

El presupuesto distingue entre costos respaldados por el acta, recursos
ya disponibles y partidas pendientes de cotización.

### 3.1. Hardware y montaje

| Ítem | Cantidad | Costo unitario | Subtotal | Estado |
|---|---:|---:|---:|---|
| Sensor RGB TCS34725 | 1 | $20.000–$80.000 | $20.000–$80.000 | Rango indicado en el acta. |
| Sensor MQ-135 | 1 | $10.000–$25.000 | $10.000–$25.000 | Estimación del acta, pendiente de proveedor. |
| ESP32-S3 | 1 | $0 incremental | $0 | No se cuanto. |
| Contenedor oscuro y soporte de medición | 1 | Por cotizar | Por cotizar | Debe permitir medir color y, si se conserva el MQ-135, alojar su cámara ventilable. |
| LED propio y elementos de montaje | 1 conjunto | Por cotizar | Por cotizar | Requeridos para las lecturas RGB. |
| Cableado, divisor de voltaje y conexiones | 1 conjunto | Por cotizar | Por cotizar | Adaptar la entrada analógica del MQ-135 a 3,3 V si se mantiene. |
| Referencia blanca/gris para calibración | 1 conjunto | Por cotizar | Por cotizar | Para calibración del TCS34725. |
| Alimentación y elementos eléctricos adicionales | 1 conjunto | Por verificar | Por verificar | Revisar primero los recursos existentes. |

El subtotal de los dos sensores, tomando los rangos indicados en el
acta, es de **\$30.000 a \$105.000 COP**. Este subtotal no incluye
montaje, consumibles ni servicios.

### 3.2. Recolección de datos y operación

| Ítem | Base de cálculo | Valor presupuestal |
|---|---|---|
| Bananos para el experimento | 25 unidades, más reposición si es necesaria. | Por cotizar. |
| Reposición de bananos | Según pérdidas o ejemplares que no sirvan para el seguimiento. | Reserva por definir. |
| Referencias para etiquetado | Guía escrita y fotografías de ejemplo. | Trabajo del equipo; costo adicional no definido. |
| Backend y almacenamiento | Infraestructura existente o servicio por contratar. | Verificar costos de nube. |
| API del LLM | Consumo de tokens durante desarrollo y pruebas. | Por cotizar |
| Conectividad Wi-Fi | Red disponible. | $0 |



### 3.3. Software y desarrollo

| Concepto | Tratamiento presupuestal |
|---|---|
| Desarrollo del backend | Horas del equipo; costo monetario por definir si se valora la mano de obra. |
| Entrenamiento y validación ML | Horas del equipo; infraestructura por verificar. |
| Integración del LLM y reglas | Horas del equipo más consumo de API. |
| Dashboard e historial | Horas del equipo. |
| Chatbot | Horas del equipo más consumo de API. |
| Pruebas y documentación | Horas del equipo. |
| Herramientas de desarrollo | Verificar disponibilidad de herramientas gratuitas o licencias existentes. |


## 4. Resumen presupuestal y reserva

| Concepto | Mínimo (COP) | Máximo (COP) |
|---|---:|---:|
| Sensor TCS34725 | $20.000 | $80.000 |
| Sensor MQ-135 | $10.000 | $25.000 |
| ESP32-S3 disponible | $0 incremental | $0 incremental |
| **Subtotal de sensores** | **$30.000** | **$105.000** |
| Reserva provisional del 10 % sobre sensores | $3.000 | $10.500 |
| **Subtotal de sensores con reserva** | **$33.000** | **$115.500** |

**\$33.000--\$115.500 COP no es el presupuesto total
definitivo**. Es una referencia inicial de sensores con reserva
provisional. Para completar el presupuesto hacen falta cotizaciones del
montaje, consumibles, bananos, servicios y transporte.

### 4.1. Fórmula de cierre del presupuesto

Para cada partida pendiente se debe registrar:

1.  Cantidad necesaria y unidad de compra.
2.  Cotización o precio de referencia y fecha de consulta.
3.  Costo unitario y subtotal.
4.  Responsable de la compra.
5.  Confirmación de si el recurso ya existe o debe adquirirse.
6.  Evidencia de compra o disponibilidad.

La estructura de cálculo es:

**Costo directo = hardware + montaje + consumibles + servicios +
transporte y otros gastos**

**Presupuesto total = costo directo + reserva aprobada**

Si el curso requiere contabilizar la mano de obra, debe presentarse por
separado el costo económico de las horas-persona. Ese costo no debe
confundirse con el dinero que realmente debe desembolsar el equipo.

## 5. Riesgos que afectan cronograma y presupuesto

| Riesgo | Efecto sobre el plan | Respuesta propuesta |
|---|---|---|
| Dataset pequeño o poco diverso | Puede impedir alcanzar las métricas objetivo. | Seguimiento riguroso, doble etiquetado y validación por banano. |
| Retraso en la compra de sensores | Puede afectar el periodo de medición. | Verificar disponibilidad inmediata y buscar un proveedor alternativo. |
| Maduración más rápida de lo esperado | Puede reducir la oportunidad de recoger datos útiles. | Escalonar la compra y revisar diariamente la cobertura de etapas. |
| Ruido o deriva del MQ-135 | Puede reducir la utilidad de las mediciones de COV. | Precalentamiento, línea base en aire limpio y registro de condiciones. |
| Inconsistencia del LLM | Puede producir recomendaciones cambiantes o absurdas. | Esquema JSON fijo, reglas de validación y prueba de 20 repeticiones. |
| Falta de presupuesto para la API | Puede interrumpir las pruebas. | Definir cuota, límite de gasto y registro de tokens antes de las pruebas intensivas. |

Los riesgos técnicos se basan en el acta; las respuestas adicionales de
gestión son propuestas para esta planificación.

## 6. Lista de decisiones pendientes

-   Confirmar con el patrocinador si el MQ-135 se mantiene en el
    alcance aprobado.
-   Verificar qué hardware, cables, contenedor y herramientas ya
    tiene el equipo.
-   Obtener cotizaciones de materiales pendientes y definir el costo
    de los 25 bananos.
-   Definir proveedor, modelo, cuota y presupuesto máximo de la API
    del LLM.
-   Asignar nombres a responsables de hardware, datos/ML,
    backend/LLM y frontend en la matriz RACI.

