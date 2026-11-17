/**
 * Capa de acceso al backend. Toda la app habla con el servidor SOLO a través de
 * window.Bananin.api. Para cambiar rutas o formato de datos, edita este archivo.
 *
 * Endpoints esperados (ver README.md para los JSON):
 *   GET  /api/lectura/actual
 *   GET  /api/estado
 *   GET  /api/historial?limit=&desde=&hasta=
 *   GET  /api/historial/reporte        -> archivo (CSV/PDF)
 *   POST /api/chat                     -> { mensaje, conversacion_id }
 */
(function () {
  const cfg = window.BANANIN_CONFIG || {};
  const Bananin = (window.Bananin = window.Bananin || {});
  const state = { demo: false };

  async function http(path, opts = {}) {
    if (cfg.FORCE_MOCK) throw new Error("FORCE_MOCK");
    const ctrl = new AbortController();
    const timer = setTimeout(() => ctrl.abort(), cfg.TIMEOUT_MS || 6000);
    const headers = { "Content-Type": "application/json" };
    if (cfg.AUTH_TOKEN) headers.Authorization = "Bearer " + cfg.AUTH_TOKEN;
    try {
      const res = await fetch((cfg.API_BASE_URL || "") + path, { ...opts, headers, signal: ctrl.signal });
      if (!res.ok) throw new Error("HTTP " + res.status);
      return res;
    } finally {
      clearTimeout(timer);
    }
  }

  function setDemo(on) {
    if (state.demo !== on) {
      state.demo = on;
      document.dispatchEvent(new CustomEvent("bananin:demo", { detail: on }));
    }
  }

  async function call(path, opts, mockFn, parse = (r) => r.json()) {
    try {
      const res = await http(path, opts);
      setDemo(false);
      return await parse(res);
    } catch (err) {
      if (!cfg.ALLOW_MOCK_FALLBACK) throw err;
      setDemo(true);
      return mockFn();
    }
  }

  /* ---------------- Datos de demostración ---------------- */
  const ETAPAS = ["Verde", "Pintón", "Maduro", "Sobremaduro"];
  const rand = (a, b) => Math.round(a + Math.random() * (b - a));
  const mockHist = (() => {
    const out = [];
    const now = Date.now();
    for (let i = 0; i < 14; i++) {
      const t = new Date(now - i * 4.2 * 3600 * 1000);
      out.push({ id: i + 1, fecha: t.toISOString(), rgb: 51000 + Math.round((14 - i) * 450 + Math.random() * 900), color: null });
    }
    return out;
  })();

  const mock = {
    lectura: () => ({ r: rand(210, 218), g: 190, b: 60, color: "#d6be3c", timestamp: new Date(Date.now() - 12000).toISOString() }),
    estado: () => ({
      etapa: 3, etapas_total: 4, etapa_nombre: ETAPAS[2], dias_restantes: 3,
      destino: "Vender local", validado_por_reglas: true,
      justificacion: "El banano está en etapa 3 y le quedan unos 3 días de vida útil. Un transporte largo supera ese tiempo, por lo que se recomienda venderlo en el mercado local.",
      confianza: 82, valor_estimado: null
    }),
    historial: (limit) => mockHist.slice(0, limit || 50),
    chat: (msg) => {
      const m = (msg || "").toLowerCase();
      let r = "Puedo ayudarte con el transporte, el destino de distribución y el mejor momento de venta según el estado de tu fruta. ¿Sobre cuál quieres saber?";
      if (/hola|hello|buenas/.test(m)) r = "¡Hola! 🍌 Cuéntame, ¿quieres saber qué hacer con tu banano hoy?";
      else if (/transport|enviar|lejos/.test(m)) r = "Con ≈ 3 días de vida útil, un transporte largo no es recomendable. Te sugiero **vender en el mercado local**.";
      else if (/vend|precio|cuándo|cuando/.test(m)) r = "El banano está en **etapa 3 de 4 (Maduro)**. Lo mejor es venderlo en los próximos 2 días para no perder valor.";
      return { respuesta: r, conversacion_id: "demo" };
    }
  };

  const csv = (rows) =>
    "﻿fecha,rgb\n" + rows.map((r) => `${r.fecha},${r.rgb}`).join("\n");

  /* ---------------- API pública ---------------- */
  Bananin.api = {
    get isDemo() { return state.demo; },

    getLecturaActual: () => call("/api/lectura/actual", {}, mock.lectura),

    getEstado: () => call("/api/estado", {}, mock.estado),

    getHistorial: ({ limit = 50, desde, hasta } = {}) => {
      const q = new URLSearchParams({ limit });
      if (desde) q.set("desde", desde);
      if (hasta) q.set("hasta", hasta);
      return call("/api/historial?" + q, {}, () => mock.historial(limit));
    },

    /** Descarga el informe. Devuelve { blob, filename } */
    async descargarReporte() {
      return call(
        "/api/historial/reporte", {},
        () => ({ blob: new Blob([csv(mockHist)], { type: "text/csv;charset=utf-8" }), filename: "informe-bananin.csv" }),
        async (res) => {
          const cd = res.headers.get("Content-Disposition") || "";
          const m = /filename="?([^";]+)"?/i.exec(cd);
          return { blob: await res.blob(), filename: m ? m[1] : "informe-bananin.csv" };
        }
      );
    },

    enviarMensaje: (mensaje, conversacion_id = null) =>
      call("/api/chat", { method: "POST", body: JSON.stringify({ mensaje, conversacion_id }) }, () => mock.chat(mensaje)),
  };
})();
