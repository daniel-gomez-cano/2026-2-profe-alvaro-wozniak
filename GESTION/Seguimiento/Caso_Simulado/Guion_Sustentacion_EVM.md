# Bananin — Guion de la sustentación EVM (5 integrantes, máximo 15 min)

> **Caso simulado.** Las cifras en **negrita** son del caso simulado (`EVM_Bananin_CASO_SIMULADO.xlsx`). En la presentación real se cambian por las de ustedes. Todo lo demás (definiciones, fórmulas, umbrales del semáforo, descripción del proyecto) sale del codelab y del Acta de constitución.

## 1. Resumen de tiempos

Los tiempos salen de contar las palabras de cada bloque a **135 palabras por minuto**, que es un ritmo cómodo para presentar con pausas para señalar la diapositiva. El guion suma **12 min**. Con los 4 cambios de persona y de diapositiva (unos 15 s cada uno) llega a **unos 13 min**, o sea **2 min de colchón** bajo el límite de 15. Si en un ensayo pasan de 14 min, recorten lo de «Si falta tiempo» (sección 6).

| # | Sección | Tema (diapositiva) | Quién | Tiempo | Acumulado |
|---|---|---|---|---:|---:|
| 1 | Introducción | Qué es Bananin, qué queremos y dónde vamos | John | 1:40 | 1:40 |
| 2 | Panorama | Línea base (diap. 2 del PPTX) | John | 0:35 | 2:15 |
| 2 | Panorama | Tres variables y cálculo del PV (diap. 3 y 15) | Jorman | 1:00 | 3:15 |
| 2 | Panorama | EV: qué se aceptó y con qué evidencia (diap. 16) | Jorman | 0:55 | 4:10 |
| 2 | Panorama | AC: horas por persona y semana (diap. 17) | Jorman | 0:35 | 4:45 |
| 3 | Métricas EVM | Desviaciones e índices (diap. 4) | Daniel | 0:40 | 5:25 |
| 3 | Métricas EVM | Curva S (diap. 5) | Daniel | 1:05 | 6:30 |
| 3 | Métricas EVM | SPI: tabla de cortes y cálculo (diap. 6 y 7) | Daniel | 0:45 | 7:15 |
| 3 | Métricas EVM | CPI: cálculo y pronóstico (diap. 7) | Edward | 0:50 | 8:05 |
| 3 | Métricas EVM | Semáforo RAG (diap. 8) | Edward | 0:40 | 8:45 |
| 4 | Plan de mitigación | ¿Dónde estamos? (diap. 9) | Edward | 0:40 | 9:25 |
| 4 | Plan de mitigación | El SPI verde esconde un atraso (diap. 10) | Avalo | 0:45 | 10:10 |
| 4 | Plan de mitigación | Plan de acciones correctivas (diap. 11) | Avalo | 1:20 | 11:30 |
| 4 | Plan de mitigación | Qué decidimos hoy (diap. 12) | Avalo | 0:30 | 12:00 |

Por persona: **John 2:15 · Jorman 2:30 · Daniel 2:30 · Edward 2:10 · Avalo 2:35.**

Los números de diapositiva son los de `Sustentacion_EVM_Bananin_CASO_SIMULADO.pptx`. En su Canva usen el mismo contenido en el mismo orden.

## 2. Qué le falta a la sección 3 (y qué cambiar en el Canva)

La sección 3 es el núcleo: la rúbrica reparte la nota entre la curva S (20 %), el cálculo de SPI y CPI (25 %), la comprensión de las métricas (25 %), el análisis de la situación (15 %) y el plan de acciones (15 %). Las secciones 2 y 3 sostienen el 70 % de la nota y la 4 el 30 %. Mirando su plan:

