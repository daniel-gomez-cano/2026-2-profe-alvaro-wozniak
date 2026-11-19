# Bananin — Entregables de los codelabs: índice, hallazgos y pendientes

Preparado el **10-nov-2026** a partir de `GESTION/Documentos` del repositorio (`daniel-gomez-cano/2026-2-profe-alvaro-wozniak`) y de los dos codelabs («Seguimiento de Proyectos con EVM» y «Seguimiento y Métricas: Saberlo a Tiempo»). **No se subió nada al repositorio.**

## 1. Qué hay en esta carpeta

| Archivo | Para qué sirve | Estado |
|---|---|---|
| `EVM_Bananin.xlsx` | **Herramienta de seguimiento**: línea base, PV semanal, horas (AC), seguimiento EVM con fórmulas y semáforos, curva S, tablero. | ✅ Listo; faltan los datos reales del equipo |
| `01_Linea_Base_EVM.md` | Entregable 1 de EVM: WBS, cronograma, BAC, PV acumulado, criterios de aceptación. | 📝 Propuesta; falta aprobar y congelar |
| `02_Seguimiento_EVM.md` | Entregable 2 de EVM: fórmulas, tablas por corte, interpretación, registro de decisiones. | ⏳ PV listo; EV/AC pendientes |
| `03_Tablero_de_Metricas.md` | Paso 4 de «Saberlo a Tiempo»: 5 métricas con umbrales y decisión asociada. | 📝 Confirmar 2 umbrales *supuesto* |
| `04_Cadencia_de_Seguimiento.md` | Paso 5: ritmo de revisión, quién actualiza qué, acta de reunión. | 📝 Falta definir día/hora y aprobar |
| `05_Informe_Ejecutivo_Final_EVM_PLANTILLA.md` | Entregable 3 de EVM (se completa el 16-dic). | 📝 Plantilla |
| `06_Checklist_Codelabs_y_Rubrica.md` | Todo lo que piden los dos codelabs vs. lo que ya hay. | ✅ |
| `curva_S_linea_base.png` | Curva S del plan (PV). | ✅ |
| `Etiquetado/` | Guía de etiquetado E1–E4 + protocolo de sesión (BORRADOR), plantillas CSV y script de concordancia (RF09) con pruebas. | 📝 / ✅ script verificado |

## 2. Lo que necesito de ustedes (en orden de urgencia)

1. **Estado real de las actividades de la línea base.** Para cada una de **A01–A06**: ¿está terminada y aceptada según su criterio (`01_Linea_Base_EVM.md` §2)? ¿En qué fecha? Lo más importante: **A04 (prueba continua de 7 días ≥ 95 %)**, **A05 (guía de etiquetado)** y **A06 (25 bananos con seguimiento)**. Dijeron que los sensores están comprados y probados y que el dataset está «empezando»; con eso no puedo decidir el 0/100 ni inventarlo.
2. **Horas dedicadas desde el 26-oct** (aproximadas, por persona y semana). Sin eso no hay AC ni CPI.
3. **Cuántas sesiones y tomas llevan** y cuántas se perdieron, para el indicador de calidad del tablero.
4. **Tres decisiones** (ver §4): ¿dividir A07?, ¿medir en horas o con los pesos de la EDT?, ¿confirmar los umbrales marcados *supuesto*?
5. **Quién es quién** en la RACI (hardware, datos/ML, backend/LLM, frontend) y cuántos son realmente (4 o 5).
6. **¿Lo subo al repositorio?** Puedo dejarlo en `GESTION/Seguimiento/` y `PI2/`, pero no lo hago sin que me lo pidan.

Con 1 y 2 completo en minutos los cortes S2, S3 y S4 (los 3 reales que exige el codelab) y la curva S queda dibujada.

## 3. Hallazgos sobre el repositorio (verificados contra los archivos)

**Alta prioridad — afectan lo que van a entregar**

