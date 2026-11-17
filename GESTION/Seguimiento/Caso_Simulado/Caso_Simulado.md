# Bananin — Caso simulado para la sustentación EVM

> **Todo el avance, las horas y las fechas de aceptación de esta carpeta son inventados.** Sirven de modelo para armar la presentación real. No los presenten como datos del equipo.

Archivos de esta carpeta:
- `Sustentacion_EVM_Bananin_CASO_SIMULADO.pptx`: 18 diapositivas con notas del orador. Las 12 primeras son la sustentación y las 6 últimas (13–18) son el **anexo de respaldo**, que no se presenta: se abre solo si preguntan de dónde sale un número.
- `EVM_Bananin_CASO_SIMULADO.xlsx`: el mismo libro de seguimiento, lleno con los datos del caso, más tres hojas de respaldo (`Registro_Horas`, `Evidencias` y `Hecho_Falta`). De aquí salen todas las cifras de las diapositivas.

## 1. El escenario

Corte al **16-nov-2026**, con tres cortes cerrados: S1 (1-nov), S2 (8-nov) y S3 (15-nov).

- **Sensores:** configurados y probados correctamente (A01–A04 aceptadas).
- **Interfaces:** dashboard, historial y chatbot terminados y aceptados (A13–A15), **antes** de lo planificado (el plan las pone del 3 al 9-dic).
- **Todo lo demás:** sin aceptar. Guía de etiquetado (A05), 25 bananos (A06) y tramo 1 del seguimiento (A07.1) están empezados, pero no terminados.

Resultado calculado por el libro (horas-persona, BAC = 215 h):

| Corte | PV | EV | AC | SV | CV | SPI | CPI |
|---|---:|---:|---:|---:|---:|---:|---:|
| S1 · 1-nov | 53,9 | 32,0 | 46,0 | −21,9 | −14,0 | 0,59 rojo | 0,70 rojo |
| S2 · 8-nov | 63,0 | 52,0 | 84,0 | −11,0 | −32,0 | 0,83 rojo | 0,62 rojo |
| S3 · 15-nov | 72,0 | 85,0 | 112,0 | +13,0 | −27,0 | **1,18 verde** | **0,76 rojo** |

Pronóstico al corte S3: EAC = 215 ÷ 0,76 ≈ **283 h** (68 h por encima del plan), ETC ≈ 171 h, TCPI = 1,26 (rojo).

**La lectura que enseña el caso:** el SPI verde engaña. Hardware va al plan (40 de 40 h), las interfaces están adelantadas (45 h ganadas con plan en diciembre) y el dataset está en **0 de 32 h**, justo cuando el hito H3 (dataset con doble etiquetado) es el 18-nov. El CPI en rojo sale de gastar 112 h para ganar 85.

## 2. Qué es real y de dónde sale

| Dato | Fuente |
|---|---|
| BAC 215 h, 20 filas de actividades, horas, fechas y criterios de aceptación | Línea base (`01_Linea_Base_EVM.md`), construida desde `Cronograma.md` |
| PV de cada corte (53,9 / 63,0 / 72,0 / 100,7 h…) | Calculado con la línea base |
| Fórmulas, regla 0/100 y umbrales del semáforo (0,95 / 0,85) | Codelab «Seguimiento de Proyectos con EVM» |
| Nombres del equipo | Acta de constitución (§ equipo de desarrollo) |
| Hitos (H3 dataset 18-nov, H4 2-dic, H5 16-dic) | `Cronograma.md` |
| Una toma por minuto en el nodo (`PERIODO_HISTORIAL_MS = 60000`) | Código de la rama `Sensor` |
| Cálculo de todas las métricas | Libro de Excel, verificado con un motor de fórmulas independiente (todas las cifras coinciden con las cuentas a mano) |

## 3. Qué inventé y por qué

