/** Pantalla Historial. */
(function () {
  const { api, fmt, charts } = window.Bananin;
  const $ = (id) => document.getElementById(id);
  const esc = (s) => String(s).replace(/[&<>"']/g, (c) => ({ "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;", "'": "&#39;" }[c]));

  function agrupar(registros) {
    const grupos = new Map();
    [...registros]
      .sort((a, b) => new Date(b.fecha) - new Date(a.fecha))
      .forEach((r) => {
        const k = fmt.dia(r.fecha);
        if (!grupos.has(k)) grupos.set(k, []);
        grupos.get(k).push(r);
      });
    return grupos;
  }

  function pintarLista(registros) {
    const cont = $("hist-list");
    if (!registros.length) {
      cont.innerHTML = '<p class="text-secondary">Aún no hay lecturas registradas.</p>';
      return;
    }
    let html = "";
    for (const [dia, items] of agrupar(registros)) {
      html += `<div class="hist-day">Día: ${esc(dia)}</div>`;
      html += items
        .map((r) => `<div class="hist-row">
            <span class="dot" style="background:${esc(r.color || "#e0b92d")}"></span>
            <span class="time">${fmt.hora(r.fecha)}</span>
            <span>RGB : <strong>${fmt.num(r.rgb)}</strong></span></div>`)
        .join("");
    }
    cont.innerHTML = html;
  }

  async function descargar() {
    const btn = $("btn-informe"), msg = $("informe-msg");
    btn.disabled = true; msg.textContent = "Generando informe…";
    try {
      const { blob, filename } = await api.descargarReporte();
      const url = URL.createObjectURL(blob);
      const a = Object.assign(document.createElement("a"), { href: url, download: filename });
      document.body.appendChild(a); a.click(); a.remove();
      setTimeout(() => URL.revokeObjectURL(url), 1000);
      msg.textContent = "Informe descargado ✓";
    } catch (e) {
      console.error(e);
      msg.textContent = "No se pudo generar el informe. Inténtalo de nuevo.";
    } finally {
      btn.disabled = false;
    }
  }

  async function init() {
    $("btn-informe").addEventListener("click", descargar);
    try {
      const hist = await api.getHistorial({ limit: 200 });
      pintarLista(hist);
      charts.colorEnElTiempo($("chart"), hist);
    } catch (e) {
      console.error(e);
      $("hist-list").innerHTML = "";
      const box = $("error");
      box.textContent = "No se pudo cargar el historial desde el servidor.";
      box.classList.remove("d-none");
    }
  }
  init();
})();
