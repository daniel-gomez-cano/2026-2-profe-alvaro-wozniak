## 1. Plan de Calidad




| Atributo | Estandar | Cómo se verifica | Quién y cuándo | Si falla |
|---|---|---|---|---|
| Corrección funcional | Cada requisito cumple su criterio de aceptación | Pruebas automáticas del criterio | Quien programa, antes de abrir el Pull Request | No se mezcla |
| Tiempo de respuesta | Proceso completo de lectura del banano no superior a 5 segundos | Pruebas de diagnostico del proceso | Daniel Goméz Cano, en el momento de trabajar los datos | Se registra como riesgo y se decide |
| Presición de los modelos | Precisión de los modelos deterministicos sea de un 80% | Pruebas automaticas del criterio | Santiago Avalo Monsalve, en el momento de crear el dataset/modelo determinista | Se registra como riesgo y se decide |



## 2. Definición de terminado

### IOT
- Que se entreguen datos del sensor en un csv a la nube, organizados por columnas (id, estado, dias_estimados, org_vol, og_vol,norm,, R, G, B, C, r_norm, g_norm, b_norm)

### Nube
- Que se entreguen los datos a los modelos no deterministicos.

### Clasificación/Regresión
- Que se entreguen los datos del resultado del modelo al LLM.

### LLM
- Que se entregue al frontend una respuesta que incluya el estado de maduración del banano, dias restantes y la desición de destino optimo para el banano.

### Frontend