| # | Lo inventado | Por qué |
|---|---|---|
| 1 | **Fecha de la sustentación (16-nov) y tres cortes cerrados (S1–S3).** | Es la fecha de hoy. El corte del 22-nov todavía no existe, así que solo se usan cortes que ya pasaron. |
| 2 | **Fechas de aceptación:** A01 27-oct, A02 29-oct, A03 1-nov, A04 5-nov, A13 7-nov, A14 12-nov, A15 14-nov. | Hacen falta fechas para que el EV suba por semana. Las escogí dentro o después de la ventana planificada de cada actividad. A04 tarda hasta el 5-nov porque la prueba dura 7 días continuos. |
| 3 | **A13, A14 y A15 aceptadas antes de lo planificado.** | Es la condición que pediste («interfaces de todo»). Es lo que hace que el SPI quede verde mientras el dataset no avanza. |
| 4 | **A15 (chatbot) aceptada sin que el backend del LLM (A11) esté hecho.** | Es lo menos realista del caso: el criterio de A15 pide respuestas con datos almacenados. Lo dejé así por tu condición. Si en la presentación real ocurre algo parecido, hay que explicar con qué datos y con qué backend se aceptó. |
| 5 | **Horas por persona por semana:** S1 = 14+12+11+9 = 46 h, S2 = 11+10+9+8 = 38 h, S3 = 8+7+7+6 = 28 h (AC 46 / 84 / 112). | No existen horas reales. Quise un CPI < 1 verosímil: horas de más en hardware y horas gastadas en dataset sin entregable aceptado. La caída de horas por semana imita el cansancio de un grupo universitario. |
| 6 | **Desglose del AC de 112 h: 52 h en hardware, 40 en interfaces y 20 en dataset**, y las horas de cada actividad (por ejemplo, A04 con 13 h frente a 8 planificadas). | Justifica la frase «20 h de dataset sin EV» y da un CPI < 1 verosímil. Ahora sí está en el libro: sale del `Registro_Horas`. |
| 6a | **El `Registro_Horas`: 35 registros** (fecha, integrante, actividad, horas, qué se hizo, fuente) que suman 112 h. | Hace falta un registro de origen para que el AC se pueda defender. Repartí las horas de cada persona y semana entre actividades y días, con una regla: **ninguna hora cae antes del inicio real de una actividad ni después de su aceptación** (el programa lo verifica). Las descripciones y la fuente («registro personal del integrante») son inventadas. |
| 6b | **La hoja `Evidencias`:** tipo de prueba, qué muestra, quién aceptó y dónde está, para las 7 actividades aceptadas. | Respalda el EV. Quién aceptó (Edward Pinto como patrocinador, Daniel Gómez como gerente) y los nombres de archivo (`Evidencias/A04_reporte_7_dias.csv`, etc.) **son inventados: esos archivos no existen**. Para las 3 actividades en curso (A05, A06 y A07.1) anoté qué falta. |
| 7 | **Tomas del nodo: 10.080 programadas y 9.826 recibidas (97,5 %).** | 10.080 = una por minuto durante 7 días, que viene del firmware. Las 9.826 son inventadas: solo para que la métrica de calidad del libro (RNF01, ≥ 95 %) salga verde. Está en el libro, no en las diapositivas. |
| 8 | **Ejemplo «la guía A05 al 90 % suma 0 h» (diapositiva 3).** | Ilustra la regla 0/100 con una actividad real del proyecto. |
| 9 | **Acciones correctivas, responsables y fechas** (diapositiva 11). Santiago Avalo con A05/A06; Daniel Gómez con congelar el frontend; ambos con el tope de horas. | Los asigné según las áreas de `Calidad.md` (Santiago Avalo con el dataset) y el rol de gerente del Acta (Daniel Gómez). Las fechas (17, 20 y 22-nov) son inventadas. Ajústenlas a su RACI real. |
| 10 | **Propuesta de mover H3 al 25-nov.** | Es una posible decisión de «mover la fecha», una de las palancas del codelab. Habría que negociarla con el patrocinador (Edward Pinto, según el Acta). |
| 11 | **Meta del 22-nov: CPI ≥ 0,80, con A05, A06 y A07.1 aceptadas (+23 h de EV).** | Cuenta hecha: EV pasaría de 85 a 108 h; para CPI 0,80 el AC debe quedar en 135 h o menos, es decir **≤ 23 h de trabajo en la semana 4**. Es una meta, no una predicción. |
| 12 | **«A10 pasaría de 26 h a unas 15 h» si se entrena un solo modelo.** | Estimación inventada, solo en las notas del orador, para mostrar cómo se recorta alcance con números. |