1. **Falta una diapositiva de definiciones.** El 25 % de «comprensión» pide explicar con terminología correcta qué representa PV, EV, AC, SPI y CPI, no repetir fórmulas. Pónganla al inicio del Panorama («Tres variables: PV, EV y AC», diap. 3) y otra al inicio de la sección 3 («Desviaciones e índices», diap. 4). Sin ellas, esa nota depende de que alguien improvise la teoría.
2. **Falta la tabla de cortes (diap. 6).** La rúbrica dice «se verifican los cálculos» y «bien derivados de los datos». La tabla con PV, EV, AC, SV, CV, SPI y CPI de los tres cortes es la prueba. Con solo la curva S el profesor no ve la derivación.
3. **La cuenta a la vista (diap. 7).** Debe verse «EV ÷ PV = 85,0 ÷ 72,0 = 1,18». Que se vea el dato, la fórmula y el resultado, no solo el número.
4. **La interpretación debe decir las dos frases literales** (la rúbrica de «análisis» las pide): «estamos adelantados/atrasados» y «estamos por encima/por debajo del presupuesto». Están en el guion de Edward (diap. 9).
5. **El pronóstico (EAC) es opcional**, pero justifica las acciones del final. Lo dejé como una frase en el guion del CPI, sin diapositiva propia.
6. **Los números de diapositiva del anexo (13–18)** quedan fuera del tiempo. Tengan a mano el PPTX para abrirlos si preguntan.

## 3. Cosas por corregir antes de ensayar

- **Equipo de 5 o de 4.** El Acta y mi PPTX nombran a 4 (Avalo, Daniel, Edward y Santiago Useche). Ustedes son 5 (Daniel, Avalo, Edward, John y Jorman). Hay que actualizar el Acta, la portada y, en el libro, las columnas de `Horas_AC` (hoy son 4 personas). Si no, el profesor puede preguntar por qué los nombres no coinciden.
- **Quién acepta.** El Acta dice en un lugar que el patrocinador es Edward y en otro que es el docente. En `Evidencias` puse a Edward y a Daniel como quienes aceptan. Acuerden una regla y úsenla igual en toda la presentación.
- **Roles por persona.** Repartí los temas según lo que hizo cada uno en el repositorio: Jorman escribió el Cronograma y el Presupuesto (por eso explica PV, EV y AC), John corrigió los Requisitos (por eso abre), Daniel es el gerente (por eso lleva la curva y el SPI) y Avalo es el responsable del dataset (por eso lleva el atraso del dataset y las acciones). Cámbienlo si prefieren otra distribución: los bloques son contiguos para que el relevo sea fácil.

---

## 4. Guion por persona

Convenciones: **[Diap. N]** indica cuándo cambiar de diapositiva. *(Gesto)* son indicaciones. El texto en cita es lo que se dice; adáptenlo con sus palabras, pero mantengan los términos (valor planificado, valor ganado, costo real, SPI, CPI) y las cifras.

### 4.1 John — Introducción y línea base (2:15)

**Introducción (1:40)** — [Diap. 1 y las de contexto del Canva]

> Buenos días, profe y compañeros. Somos el equipo de Bananin, y hoy les mostramos cómo seguimos el proyecto con la técnica del valor ganado, o EVM. Yo soy John y les cuento qué es Bananin.
>
> Hoy el destino de un lote de bananos, venderlo cerca, enviarlo lejos, procesarlo o descartarlo, se decide a ojo, sin datos objetivos de su maduración. Eso genera pérdidas: fruta que llega pasada o que se vende por debajo de su valor. De ahí nace el proyecto. *(Si quieren, aquí cuenten en una frase cómo se les ocurrió la idea.)*
>
> Es un nodo IoT de bajo costo con un sensor de color, el TCS34725, y uno de gases, el MQ-135. El color de la cáscara nos dice la etapa de maduración, de E1 a E4. Un modelo estima esa etapa y un modelo de lenguaje recomienda el destino del lote, que se ve en un dashboard.
>
> Nuestra meta, para el 16 de diciembre, es una demostración en vivo de punta a punta que acierte la etapa en al menos el 85 % de bananos nuevos, con una etapa de tolerancia.
>
> ¿Dónde vamos? **El nodo sensor ya está configurado y probado, y las interfaces, dashboard, historial y chatbot, ya están terminadas.** Falta lo más delicado: el dataset de 25 bananos con doble etiquetado, el clasificador y la lógica del modelo de lenguaje. Eso es lo que el EVM nos ayuda a vigilar.

**Línea base (0:35)** — [Diap. 2]

> Para medir hay que tener un plan. Nuestra línea base es de **215 horas-persona**, repartidas en 20 filas de actividades y siete frentes, desde hardware hasta integración y pruebas. Cada actividad tiene horas, fechas y un criterio de aceptación escrito. Medimos en horas porque es el esfuerzo que realmente controlamos.
>
> A ese presupuesto total, las 215 horas, el EVM lo llama BAC. Con esto, Jorman les explica de dónde salen los tres datos con los que medimos.

