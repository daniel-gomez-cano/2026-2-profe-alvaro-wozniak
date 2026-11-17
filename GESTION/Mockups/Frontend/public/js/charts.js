/** Gráfica de línea "color en el tiempo" reutilizable (Inicio e Historial). */
(function () {
  const Bananin = (window.Bananin = window.Bananin || {});
  const instances = new WeakMap();
  const registry = new Map(); // canvas -> registros, para redibujar al cambiar de tema

  Bananin.charts = {
    colorEnElTiempo(canvas, registros) {
      if (!window.Chart || !canvas) return;
      registry.set(canvas, registros);
      const css = getComputedStyle(document.documentElement);
      const v = (n) => css.getPropertyValue(n).trim();
      const line = v("--bn-chart-line"), grid = v("--bn-line"), muted = v("--bn-muted"), surface = v("--bn-surface");
      const dark = document.documentElement.getAttribute("data-bs-theme") === "dark";
      const data = [...registros].sort((a, b) => new Date(a.fecha) - new Date(b.fecha));
      const labels = data.map((r) => `${Bananin.fmt.diaCorto(r.fecha)} ${Bananin.fmt.hora(r.fecha)}`);
      const values = data.map((r) => r.rgb);

      const prev = instances.get(canvas);
      if (prev) prev.destroy();

      const ctx = canvas.getContext("2d");
      const grad = ctx.createLinearGradient(0, 0, 0, canvas.clientHeight || 220);
      grad.addColorStop(0, dark ? "rgba(242,194,48,.35)" : "rgba(242,194,48,.40)");
      grad.addColorStop(1, "rgba(242,194,48,0)");

      instances.set(canvas, new Chart(ctx, {
        type: "line",
        data: {
          labels,
          datasets: [{
            data: values, borderColor: line, backgroundColor: grad, fill: true,
            tension: 0.35, borderWidth: 2.5, pointRadius: 3, pointHoverRadius: 6, pointBackgroundColor: surface, pointBorderColor: line,
          }],
        },
        options: {
          responsive: true, maintainAspectRatio: false,
          plugins: {
            legend: { display: false },
            tooltip: { callbacks: { label: (c) => ` RGB: ${Bananin.fmt.num(c.parsed.y)}` } },
          },
          scales: {
            x: { grid: { display: false }, ticks: { maxTicksLimit: 5, color: muted, font: { size: 11 } } },
            y: { grid: { color: grid }, ticks: { color: muted, font: { size: 11 }, callback: (v) => Bananin.fmt.num(v) } },
          },
        },
      }));
    },
  };

  document.addEventListener("bananin:theme", () => {
    registry.forEach((datos, canvas) => Bananin.charts.colorEnElTiempo(canvas, datos));
  });
})();
