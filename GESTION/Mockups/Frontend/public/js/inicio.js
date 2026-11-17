/** Pantalla Inicio (dashboard). */
(function () {
  const { api, fmt, charts } = window.Bananin;
  const $ = (id) => document.getElementById(id);
  const unskel = (...els) => els.forEach((e) => e.classList.remove("skeleton"));
  const STAGE_COLORS = ["#8fb339", "#d4c339", "#e0b92d", "#a8741a"];
  let lecturaTs = null;

  function pintarLectura(l) {
    lecturaTs = l.timestamp;
    $("lectura").textContent = `R ${l.r}`;
    $("swatch").style.background = l.color || `rgb(${l.r},${l.g ?? 180},${l.b ?? 60})`;
    unskel($("lectura"), $("swatch"));
    tick();
  }
  const tick = () => { if (lecturaTs) $("lectura-hace").textContent = fmt.hace(lecturaTs); };

  function pintarEstado(e) {
    $("etapa").textContent = `Etapa ${e.etapa} de ${e.etapas_total}`;
    [...$("stage-bar").children].forEach((s, i) => s.classList.toggle("on", i < e.etapa));
    $("etapa-nombre").textContent = e.etapa_nombre;
    $("dias").textContent = `≈ ${e.dias_restantes} ${e.dias_restantes === 1 ? "día" : "días"}`;
    $("destino").textContent = e.destino;
    $("regla").classList.toggle("d-none", !e.validado_por_reglas);
    $("justificacion").textContent = e.justificacion;
    $("confianza").textContent = `Confianza: ${e.confianza} %`;
    $("valor").textContent = `Valor estimado: ${fmt.money(e.valor_estimado)} COP/kg`;
    unskel($("etapa"), $("dias"), $("destino"), $("justificacion"));
  }

  async function init() {
    try {
      const [lectura, estado, hist] = await Promise.all([
        api.getLecturaActual(), api.getEstado(), api.getHistorial({ limit: 12 }),
      ]);
      pintarLectura(lectura);
      pintarEstado(estado);
      charts.colorEnElTiempo($("mini-chart"), hist);
    } catch (err) {
      console.error(err);
      const box = $("error");
      box.textContent = "No se pudo conectar con el servidor. Verifica tu conexión e inténtalo de nuevo.";
      box.classList.remove("d-none");
    }
    setInterval(tick, 1000);
    setInterval(async () => {
      try { pintarLectura(await api.getLecturaActual()); } catch (_) { /* siguiente ciclo */ }
    }, window.BANANIN_CONFIG.REFRESH_LECTURA_MS || 10000);
  }
  init();
})();
