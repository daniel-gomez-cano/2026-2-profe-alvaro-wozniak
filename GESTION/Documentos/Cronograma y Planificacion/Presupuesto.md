# Plan de cronograma y presupuesto del proyecto Bananin
## Presupuesto del proyecto

El presupuesto distingue entre costos respaldados por el acta, recursos
ya disponibles y partidas pendientes de cotización.

### Hardware y montaje

| Ítem                                    | Cantidad   | Costo unitario | Subtotal    |
| --------------------------------------- | ---------- | -------------- | ----------- |
| Sensor RGB TCS34725                     | 1          | $30.900        | $30.900     |
| Sensor MQ-135                           | 2          | $12.000        | $24.000     |
| ESP32-S3                                | 2          | $38.792        | $77.584     |
| Contenedor oscuro y soporte de medición | 1          | Por cotizar    | Por cotizar |
| Referencia blanca/gris para calibración | 1 conjunto | Por cotizar    | Por cotizar |

Subtotal de los dos sensores: $54.900 COP.
Subtotal de los componentes con precio definido (sensores + ESP32-S3): $132.484 COP,  sin incluir montaje.

### Recolección de datos y operación

| Ítem | Base de cálculo | Valor presupuestal |
|---|---|---|
| Bananos para el experimento | 25 unidades, más reposición si es necesaria. | ~ $17.500 |
| API del LLM | Consumo de tokens durante desarrollo y pruebas. | Capa Gratuita |


### Resumen presupuestal y reserva

| Concepto                                  | Valor (COP)        |
| ----------------------------------------- | ------------------ |
| Sensor RGB TCS34725 (1 unidad)            | $30.900            |
| Sensores MQ-135 (2 unidades)              | $24.000            |
| ESP32-S3 (2 unidades)                     | $77.584            |
| Subtotal de hardware con precio definido  | $132.484           |
| Reserva provisional del 10 %              | $13.248            |
| Subtotal de hardware con reserva          | $145.732           |
| Bananos para el experimento (25 unidades) | \~$17.500          |
| API del LLM                               | $0 (capa gratuita) |
| Total estimado                            | $163.232           |

Consideraciones presupuestales:

El valor de $163.232 COP corresponde a una estimación que incluye el hardware con precio definido y los bananos para el experimento.

Este valor no constituye el presupuesto definitivo del proyecto. Quedan pendientes las cotizaciones del contenedor oscuro, el soporte de medición, la referencia blanca/gris para calibración, los consumibles y los posibles gastos de montaje.

La estructura de cálculo es:

**Costo directo = hardware + montaje + consumibles + servicios + otros gastos**

**Presupuesto total = costo directo**

## 5. Riesgos que afectan cronograma y presupuesto

| Riesgo | Efecto sobre el plan | Respuesta propuesta |
|---|---|---|
| Dataset pequeño o poco diverso | Puede impedir alcanzar las métricas objetivo. | Seguimiento riguroso, doble etiquetado y validación por banano. |
| Maduración más rápida de lo esperado | Puede reducir la oportunidad de recoger datos útiles. | Escalonar la compra y revisar diariamente la cobertura de etapas. |
