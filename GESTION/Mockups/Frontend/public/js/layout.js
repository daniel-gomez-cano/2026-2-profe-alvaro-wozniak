/** Sidebar compartido, utilidades de formato y aviso de modo demo. */
(function () {
  const Bananin = (window.Bananin = window.Bananin || {});

  const MESES = ["ene", "feb", "mar", "abr", "may", "jun", "jul", "ago", "sep", "oct", "nov", "dic"];
  Bananin.fmt = {
    num: (n) => new Intl.NumberFormat("en-US").format(n),
    hora: (d) => {
      d = new Date(d);
      let h = d.getHours();
      const ampm = h >= 12 ? "pm" : "am";
      h = h % 12 || 12;
      return `${h}:${String(d.getMinutes()).padStart(2, "0")} ${ampm}`;
    },
    dia: (d) => {
      d = new Date(d);
      return `${d.getDate()}/${MESES[d.getMonth()]}/${d.getFullYear()}`;
    },
    diaCorto: (d) => {
      d = new Date(d);
      return `${d.getDate()} ${MESES[d.getMonth()]}`;
    },
    hace: (iso) => {
      const s = Math.max(0, Math.round((Date.now() - new Date(iso)) / 1000));
      if (s < 60) return `hace ${s} s`;
      if (s < 3600) return `hace ${Math.floor(s / 60)} min`;
      return `hace ${Math.floor(s / 3600)} h`;
    },
    money: (n) => (n == null ? "—" : "$ " + new Intl.NumberFormat("es-CO").format(n)),
  };

  // Tema claro/oscuro (se guarda en localStorage; el valor inicial lo pone un script en <head>)
  Bananin.theme = {
    get: () => document.documentElement.getAttribute("data-bs-theme") === "dark" ? "dark" : "light",
    set(t) {
      document.documentElement.setAttribute("data-bs-theme", t);
      try { localStorage.setItem("bananin.theme", t); } catch (_) {}
      document.dispatchEvent(new CustomEvent("bananin:theme", { detail: t }));
    },
  };

  const NAV = [
    { href: "index.html", id: "inicio", label: "Inicio", icon: "bi-grid-1x2" },
    { href: "historial.html", id: "historial", label: "Historial", icon: "bi-clock-history" },
    { href: "bananin.html", id: "bananin", label: "Bananin", icon: "bi-chat-dots" },
  ];

  document.addEventListener("DOMContentLoaded", () => {
    const active = document.body.dataset.page;
    const aside = document.getElementById("sidebar");
    if (aside) {
      aside.innerHTML =
        `<div class="brand"><div class="brand-logo">🍌</div><span class="brand-name">Bananín</span></div>` +
        NAV.map(
          (n) =>
            `<a class="nav-link-bn ${n.id === active ? "active" : ""}" href="${n.href}" ${n.id === active ? 'aria-current="page"' : ""}>
               <i class="bi ${n.icon}"></i>${n.label}</a>`
        ).join("") +
        `<button type="button" id="theme-switch" class="theme-switch" role="switch" aria-checked="false">
           <span class="ts-label"><i class="bi bi-moon-stars"></i><span id="ts-text">Modo oscuro</span></span>
           <span class="ts-track"><span class="ts-thumb"><i class="bi bi-sun-fill"></i></span></span>
         </button>`;

      const sw = document.getElementById("theme-switch");
      const paint = () => {
        const dark = Bananin.theme.get() === "dark";
        sw.setAttribute("aria-checked", dark);
        sw.querySelector(".ts-thumb i").className = "bi " + (dark ? "bi-moon-fill" : "bi-sun-fill");
        sw.querySelector(".ts-label i").className = "bi " + (dark ? "bi-moon-stars" : "bi-sun");
        document.getElementById("ts-text").textContent = dark ? "Modo oscuro" : "Modo claro";
      };
      sw.addEventListener("click", () => { Bananin.theme.set(Bananin.theme.get() === "dark" ? "light" : "dark"); paint(); });
      paint();

      const btn = document.createElement("button");
      btn.className = "menu-toggle";
      btn.setAttribute("aria-label", "Abrir menú");
      btn.innerHTML = '<i class="bi bi-list fs-4"></i>';
      btn.onclick = () => aside.classList.toggle("open");
      document.body.appendChild(btn);
      document.addEventListener("click", (e) => {
        if (!aside.contains(e.target) && !btn.contains(e.target)) aside.classList.remove("open");
      });
    }

    // Aviso "datos de demostración" junto a la pastilla "Gestión de Banano"
    const group = document.querySelector(".pill-group");
    document.addEventListener("bananin:demo", (e) => {
      let tag = document.getElementById("demo-pill");
      if (e.detail && group && !tag) {
        tag = document.createElement("span");
        tag.id = "demo-pill";
        tag.className = "pill pill-demo";
        tag.innerHTML = '<i class="bi bi-wifi-off"></i> Datos de demostración';
        group.prepend(tag);
      } else if (!e.detail && tag) tag.remove();
    });
  });
})();
