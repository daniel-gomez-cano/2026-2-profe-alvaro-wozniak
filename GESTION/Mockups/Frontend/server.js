/**
 * Servidor de EJEMPLO (sin dependencias): sirve /public y expone la API que
 * espera el frontend. Reemplaza los datos simulados por tu sensor, base de
 * datos y LLM reales; el contrato JSON (README.md) debe mantenerse.
 *
 *   node server.js   ->  http://localhost:3000
 */
const http = require("http");
const fs = require("fs");
const path = require("path");

const PORT = process.env.PORT || 3000;
const PUBLIC = path.join(__dirname, "public");
const MIME = { ".html": "text/html; charset=utf-8", ".js": "text/javascript", ".css": "text/css", ".png": "image/png", ".svg": "image/svg+xml", ".ico": "image/x-icon" };

// ---- Datos simulados ----
const historial = Array.from({ length: 14 }, (_, i) => ({
  id: i + 1,
  fecha: new Date(Date.now() - i * 4.2 * 3600e3).toISOString(),
  rgb: 51000 + Math.round((14 - i) * 450 + Math.random() * 900),
  color: null,
}));

const estado = () => ({
  etapa: 3, etapas_total: 4, etapa_nombre: "Maduro", dias_restantes: 3,
  destino: "Vender local", validado_por_reglas: true,
  justificacion: "El banano está en etapa 3 y le quedan unos 3 días de vida útil. Un transporte largo supera ese tiempo, por lo que se recomienda venderlo en el mercado local.",
  confianza: 82, valor_estimado: null,
});

const lectura = () => ({ r: 210 + Math.round(Math.random() * 8), g: 190, b: 60, color: "#d6be3c", timestamp: new Date(Date.now() - 12000).toISOString() });

function chat(msg) {
  const m = (msg || "").toLowerCase();
  if (/transport|lejos|enviar/.test(m)) return "Con ≈ 3 días de vida útil, un transporte largo no es recomendable. Te sugiero **vender en el mercado local**.";
  if (/vend|precio|cu[aá]ndo/.test(m)) return "El banano está en **etapa 3 de 4 (Maduro)**. Lo mejor es venderlo en los próximos 2 días.";
  if (/tiempo|d[ií]as|queda/.test(m)) return "Le quedan aproximadamente **3 días** de vida útil.";
  return "¡Hola! 🍌 Puedo ayudarte con transporte, destino y momento de venta. ¿Qué necesitas?";
}

// ---- Utilidades ----
const json = (res, code, body) => {
  res.writeHead(code, { "Content-Type": "application/json; charset=utf-8", "Access-Control-Allow-Origin": "*" });
  res.end(JSON.stringify(body));
};
const readBody = (req) => new Promise((ok) => { let d = ""; req.on("data", (c) => (d += c)); req.on("end", () => ok(d)); });

http.createServer(async (req, res) => {
  const url = new URL(req.url, "http://localhost");

  if (req.method === "OPTIONS") {
    res.writeHead(204, { "Access-Control-Allow-Origin": "*", "Access-Control-Allow-Headers": "Content-Type, Authorization", "Access-Control-Allow-Methods": "GET,POST,OPTIONS" });
    return res.end();
  }

  // ---- API ----
  if (url.pathname === "/api/lectura/actual") return json(res, 200, lectura());
  if (url.pathname === "/api/estado") return json(res, 200, estado());
  if (url.pathname === "/api/historial") {
    const limit = Math.min(+url.searchParams.get("limit") || 50, 500);
    return json(res, 200, historial.slice(0, limit));
  }
  if (url.pathname === "/api/historial/reporte") {
    const csv = "﻿fecha,rgb\n" + historial.map((r) => `${r.fecha},${r.rgb}`).join("\n");
    res.writeHead(200, { "Content-Type": "text/csv; charset=utf-8", "Content-Disposition": 'attachment; filename="informe-bananin.csv"', "Access-Control-Allow-Origin": "*", "Access-Control-Expose-Headers": "Content-Disposition" });
    return res.end(csv);
  }
  if (url.pathname === "/api/chat" && req.method === "POST") {
    try {
      const { mensaje, conversacion_id } = JSON.parse((await readBody(req)) || "{}");
      return json(res, 200, { respuesta: chat(mensaje), conversacion_id: conversacion_id || "c-" + Date.now() });
    } catch { return json(res, 400, { error: "JSON inválido" }); }
  }
  if (url.pathname.startsWith("/api/")) return json(res, 404, { error: "No encontrado" });

  // ---- Estáticos ----
  let file = path.normalize(path.join(PUBLIC, url.pathname === "/" ? "index.html" : url.pathname));
  if (!file.startsWith(PUBLIC)) { res.writeHead(403); return res.end(); }
  fs.readFile(file, (err, data) => {
    if (err) { res.writeHead(404); return res.end("404"); }
    res.writeHead(200, { "Content-Type": MIME[path.extname(file)] || "application/octet-stream" });
    res.end(data);
  });
}).listen(PORT, () => console.log(`Bananín en http://localhost:${PORT}`));