### 4.2 Jorman — Panorama: de dónde salen el PV, el EV y el AC (2:30)

**Tres variables y cálculo del PV (1:00)** — [Diap. 3, luego 15]

> Gracias, John. Soy Jorman. El EVM compara tres números. El primero es el **PV, el valor planificado**: lo que el plan dice que ya deberíamos haber hecho a una fecha.
>
> Lo calculamos repartiendo las horas de cada actividad de forma lineal entre su fecha de inicio y de fin, y sumando todas. Un ejemplo: la actividad A07.1 tiene **13 horas entre el 30 de octubre y el 8 de noviembre**, diez días. El 1 de noviembre han pasado tres, así que su PV es 13 por 3 entre 10, **3,9 horas**. Sumando las 20 actividades, **al 15 de noviembre el PV es de 72 horas**.
>
> Les decimos con honestidad que el reparto lineal es una simplificación: el trabajo real no avanza parejo. Pero nos da una referencia clara y la misma para todos los cortes.

**EV: qué se aceptó y con qué evidencia (0:55)** — [Diap. 16]

> El segundo es el **EV, el valor ganado**: cuánto del plan está realmente terminado y aceptado. Usamos la regla cero-cien: si una actividad cumple su criterio de aceptación, gana el 100 % de sus horas; si no, gana cero, aunque esté al 90 %. Así evitamos decir «vamos al 90 %» cuando no hay nada terminado.
>
> En la tabla están las actividades aceptadas, cada una con su fecha, su evidencia y quién la aceptó. Por ejemplo, la prueba de siete días del nodo: **9.826 de 10.080 tomas, el 97,5 %**, que cumple el criterio de 95 %. Suman **85 horas**. Ojo: la guía de etiquetado, los bananos y el primer tramo del dataset **no suman**, aunque ya tienen trabajo, porque todavía no están aceptados.

**AC: horas por persona y semana (0:35)** — [Diap. 17]

> El tercero es el **AC, el costo real**: lo que realmente hemos gastado, en horas-persona. Cada uno registra fecha, actividad y horas, y una hoja suma ese registro por persona y por semana: **46 horas la primera semana, 38 la segunda y 28 la tercera, 112 en total**.
>
> Por frente: hardware **52 horas, con 40 planificadas**; dataset 20; interfaces 40. Con estos tres datos, el PV, el EV y el AC, calculamos todo lo demás. Daniel les muestra cómo.

### 4.3 Daniel — Métricas: desviaciones, curva S y SPI (2:30)

**Desviaciones e índices (0:40)** — [Diap. 4]

> Gracias, Jorman. Soy Daniel, el gerente del proyecto. Con PV, EV y AC calculamos dos desviaciones y dos índices. Las desviaciones dicen **cuánto** nos alejamos del plan, en horas: la desviación de cronograma, SV, es EV menos PV; y la de costo, CV, es EV menos AC. Los índices dicen **qué tan eficientes** somos, y el uno es el plan: el SPI, valor ganado entre valor planificado, mira el cronograma; el CPI, valor ganado entre costo real, mira el costo, que aquí es esfuerzo en horas.

**Curva S (1:05)** — [Diap. 5]

> Esta es la curva S: las horas acumuladas a lo largo del tiempo, que se llama así por su forma, lenta al inicio, rápida en el medio y lenta al final. La verde es el plan, el PV: arranca fuerte, con **54 horas en la primera semana**, y llega a 215 el 16 de diciembre. La dorada es el valor ganado, el EV, y la azul el costo real, el AC, con un corte cada semana.
>
> Lo que se ve: en las dos primeras semanas el valor ganado estuvo por debajo del plan, **32 horas contra 53,9 el 1 de noviembre**. El 15 de noviembre lo supera: **85 contra 72**, porque aceptamos las interfaces antes de lo planificado. Pero miren la azul: siempre está por encima de la dorada. **Gastamos 112 horas para ganar 85.** Esa brecha es la que vamos a analizar.

**SPI (0:45)** — [Diap. 6, luego 7]

