# Bananín · Frontend web

HTML + CSS + JavaScript puro, con **Bootstrap 5** (layout/componentes), **Bootstrap Icons** y **Chart.js** (gráficas), todo por CDN: no hay paso de compilación.

## Probar
```bash
node server.js        # http://localhost:3000
```
Si abres `public/index.html` directamente (sin backend) la app usa **datos de demostración** y lo indica con una pastilla amarilla.

## Estructura
```
public/
  index.html · historial.html · bananin.html   (3 pantallas)
  css/styles.css                               (tema: colores, tipografía, componentes)
  js/config.js    URL del backend y opciones
  js/api.js       ÚNICO punto de contacto con el backend (+ datos demo)
  js/layout.js    sidebar, formatos de fecha/número
  js/charts.js    gráfica "color en el tiempo"
  js/inicio.js · historial.js · chat.js        (lógica de cada pantalla)
server.js         backend de ejemplo sin dependencias
```

## Conectar tu backend
1. En `public/js/config.js` pon `API_BASE_URL` (`""` si el backend sirve también los archivos; `"https://api.tudominio.com"` si es aparte, habilitando CORS).
2. Pon `ALLOW_MOCK_FALLBACK: false` en producción para no mostrar datos de demostración si falla el servidor.
3. Implementa estos endpoints (o ajusta las rutas en `js/api.js`):

| Método | Ruta | Respuesta |
|---|---|---|
| GET | `/api/lectura/actual` | `{ "r": 214, "g": 190, "b": 60, "color": "#d6be3c", "timestamp": "2026-10-05T15:00:00Z" }` |
| GET | `/api/estado` | `{ "etapa": 3, "etapas_total": 4, "etapa_nombre": "Maduro", "dias_restantes": 3, "destino": "Vender local", "validado_por_reglas": true, "justificacion": "...", "confianza": 82, "valor_estimado": null }` |
| GET | `/api/historial?limit=50` | `[ { "id": 1, "fecha": "2026-10-05T16:45:00Z", "rgb": 56234, "color": "#e0b92d" } ]` |
| GET | `/api/historial/reporte` | Archivo (CSV/PDF) con `Content-Disposition: attachment; filename="..."` |
| POST | `/api/chat` | Body `{ "mensaje": "...", "conversacion_id": null }` → `{ "respuesta": "texto (admite **negrita**)", "conversacion_id": "abc" }` |

Notas: `fecha` en ISO 8601 (UTC); `color` es opcional; `valor_estimado` puede ser `null` (se muestra "—"). Para autenticación pon `AUTH_TOKEN` en config (se envía como `Authorization: Bearer`).