| # | Hallazgo | Dónde | Por qué importa |
|---|---|---|---|
| 1 | **Texto de una conversación quedó pegado en el cronograma**: «Perfecto. Cambiamos Hardware… de 32 a 10 horas», «el total pasa de 129 a 107», «El total correcto es 215 horas-persona, no 296». Además aparece **dos veces** la sección «Distribución propuesta del esfuerzo». | `Cronograma.md` | Es documento entregable y se ve como borrador de chat. Además los totales (296 / 107 / 215) se contradicen en el mismo archivo. Usé **215 h**, que es el único que coincide con su tabla final. |
| 2 | **Los pesos de la EDT no reflejan el esfuerzo**: Historial + interfaces pesan 40 % pero llevan ≈ 12 % de las horas; el modelo determinista (con dataset y etiquetado) pesa 10 % y lleva ≈ 37 %. | `EDT_Bananin_criterios_RACI.md` vs `Cronograma.md` | Si miden avance con los pesos de la EDT, el CPI saldrá absurdo. Detalle y tabla en `01_Linea_Base_EVM.md` §8. |
| 3 | **El plan carga 54 h (25 %) en la semana 1** y ≈ 9 h/semana en S2 y S3; A07 (26 h) era un solo paquete. Con 0/100, el SPI máximo posible habría sido 0,93 / 0,79 / 0,69 en S1–S3 aun haciendo todo bien. | `Cronograma.md` | Dividí A07 en 3 tramos para que el seguimiento no dé alarmas falsas. |
| 4 | **La guía de etiquetado (A05) debía estar lista el 29-oct** y no está en el repositorio; el dataset ya empezó. | `Cronograma.md` A05, RNF16 | Si etiquetaron sin guía, RNF16 no se cumple. Dejé un borrador en `Etiquetado/`. |
| 5 | **El esquema de datos no es el mismo en dos documentos**: `Calidad.md` pide columnas `id, estado, dias_estimados, org_vol, og_vol, norm, R, G, B, C, r_norm…`; `Requisitos.md` (RF04/RF10) pide otras, y **no hay regresión de días** (Acta §4.2). | `Calidad.md` vs `Requisitos.md` | Conviene unificar **antes** de seguir acumulando datos. `plantilla_sesiones.csv` sigue RF04/RF10. |

**Media prioridad — inconsistencias entre documentos**

| # | Hallazgo | Dónde |
|---|---|---|
| 6 | Calidad dice precisión de modelos **80 %** y respuesta **≤ 5 s**; el acta y los requisitos dicen **85 % dentro de ±1 etapa** (RNF03) y **< 30 s** (RNF02). | `Calidad.md` vs `Requisitos.md` / Acta |
| 7 | `Calidad.md` tiene la «Definición de terminado» de **Frontend vacía**, y la de «Nube» habla de «modelos no deterministas» antes de los deterministas. | `Calidad.md` |
| 8 | **Equipo de 5 vs 4**: el Acta §8 dice «5 estudiantes con 8 h/semana»; el Acta lista 4 integrantes y `Criterios de Planificacion.md` dice 4 (32 h/sem), mientras el `Cronograma.md` usa 40 h/sem. | Acta / Criterios / Cronograma |
| 9 | El presupuesto compra **2 ESP32-S3** ($77.584), pero el Acta §8 y RNF08 dicen «ESP32-S3 ya disponible / reutilizando la disponible». | `Presupuesto.md` vs Acta |
| 10 | Contenedor oscuro, soporte, referencia blanca/gris, consumibles y montaje están **«por cotizar»**, y son parte del nodo que debía estar listo el 21-oct. | `Presupuesto.md` |
| 11 | Fechas de firma del Acta (25-sep, 28-sep, 2-oct) **anteriores a la fecha del acta** (5-oct). | Acta §1 y §11 |
| 12 | La **RACI tiene los 4 roles técnicos «(asignar al revisar)»**, aunque el `READMEpls.md` la marca como bloqueada (P6). El README menciona «OBS» (P5) y **no encontré** un archivo de OBS. | `EDT_…_RACI.md`, `GESTION/READMEpls.md` |
| 13 | La línea base EVM solo cubre **26-oct → 16-dic**. Lo hecho del 6 al 25-oct (Acta, requisitos, EDT, compras) no está medido. | `Cronograma.md` |
| 14 | `Riesgos_proyecto_Bananin.md` lista riesgos pero **sin probabilidad, impacto, dueño ni respuesta** (el Acta §9 sí los tiene para 5). Los codelabs lo señalan como siguiente paso. | `Riesgos_proyecto_Bananin.md` |

## 4. Decisiones que dejé tomadas por defecto (y cómo revertirlas)