> Aquí están los tres cortes cerrados, con sus datos y sus cálculos. El SPI mide si vamos al ritmo del cronograma: valor ganado entre valor planificado. Al 15 de noviembre: **85 entre 72, igual a 1,18**. Eso es verde.
>
> En palabras: a esa fecha hicimos un 18 % más de lo que el plan esperaba. Estamos **adelantados**: la desviación de cronograma es **más 13 horas**. Y miren la evolución: **0,59, 0,83 y 1,18**. Empezamos en rojo y llegamos a verde. Pero el SPI solo mira el cronograma, no cuánto nos costó. Edward les cuenta qué pasa con el costo.

### 4.4 Edward — CPI, semáforo y dónde estamos (2:10)

**CPI (0:50)** — [Diap. 7]

> Gracias, Daniel. Soy Edward. El CPI mide qué tan bien rinde cada hora que invertimos, y es independiente del SPI: se puede ir rápido y caro, o lento y barato. Es valor ganado entre costo real: **85 entre 112, igual a 0,76**. Significa que por cada hora que gastamos producimos **0,76 horas de valor aceptado**. Somos ineficientes: la desviación de costo es **menos 27 horas**, gastamos 27 horas más de las que ganamos.
>
> Y no es nuevo: **0,70, 0,62 y 0,76**, tres cortes seguidos en rojo. Si seguimos con esta eficiencia, el pronóstico, que es el presupuesto entre el CPI, 215 entre 0,76, da **unas 283 horas**: casi 70 horas más que el plan.

**Semáforo RAG (0:40)** — [Diap. 8]

> Para no interpretar cada número a ojo usamos el semáforo RAG, de rojo, ámbar y verde, que dice qué tan urgente es actuar. Se aplica al SPI y al CPI por separado. **Verde**: 0,95 o más, solo se monitorea. **Ámbar**: entre 0,85 y 0,94, hay que revisar causas. **Rojo**: menos de 0,85, intervención inmediata.
>
> Nuestro SPI de 1,18 es **verde**. Nuestro CPI de 0,76 es **rojo**. Y cuando cualquiera de los dos baja de 0,90, hay que documentar una acción correctiva; la del CPI la presentamos en un momento.

**¿Dónde estamos? (0:40)** — [Diap. 9]

> Entonces, ¿dónde estamos? Combinando los dos índices hay cuatro cuadrantes: adelantados y eficientes, que es el ideal; atrasados y gastando de más, que es el peor; y dos intermedios. Respondamos las dos preguntas. ¿Adelantados o atrasados? **Adelantados**: el SPI es mayor que uno. ¿Por encima o por debajo del presupuesto? **Por encima**: el CPI es menor que uno, estamos gastando más de lo que producimos. Es el cuadrante de arriba a la izquierda: adelantados, pero gastando de más.
>
> Pero un SPI verde no siempre significa que todo va bien. Avalo les explica por qué.

### 4.5 Avalo — Mitigación: el atraso escondido, acciones y decisión (2:35)

**El SPI verde esconde un atraso (0:45)** — [Diap. 10]

> Gracias, Edward. Soy Avalo. El SPI es un promedio, y aquí promedia dos cosas opuestas. Hardware va al plan: **40 de 40 horas**. Las interfaces suman **45 horas ganadas**, aunque el plan las pone en diciembre. Y el dataset, que es lo que ya debería estar hecho: **0 de 32 horas**, con **20 horas gastadas**.
>
> La guía de etiquetado, los 25 bananos y el primer tramo de sesiones siguen sin aceptación. Y esto es crítico: el hito H3, el dataset con doble etiquetado, es el 18 de noviembre, y el clasificador depende de él. Si miráramos solo el SPI, concluiríamos que vamos bien, y no es así.

**Plan de acciones correctivas (1:20)** — [Diap. 11]

> Tenemos tres acciones, y cada una sale de un dato.
>
> **Uno.** El CPI de 0,76 viene en parte de 20 horas de dataset sin valor ganado, porque la guía y los bananos no están aceptados. Los cerramos esta semana: la guía validada por el bodeguero y los 25 bananos registrados. Yo respondo, para el **20 de noviembre**.
>
> **Dos.** El dataset va en 0 de 32 horas. Daniel congela el frontend desde el **17 de noviembre**, pasa esas horas al dataset y consulta al patrocinador si el hito H3 puede moverse al 25 de noviembre.
>
> **Tres.** El pronóstico es de 283 horas contra 215. Ponemos tope de horas a los modelos; y si el CPI sigue bajo 0,85 el **22 de noviembre**, entrenamos un solo modelo en lugar de dos.
>
> Usamos así las tres palancas: la forma de trabajo, la fecha y el alcance. La meta del 22 es tener aceptadas la guía, los bananos y el primer tramo de sesiones, **23 horas más de valor ganado**, con un CPI de **0,80 o más**. Eso exige que esa semana no pase de 23 horas de trabajo.

