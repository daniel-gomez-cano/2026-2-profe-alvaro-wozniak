# Bananin — Tablero de métricas

> **Paso 4 del codelab «Saberlo a Tiempo»** («Arma el tablero»): cuatro o cinco métricas, **una de cada grupo** (avance, costo/esfuerzo, calidad, flujo), con **la decisión que se va a tomar con cada una**. La métrica que no tenga decisión, se quita.
>
> Estado: **propuesta para aprobar.** Los umbrales marcados como *supuesto* no están en el acta ni en los requisitos; son míos y el equipo debe confirmarlos. El tablero vivo está en `EVM_Bananin.xlsx` → hoja `Tablero`.

## 1. Las 5 métricas

| # | Grupo | Métrica | Verde | Ámbar | Rojo | Fuente | Responsable (rol) | Frecuencia |
|---|---|---|---|---|---|---|---|---|
| 1 | **Avance** | **SPI** = EV / PV | ≥ 0,95 | 0,85–0,94 | < 0,85 | Hoja `Seguimiento` | Gerente | Semanal |
| 2 | **Costo / esfuerzo** | **CPI** = EV / AC (horas) | ≥ 0,95 | 0,85–0,94 | < 0,85 | Hojas `Horas_AC` y `Seguimiento` | Gerente | Semanal |
| 3 | **Calidad** | **% de tomas recibidas con ambas lecturas** (TCS34725 + MQ-135) | ≥ 95 % | 90–94,9 % | < 90 % | Backend / registro de sesiones | Resp. hardware | Semanal |
| 4 | **Flujo** | **Trabajo en curso** (actividades iniciadas y sin terminar) | ≤ 4 *(supuesto)* | 5 | ≥ 6 | Hoja `Linea_Base` (inicio real / aceptación) | Gerente | Diaria (10 min) |
| 5 | **Calidad de datos** *(riesgo del Cronograma)* | **Participación de la etapa menos representada** entre las sesiones etiquetadas | ≥ 15 % *(supuesto)* | 10–14,9 % | < 10 % | Hoja de etiquetado (RF07) | Resp. datos/ML | Semanal |

Origen de los umbrales:

- **SPI/CPI**: semáforo RAG del codelab de EVM.
- **% de tomas ≥ 95 %**: viene directo de **RNF01** del `Requisitos.md` (y del criterio de aceptación de EDT 2.1.1).
- **Trabajo en curso ≤ 4**: *supuesto* = número de integrantes (regla práctica de la ley de Little). El propio plan llega a 5 actividades simultáneas el 26-oct, por eso 5 es ámbar y no rojo.
- **Etapa menos representada ≥ 15 %**: *supuesto*. Con 4 etapas equilibradas cada una sería 25 %; por debajo de 10 % el clasificador casi no ve esa etapa. Atiende el riesgo que el propio `Cronograma.md` señala («maduración más rápida de lo esperado»).

## 2. La decisión que habilita cada métrica

| # | Si la métrica se pone en rojo… | Decisión que se toma |
|---|---|---|
| 1 SPI | < 0,85 | En la reunión semanal se decide **qué se recorta, qué se aplaza o cómo cambia el trabajo**. Nunca «esforzarse más». |
| 2 CPI | < 0,85 | **Reestimar** lo que falta (EAC) y decidir entre recortar alcance o pedir más horas al patrocinador. |
| 3 % tomas | < 95 % | **Parar la recolección** y arreglar conectividad/buffer (RF05) antes de seguir sumando sesiones: cada toma perdida es dato perdido de un banano que no se puede repetir. |
| 4 Trabajo en curso | ≥ 6 | **No se empieza nada nuevo** hasta cerrar algo. Con el mismo equipo, empezar menos cosas hace que cada una salga antes. |
| 5 Balance de etapas | < 10 % | **Escalonar la compra de bananos** o ajustar el seguimiento para cubrir la etapa faltante; si no se alcanza a tiempo, documentarlo como riesgo (nunca simular la cobertura). |

## 3. Chequeo de «un tablero equilibrado»

El codelab pide al menos una métrica de cada grupo para que ninguna dimensión se optimice destruyendo otra:

| Grupo | Cubierto por |
|---|---|
| Avance | SPI |
| Costo / esfuerzo | CPI |
| Calidad | % de tomas recibidas con ambas lecturas (+ balance de etapas para la calidad del dataset) |
| Flujo | Trabajo en curso |

Cinco métricas: dentro del rango «cuatro o cinco» del codelab. Más y la revisión semanal se abandona en la semana seis.

## 4. Relevo de métricas por fase (para que el tablero no se quede viejo)

| Hito | Qué cambia |
|---|---|
| **H3 (18-nov)** | Se **agrega** el acuerdo entre etiquetadores ±1 etapa **≥ 90 %** (RNF04) y se reporta el kappa ponderado (sin umbral, RF09). |
| **H4 (2-dic)** | La métrica 3 se **reemplaza** por «±1 etapa ≥ 85 % en bananos no vistos» (RNF03) y se agrega «misma acción ≥ 18/20 repeticiones del LLM» (RNF05). |
| **H5 (16-dic)** | Se agrega «tiempo de sesión completa → dashboard < 30 s en ≥ 19 de 20 sesiones» (RNF02). |

Siempre se mantienen **cinco o menos**: al entrar una, sale la que ya no habilite una decisión.

## 5. Lo que a propósito NO está en el tablero

- **Métricas DORA** (frecuencia de despliegue, tiempo de cambio a producción, % de fallos, tiempo de restauración): el codelab las presenta como estándar de la industria, pero este es un prototipo IoT de **un solo nodo, sin CI/CD ni producción**. Medirlas sería una métrica de vanidad. Si luego se monta despliegue continuo del backend/dashboard, se reevalúa.
- **Cobertura de pruebas**: el codelab advierte que se manipula con facilidad. Los criterios de verificación de los RF/RNF ya cubren la calidad.
- **Cantidad de tomas o de bananos «registrados»**: mide actividad, no resultado.

## 6. Entradas que hay que alimentar cada semana

| Dato | Dónde se escribe | Quién |
|---|---|---|
| Horas por persona | `Horas_AC` | Cada integrante |
| Fecha de inicio real y de aceptación de cada actividad | `Linea_Base` | Responsable del paquete |
| Tomas programadas y recibidas con ambas lecturas | `Tablero` (entradas) | Resp. hardware |
| Sesiones etiquetadas por etapa (E1–E4) | `Tablero` (entradas) | Resp. datos/ML |