| Decisión | Por qué | Cómo cambiarla |
|---|---|---|
| **BAC = 215 horas-persona** | Coincide con el Cronograma y permite que EV y AC estén en la misma unidad. | Cambiar las horas en `Linea_Base`. |
| **A07 dividida en 13 + 9 + 4 h** | Evita un SPI artificialmente bajo con 0/100. | Pídanmelo y regenero el libro con A07 entera (el libro asume 20 filas de actividades; no conviene borrar filas a mano). |
| **Horas por actividad dentro de cada categoría** | Los totales por categoría son oficiales; el reparto interno es mío. | Editar la columna «Horas» en `Linea_Base` (la tabla de control muestra «OK» si suman lo oficial). |
| **PV lineal por día calendario** | Es el método del codelab (plan repartido durante la duración de cada paquete). | — |
| **Umbrales *supuesto*:** trabajo en curso ≤ 4, etapa menos representada ≥ 15 % | No están en el acta. | Hoja `Tablero`, celdas amarillas. |

## 5. Otros archivos que el proyecto va a necesitar (no los generé, ordenados por fecha del cronograma)

| Para | Archivo / entregable | Ref. |
|---|---|---|
| **Ya** | Evidencia de la prueba de 7 días (conteo de tomas recibidas con ambas lecturas / programadas) | A04, RNF01 |
| **Ya** | Confirmación escrita del bodeguero sobre la escala de 4 etapas | RNF16 |
| **Ya** | Cotizaciones/facturas de cada sensor dentro del rango (RNF08) y decisión sobre el MQ-135 por escrito | A01, RNF08 |
| 18-nov (H3) | Dataset de 25 bananos con doble etiquetado + **informe de concordancia** (script ya listo) | A07, A08, RF06–RF09 |
| 19–22-nov | **Script de preprocesamiento** y particiones por banano sin fuga | A09, RF11, RF13 |
| 23–27-nov | **Informe de métricas**: 2 modelos × 2 configuraciones (solo color / color + MQ-135), matriz de confusión, exacta y ±1; entrenamiento reproducible (semilla, dependencias fijas) | A10, RF14, RNF03, RNF11 |
| 19–30-nov | **Tabla de contexto versionada** (CSV/JSON) con fuente o «supuesto» por fila y **validación del cliente**; esquema JSON del LLM; reglas con casos de prueba | A11, RF15–RF19 |
| 28-nov–2-dic (H4) | 12 escenarios de prueba con acción esperada; script de **20 repeticiones**; registro de llamadas al LLM | RNF05–RNF07, RNF18 |
| 3–9-dic | Archivo de configuración de ejemplo (sin claves); 10 preguntas de prueba del chatbot | RNF12, RF29 |
| 10–15-dic | **README reproducible** (incluye precalentamiento y línea base del MQ-135); evidencias de pruebas de aceptación | RNF17, A16–A17 |
| Cuando puedan | Registro de riesgos con probabilidad/impacto/dueño/respuesta; RACI con nombres | Hallazgos 12 y 14 |

Puedo preparar cualquiera de estos; los de la parte de modelos/LLM dependen de datos que todavía no existen.

## 6. Qué verifiqué y qué no

- **Verificado:** las fórmulas del libro (BAC, PV por semana, EV, AC, SV, CV, SPI, CPI, EAC₁₋₃, ETC, VAC, TCPI, semáforos, tablero) con un motor de cálculo en Python y un escenario **ficticio** en una copia temporal: **44 de 44 comprobaciones coinciden** con un cálculo independiente. El `.xlsx` que les entrego está **sin datos de prueba**. El script de concordancia: 5 pruebas y **500 casos aleatorios idénticos a scikit-learn** (kappa cuadrático). Las cifras y fechas que cito de los documentos del repo (horas, totales, hitos, umbrales de RNF/RF) las contrasté con los archivos originales.
- **No verificado:** no pude abrir el libro en Excel ni LibreOffice (no hay LibreOffice en este equipo), así que **no vi los gráficos dibujados ni el aspecto visual**. Al abrirlo, Excel calcula las fórmulas; si ven celdas vacías o un gráfico raro, avísenme. La hoja `Datos_Grafico` muestra `#N/A` a propósito en las semanas sin reportar.
- **Supuestos míos** (marcados en cada documento): reparto de horas por actividad, criterios visuales de la guía de etiquetado, umbrales del tablero y cadencia.