**Qué decidimos hoy (0:30)** — [Diap. 12]

> En resumen: estamos **adelantados** en cronograma, con un SPI de 1,18, pero **gastando de más**, con un CPI de 0,76, y el pronóstico es de 283 horas contra 215. Decidimos cerrar la guía y los bananos, congelar el frontend y poner tope de horas a los modelos. Volvemos a medir el **22 de noviembre**. Muchas gracias; quedamos atentos a sus preguntas.

---

## 5. Preguntas probables y quién responde

Cualquiera puede ser interpelado (la rúbrica evalúa al equipo), así que todos deben saber lo básico. Primera respuesta sugerida:

| Pregunta | Responde | Dónde está el respaldo |
|---|---|---|
| ¿De dónde sale el plan y las 215 h? | John | Anexo, diap. 14 |
| ¿Cómo calcularon el PV? ¿Por qué lineal? | Jorman | Anexo, diap. 15 |
| ¿Por qué esa actividad cuenta como terminada? ¿Qué pasa con una al 90 %? | Jorman | Anexo, diap. 16 |
| ¿De dónde salen las horas? ¿Quién las registra? | Jorman | Anexo, diap. 17 |
| ¿Qué diferencia hay entre SV y SPI, o entre CV y CPI? | Daniel | Diap. 4 |
| ¿Por qué el SPI sube si el CPI está en rojo? | Daniel y Avalo | Diap. 10 |
| ¿Qué significa un CPI de 0,76? | Edward | Diap. 7 |
| ¿Por qué esos umbrales (0,95 y 0,85)? | Edward | Codelab: son umbrales de uso habitual |
| ¿Qué pasa si no cumplen H3? | Avalo | Diap. 11, acción 2 |
| ¿Qué hacen si el CPI no mejora el 22-nov? | Daniel | Diap. 11, acción 3 |
| ¿Qué falta por hacer? | Avalo | Anexo, diap. 18 |

## 6. Para ensayar

1. **Cronometren con las diapositivas**, no solo leyendo. Lo normal es que se pase de tiempo en la curva S y en el plan de acciones.
2. **Cada uno memoriza sus cifras clave:** Jorman (72, 85, 112, 9.826/10.080), Daniel (1,18; 0,59→0,83→1,18; +13), Edward (0,76; −27; 283), Avalo (40/40, 0/32, 20 h, 23 h, 0,80).
3. **No lean las fórmulas, expliquen qué mide cada una.** La rúbrica premia «entendimiento conceptual y no solo la repetición de fórmulas».
4. **Un relevo limpio:** la última frase de cada persona nombra a quien sigue (ya está en el guion).
5. **Si falta tiempo**, recorten en este orden: (a) el ejemplo de A07.1 en el PV, (b) la evolución 0,70 → 0,62 → 0,76 del CPI, (c) la frase del pronóstico de 283 horas. No recorten ninguna definición ni ninguna acción.
6. **Si sobra tiempo**, cuenten en una frase cómo surgió la idea (John) o den un ejemplo de cómo se aceptó una actividad (Jorman).

## 7. Qué cambia cuando pasen a los datos reales

- Las cifras en negrita: PV, EV, AC, los tres cortes, el SPI y el CPI, la evidencia de cada actividad aceptada, las horas por persona, las 20 h del dataset y las acciones con responsable y fecha.
- Si el SPI real sale en rojo o ámbar, **el guion de Avalo cambia**: la diapositiva 10 («El SPI verde esconde un atraso») ya no aplica. La reemplazan por el desglose por frente igual, pero con el mensaje que salga de sus datos.
- Si el CPI real sale verde, la sección de acciones se centra en el SPI. La rúbrica pide acciones para «los problemas detectados por las métricas»: si no hay problemas, hay que explicar por qué y qué riesgo vigilan.
- Cuenten las palabras otra vez. Si el guion nuevo es más largo, recorten antes de ensayar.