## 3b. El respaldo: qué pregunta responde cada pieza

| Pregunta del profesor | Diapositiva del anexo | Hoja del libro |
|---|---|---|
| ¿De dónde sale el plan y el BAC? | 14 · Línea base: 215 h en 7 frentes | `Linea_Base` |
| ¿Cómo calcularon el PV? | 15 · Cómo se calcula el PV (fórmula, ejemplo con A07.1 y PV por semana) | `PV_Semanal` |
| ¿Por qué esas actividades cuentan como terminadas? | 16 · EV: qué se aceptó y con qué evidencia | `Evidencias` |
| ¿De dónde salen las horas? | 17 · AC por persona, semana y frente | `Registro_Horas` → `Horas_AC` |
| ¿Qué hicieron y qué falta? | 18 · Plan, ganado y gastado por frente, con estado | `Hecho_Falta` |

Cómo está conectado el libro: `Registro_Horas` alimenta `Horas_AC` (ya no se teclea), `Horas_AC` y `Linea_Base` alimentan `Seguimiento` (EV, AC, SPI, CPI), y `Evidencias` y `Hecho_Falta` cruzan las tres. La hoja `Evidencias` marca **«FALTA EVIDENCIA»** si una actividad está terminada y no tiene prueba o quién aceptó, y **«EV sin aceptación»** si una actividad suma EV sin estar aceptada. Verifiqué que los totales cuadran: PV 72,0, EV 85,0 y AC 112,0 coinciden entre `Hecho_Falta`, `Evidencias`, `Registro_Horas` y `Seguimiento`.

## 4. Cómo usarlo para la presentación real

| Diapositiva | Qué se reemplaza con datos reales | De dónde sale |
|---|---|---|
| 1 | Integrantes y fecha | Acta |
| 2 | Nada (revisar que A07 dividida y «horas» sean lo que decidieron) | `01_Linea_Base_EVM.md` |
| 3–4 | Solo los ejemplos de Bananin | Codelab y su propio avance |
| 5 | Series EV y AC | Hoja `Curva_S` del libro |
| 6 | Toda la tabla de cortes | Hoja `Seguimiento` |
| 7–9 | Cifras, semáforo y cuadrante | Hoja `Seguimiento` y hoja `Tablero` |
| 10 | Plan y ganado por frente | Hoja `Linea_Base` (horas por categoría y actividades aceptadas) |
| 11 | Datos, causas, acciones, responsables y fechas | Reunión del equipo |
| 12 | Estado y decisión | Lo mismo que 7–11 |

Pasos, en orden:
1. Llenar en el libro: fecha de aceptación (columna K de `Linea_Base`), «SI» en cada corte reportado (`Seguimiento`), y **las horas en `Registro_Horas`**, una fila por bloque de trabajo con la fecha del día en que se trabajó. En el libro limpio `EVM_Bananin.xlsx` todavía no existen las hojas `Registro_Horas`, `Evidencias` y `Hecho_Falta`; si quieren usar ese libro, hay que agregarlas (se puede copiar de este).
2. Guardar de verdad cada evidencia (acta, capturas, reporte) y anotar en `Evidencias` quién aceptó y dónde está el archivo. Si no se puede abrir, no cuenta.
3. Pasar las cifras a la presentación. El libro calcula SPI, CPI, EAC y los colores.
4. Quitar «caso simulado» del pie de las diapositivas (Vista > Patrón de diapositivas) y la etiqueta de la portada.
5. Revisar las notas del orador de cada diapositiva: explican qué decir, qué criterio de la rúbrica cubre y qué preguntas puede hacer el profesor.

## 5. Límites de lo verificado

- **No pude ver la presentación ni el libro en PowerPoint o Excel**: no hay LibreOffice en este equipo. Verifiqué que los archivos sean válidos, que ninguna figura se salga de la lámina y que el texto quepa midiéndolo con las fuentes. Revisen visualmente la gráfica de la diapositiva 5, el cuadrante de la 9 y la tabla de la 6.
- **Los números sí están verificados**: las cifras de las diapositivas salen del libro calculado con un motor de fórmulas independiente de Excel, y coinciden con las cuentas hechas a mano.
