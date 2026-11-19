# Bananin — Checklist de los codelabs y rúbrica

Cruce entre **lo que piden los dos codelabs** y **lo que ya existe** (estado al 10-nov-2026). Leyenda: ✅ listo · 📝 preparado, falta aprobación o datos del equipo · ⏳ depende del avance real · ❌ no existe aún.

## 1. Codelab «Seguimiento y Métricas: Saberlo a Tiempo» — paso 11 «Aplícalo a tu proyecto»

| Paso del codelab | Qué pide | Estado | Dónde está / qué falta |
|---|---|:--:|---|
| 1. Confirma tu línea base | Alcance en la EDT, tiempo en el cronograma, costo repartido por paquete | 📝 | `01_Linea_Base_EVM.md`. Falta aprobar y registrar la fecha de congelación. |
| 2. Calcula el valor ganado de hoy | PV, EV, AC con regla todo-o-nada; variaciones, índices y cuadrante | ⏳ | **PV ✅** (63,0 h a 8-nov; 72,0 h a 15-nov). **EV y AC: faltan datos reales del equipo.** |
| 3. Si estás en rojo, decide | Recortar alcance, mover fecha o cambiar forma de trabajo; escribir cuál y por qué | ⏳ | Registro de decisiones listo en `02_Seguimiento_EVM.md` §4. Se llena tras el primer corte. |
| 4. Arma el tablero | 4–5 métricas, una por grupo, con la decisión que habilita | 📝 | `03_Tablero_de_Metricas.md` + hoja `Tablero`. Falta confirmar los 2 umbrales marcados *supuesto*. |
| 5. Acuerden la cadencia | 10 min diarios, 20 semanales, por escrito en el repo | 📝 | `04_Cadencia_de_Seguimiento.md`. Falta definir día/hora/canal y que el equipo lo apruebe. |
| Reto adicional | SPI y CPI cada semana un mes, línea en el tiempo | 📝 | Gráfico SPI/CPI en la hoja `Curva_S`; se llena con los cortes. |

### Validación del codelab (7 preguntas)

| Pregunta | Estado | Nota |
|---|:--:|---|
| ¿Línea base congelada y se sabe la fecha? | 📝 | Falta la fecha (`01_Linea_Base_EVM.md` §7). |
| ¿Calculaste PV, EV y AC con la regla de todo o nada? | ⏳ | PV sí; EV/AC con datos reales. |
| ¿Sabes en qué cuadrante de SPI/CPI está tu proyecto? | ⏳ | Tras el primer corte. |
| ¿El tablero tiene métrica de avance, esfuerzo, calidad y flujo? | ✅ | SPI · CPI · % tomas · trabajo en curso (+ balance de etapas). |
| ¿Cada métrica tiene escrita la decisión que habilita? | ✅ | `03_Tablero_de_Metricas.md` §2. |
| ¿La cadencia está acordada por escrito? | 📝 | Propuesta lista; falta el acuerdo del equipo. |
| ¿La última revisión terminó en una decisión? | ⏳ | Aún no hubo revisión con este tablero. |

## 2. Codelab «Seguimiento de Proyectos con EVM» — Actividad del Proyecto

| Entregable | Contenido exigido | Estado | Dónde |
|---|---|:--:|---|
| **1. Línea base** (semana 1) | WBS con % del BAC · cronograma con fechas · BAC definido y justificado · columna PV acumulado de todas las semanas | ✅ | `01_Linea_Base_EVM.md`, hoja `PV_Semanal`. *(Nota: el codelab pide hacerla en la semana 1; el proyecto ya va por la semana 3, así que se aprueba ahora y se congela con fecha.)* |
| **2. Seguimiento semanal** (cada semana) | Tabla EVM (PV, EV, AC, SV, CV, SPI, CPI, EAC, ETC, VAC, TCPI) · curva S actualizada · semáforo RAG · interpretación y acción | ⏳ | Estructura lista en `EVM_Bananin.xlsx` y `02_Seguimiento_EVM.md`. **Faltan ≥ 3 cortes reales.** |
| **3. Informe ejecutivo final** | Curva S completa · evolución SPI/CPI · EAC final vs BAC · lecciones aprendidas | 📝 | `05_Informe_Ejecutivo_Final_EVM_PLANTILLA.md` (se completa el 16-dic). |

### Criterios de aceptación del codelab

| Criterio | Estado |
|---|:--:|
| SPI, CPI, EAC y VAC calculados con la fórmula visible (no solo el número) | 📝 Fórmulas visibles en la hoja `Seguimiento` y en `02_Seguimiento_EVM.md` §2. |
| Curva S con tres líneas diferenciadas (PV, EV, AC) en el mismo gráfico | 📝 Gráfico listo; PV dibujada, EV y AC aparecen al reportar. |
| EV nunca sube si el entregable no está terminado y aceptado (0/100) | ✅ Diseñado así: EV se calcula solo con fechas de aceptación. |
| Semáforo RAG explícito en cada corte | ✅ Columnas K, L y S de `Seguimiento`. |
| Al menos una acción correctiva por desviación donde SPI o CPI < 0,90 | ⏳ Columna T de `Seguimiento`. |

### Rúbrica de evaluación (100 pts) — autoevaluación

| Criterio | Pts | Descripción | Estado hoy | Para asegurar los puntos |
|---|---:|---|---|---|
| Línea base completa y coherente | 20 | WBS, cronograma, BAC y PV definidos antes de empezar | Casi completa | Aprobar y congelar con fecha; resolver el desacuerdo pesos EDT vs. horas (`01` §8). |
| EVM correcto (fórmulas y cálculos) | 30 | PV, EV, AC, SPI, CPI, EAC con fórmulas visibles en ≥ 3 cortes | Herramienta lista y verificada | Registrar horas y aceptaciones de S2, S3, S4. |
| Curva S con las tres líneas | 20 | PV, EV y AC graficados, con interpretación escrita | Falta EV/AC | Datos de los cortes + 3–4 líneas de interpretación por corte. |
| Análisis de desviaciones | 20 | Por corte: causa, semáforo RAG, acción concreta | Plantilla lista | Llenar en cada corte (aunque sea verde, justificar). |
| Informe ejecutivo final | 10 | Tendencia SPI/CPI, lecciones aprendidas | Plantilla lista | Se hace el 16-dic con los datos reales. |

> Esta es la rúbrica que trae el codelab. Conviene confirmar con el docente (Álvaro Salazar) si la entrega se evalúa con esta u otra, y en qué formato.
