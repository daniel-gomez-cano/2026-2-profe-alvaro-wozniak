/**
 * Configuración de conexión al backend.
 * - API_BASE_URL: "" = mismo origen (el backend sirve también el frontend).
 *                 "https://api.tudominio.com" = backend separado (activa CORS allá).
 * - ALLOW_MOCK_FALLBACK: si el backend no responde, usa datos de demostración
 *                        (útil para diseñar sin backend). Pon false en producción.
 * - FORCE_MOCK: ignora el backend y usa siempre datos de demostración.
 */
window.BANANIN_CONFIG = {
  API_BASE_URL: "",
  TIMEOUT_MS: 6000,
  AUTH_TOKEN: null,            // opcional: "Bearer" token
  ALLOW_MOCK_FALLBACK: true,
  FORCE_MOCK: false,
  REFRESH_LECTURA_MS: 10000    // cada cuánto se refresca la lectura actual
};
