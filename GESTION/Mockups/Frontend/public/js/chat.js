/** Pantalla Bananín (chat con el asistente / LLM). */
(function () {
  const { api } = window.Bananin;
  const $ = (id) => document.getElementById(id);
  const log = $("chat-log"), form = $("chat-form"), input = $("chat-input"), send = $("chat-send");
  const STORE = "bananin.chat.v1";

  const SALUDO = [
    "¡Hola! 🍌 Soy **Bananín**, tu amigo de IA personal para el mundo del banano.",
    "Analizo los datos del estado de tu fruta para ayudarte a decidir qué hacer con ella: cómo transportarla, por dónde distribuirla y cuándo venderla al mejor precio, para que tomes decisiones más rentables y reduzcas pérdidas.",
    "Cuéntame, ¿en qué te puedo ayudar hoy?",
  ].join("\n\n");

  let conv = { id: null, mensajes: [] };
  try { conv = JSON.parse(sessionStorage.getItem(STORE)) || conv; } catch (_) {}
  const save = () => { try { sessionStorage.setItem(STORE, JSON.stringify(conv)); } catch (_) {} };

  // Escapa HTML y permite solo **negrita** (seguro frente a XSS)
  const render = (t) =>
    t.replace(/[&<>"']/g, (c) => ({ "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;", "'": "&#39;" }[c]))
     .replace(/\*\*(.+?)\*\*/g, "<strong>$1</strong>");

  function add(rol, texto, extraClass = "") {
    const el = document.createElement("div");
    el.className = `msg ${rol} ${extraClass}`.trim();
    el.innerHTML = render(texto);
    log.appendChild(el);
    log.scrollTop = log.scrollHeight;
    return el;
  }

  async function enviar(texto) {
    texto = texto.trim();
    if (!texto) return;
    $("suggestions").classList.add("d-none");
    add("user", texto);
    conv.mensajes.push({ rol: "user", texto });
    input.value = "";
    send.disabled = input.disabled = true;

    const typing = add("bot", "");
    typing.innerHTML = '<span class="typing"><i></i><i></i><i></i></span>';
    try {
      const r = await api.enviarMensaje(texto, conv.id);
      conv.id = r.conversacion_id || conv.id;
      typing.innerHTML = render(r.respuesta);
      conv.mensajes.push({ rol: "bot", texto: r.respuesta });
    } catch (e) {
      console.error(e);
      typing.className = "msg error";
      typing.textContent = "No pude responder en este momento. Inténtalo de nuevo.";
    } finally {
      send.disabled = input.disabled = false;
      input.focus();
      log.scrollTop = log.scrollHeight;
      save();
    }
  }

  // Estado inicial
  add("bot", SALUDO);
  if (conv.mensajes.length) {
    $("suggestions").classList.add("d-none");
    conv.mensajes.forEach((m) => add(m.rol, m.texto));
  }

  form.addEventListener("submit", (e) => { e.preventDefault(); enviar(input.value); });
  $("suggestions").addEventListener("click", (e) => { if (e.target.tagName === "BUTTON") enviar(e.target.textContent); });
  input.focus();
})();
