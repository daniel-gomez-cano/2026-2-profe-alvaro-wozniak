// ============================================================
//  Nodo banano-iot  -  CREACION DE DATASET (sin internet)
//  Sensores: MQ-135 (analogico) + TCS34725 (I2C)  |  ESP32-S3
//
//  Cada fila del CSV = 1 toma de 1 banano:
//    id_banano, dias, estado, podrido,
//    R_top,R_mid,R_bot, G_top,G_mid,G_bot, B_top,B_mid,B_bot,
//    H_top,H_mid,H_bot, S_top,S_mid,S_bot, V_top,V_mid,V_bot,
//    C_top,C_mid,C_bot,
//    mq_mv, mq_rs, mq_ratio, valido
//  (R,G,B = reflectancia calibrada 0..1; C = crudo del canal clear)
//  podrido: 1 = si, 0 = no
//
//  El CSV se guarda en la flash (LittleFS: /dataset.csv) y se puede
//  corregir por comandos o volcar al PC con "dump".
//
//  IMPORTANTE (monitor serie): escribe el comando y presiona ENTER.
//  En platformio.ini agrega:  monitor_filters = send_on_enter
//                             monitor_echo = yes
// ============================================================
#include <Arduino.h>
#include <Wire.h>
#include <Preferences.h>
#include <LittleFS.h>
#include <Adafruit_TCS34725.h>

// ---------- Pines ----------
const int MQ135_PIN = 4;
const int SDA_PIN   = 8;
const int SCL_PIN   = 9;
const int LED_PIN   = 5;

// ---------- Parametros generales ----------
const int  MQ_MUESTRAS   = 20;
const int  TCS_PROMEDIO  = 5;        // lecturas del TCS que se promedian por parte
const int  N_PARTES      = 3;
// Sufijos de columna: parte superior, central e inferior del banano
const char* NOMBRE_PARTE[N_PARTES] = {"top", "mid", "bot"};
const unsigned long PRECALENTAMIENTO_MS = 15UL * 60UL * 1000UL;

// Validez de una lectura de color
const float C_MAX = 60000;           // por encima: sensor saturado
const float V_MIN = 0.10f;           // por debajo: HSV ruidoso

// ---------- Escala de maduracion (AJUSTALA A TU GUIA DE ETIQUETADO) ----------
// Por defecto: escala de von Loesecke 1..7
const int ESTADO_MIN = 1;
const int ESTADO_MAX = 4;
const char* ESTADOS[] = {
  "1 verde",
  "2 verde con amarillo",
  "3 pinton",
  "4 maduro"
};

// ---------- MQ-135 (alimentado a 3.3V, sin divisor) ----------
const float VCC_SENSOR = 3.3;
const float RL_KOHM    = 10.0;
const float DIV_FACTOR = 1.0;

// ---------- Archivos ----------
const char* ARCHIVO     = "/dataset.csv";
const char* ARCHIVO_TMP = "/dataset_tmp.csv";

Adafruit_TCS34725 tcs(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);
bool tcsOk = false;
bool vistaVivo = false;
Preferences prefs;

// ============================================================
//  Calibracion (la misma del firmware de calibracion)
// ============================================================
struct CalColor {
  float blanco[3];
  float oscuro[3];
  bool  okBlanco;
  bool  okOscuro;
};
CalColor cal = {{1, 1, 1}, {0, 0, 0}, false, false};
float r0 = -1;   // Rs en aire limpio (kOhm)

// Respaldo con tus valores: se usa solo si la NVS esta vacia
const CalColor CAL_DEFAULT = {{6910, 6839, 5158}, {23, 11, 12}, true, true};
const float    R0_DEFAULT  = 20.75;

void cargarCalibracion() {
  prefs.begin("cal", true);
  if (prefs.getBytesLength("tcs") == sizeof(cal)) prefs.getBytes("tcs", &cal, sizeof(cal));
  else cal = CAL_DEFAULT;
  r0 = prefs.getFloat("r0", R0_DEFAULT);
  prefs.end();
}

void guardarCalibracion() {
  prefs.begin("cal", false);
  prefs.putBytes("tcs", &cal, sizeof(cal));
  prefs.putFloat("r0", r0);
  prefs.end();
}

void imprimirCalibracion() {
  Serial.println("# ---- Calibracion actual ----");
  Serial.printf("# Oscuro: %s  R=%.0f G=%.0f B=%.0f\n", cal.okOscuro ? "OK" : "NO",
                cal.oscuro[0], cal.oscuro[1], cal.oscuro[2]);
  Serial.printf("# Blanco: %s  R=%.0f G=%.0f B=%.0f\n", cal.okBlanco ? "OK" : "NO",
                cal.blanco[0], cal.blanco[1], cal.blanco[2]);
  if (r0 > 0) Serial.printf("# R0 MQ-135: %.2f kOhm\n", r0);
  else        Serial.println("# R0 MQ-135: NO calibrado");
}

// ============================================================
//  Sesion: banano actual, dias, estado y podrido (persisten tras reiniciar)
// ============================================================
struct Sesion {
  char  id[16];
  float dias;
  int   estado;
  int   podrido;   // 0 = no, 1 = si
};
Sesion ses = {"B001", 0, 1, 0};
int contadorId = 1;

void idAuto(int n, char* out) { snprintf(out, 16, "B%03d", n); }

void cargarSesion() {
  prefs.begin("ses", true);
  contadorId = prefs.getInt("cnt", 1);
  String s = prefs.getString("id", "B001");
  s.toCharArray(ses.id, sizeof(ses.id));
  ses.dias    = prefs.getFloat("dias", 0);
  ses.estado  = prefs.getInt("est", ESTADO_MIN);
  ses.podrido = prefs.getInt("pod", 0);
  prefs.end();
}

void guardarSesion() {
  prefs.begin("ses", false);
  prefs.putInt("cnt", contadorId);
  prefs.putString("id", ses.id);
  prefs.putFloat("dias", ses.dias);
  prefs.putInt("est", ses.estado);
  prefs.putInt("pod", ses.podrido);
  prefs.end();
}

void mostrarSesion() {
  Serial.printf("# SESION -> banano=%s | dias=%.1f | estado=%s | podrido=%s\n", ses.id, ses.dias,
                (ses.estado >= ESTADO_MIN && ses.estado <= ESTADO_MAX)
                  ? ESTADOS[ses.estado - ESTADO_MIN] : "?",
                ses.podrido ? "si" : "no");
}

// ============================================================
//  Sensores
// ============================================================
void leerTcsPromedio(int n, float out[3], float &cOut) {
  float sr = 0, sg = 0, sb = 0, sc = 0;
  for (int i = 0; i < n; i++) {
    uint16_t r, g, b, c;
    tcs.getRawData(&r, &g, &b, &c);
    sr += r; sg += g; sb += b; sc += c;
  }
  out[0] = sr / n; out[1] = sg / n; out[2] = sb / n; cOut = sc / n;
}

void calibrarRGB(float r, float g, float b, float &rc, float &gc, float &bc) {
  float raw[3] = {r, g, b};
  float res[3];
  for (int i = 0; i < 3; i++) {
    float rango = cal.blanco[i] - cal.oscuro[i];
    res[i] = (rango > 1) ? (raw[i] - cal.oscuro[i]) / rango : 0;
    res[i] = constrain(res[i], 0.0f, 1.0f);
  }
  rc = res[0]; gc = res[1]; bc = res[2];
}

void rgbToHsv(float r, float g, float b, float &h, float &s, float &v) {
  float mx = max(r, max(g, b));
  float mn = min(r, min(g, b));
  float d = mx - mn;
  v = mx;
  s = (mx > 0) ? d / mx : 0;
  if (d < 1e-6f)    h = 0;
  else if (mx == r) h = 60.0f * fmodf((g - b) / d, 6.0f);
  else if (mx == g) h = 60.0f * ((b - r) / d + 2.0f);
  else              h = 60.0f * ((r - g) / d + 4.0f);
  if (h < 0) h += 360.0f;
}

float leerMQ135mV() {
  long suma = 0;
  for (int i = 0; i < MQ_MUESTRAS; i++) {
    suma += analogReadMilliVolts(MQ135_PIN);
    delay(5);
  }
  return suma / (float)MQ_MUESTRAS;
}

float calcularRs(float mv) {
  float vout = (mv / 1000.0f) * DIV_FACTOR;
  if (vout < 0.01f) return -1;
  return RL_KOHM * (VCC_SENSOR - vout) / vout;   // kOhm
}

struct Parte {
  uint16_t c;
  float rc, gc, bc, h, s, v;
  bool ok;
};

Parte medirParte() {
  float raw[3], cc;
  leerTcsPromedio(TCS_PROMEDIO, raw, cc);
  Parte p;
  p.c = (uint16_t)cc;
  calibrarRGB(raw[0], raw[1], raw[2], p.rc, p.gc, p.bc);
  rgbToHsv(p.rc, p.gc, p.bc, p.h, p.s, p.v);
  p.ok = (cc < C_MAX) && (p.v >= V_MIN);
  return p;
}

// ============================================================
//  Archivo CSV (LittleFS)
// ============================================================
void escribirEncabezado(Print &out) {
  out.print("id_banano,dias,estado,podrido");
  const char* canales[] = {"R", "G", "B", "H", "S", "V", "C"};
  for (int c = 0; c < 7; c++)
    for (int i = 0; i < N_PARTES; i++)
      out.printf(",%s_%s", canales[c], NOMBRE_PARTE[i]);
  out.print(",mq_mv,mq_rs,mq_ratio,valido\n");
}

void iniciarArchivo() {
  if (!LittleFS.exists(ARCHIVO)) {
    File f = LittleFS.open(ARCHIVO, FILE_WRITE);
    if (f) { escribirEncabezado(f); f.close(); }
  }
}

int contarFilas() {
  File f = LittleFS.open(ARCHIVO, FILE_READ);
  if (!f) return 0;
  int n = 0;
  while (f.available()) {
    String l = f.readStringUntil('\n');
    l.trim();
    if (l.length()) n++;
  }
  f.close();
  return max(0, n - 1);   // sin encabezado
}

// Borra una fila (borrar=true) o edita id/dias/estado/podrido (campo 0/1/2/3). fila: 1..N
bool modificarFila(int fila, bool borrar, int campo, const String& valor) {
  File in  = LittleFS.open(ARCHIVO, FILE_READ);
  File out = LittleFS.open(ARCHIVO_TMP, FILE_WRITE);
  if (!in || !out) { if (in) in.close(); if (out) out.close(); return false; }

  int linea = 0;
  bool hecho = false;
  while (in.available()) {
    String l = in.readStringUntil('\n');
    l.trim();
    if (!l.length()) continue;

    if (linea == fila && linea > 0) {
      int i1 = l.indexOf(',');
      int i2 = l.indexOf(',', i1 + 1);
      int i3 = l.indexOf(',', i2 + 1);
      int i4 = l.indexOf(',', i3 + 1);
      if (i1 > 0 && i2 > 0 && i3 > 0 && i4 > 0) {
        hecho = true;
        if (borrar) { linea++; continue; }
        String id    = l.substring(0, i1);
        String dias  = l.substring(i1 + 1, i2);
        String est   = l.substring(i2 + 1, i3);
        String pod   = l.substring(i3 + 1, i4);
        String resto = l.substring(i4);
        if (campo == 0) id = valor;
        else if (campo == 1) dias = valor;
        else if (campo == 2) est = valor;
        else if (campo == 3) pod = valor;
        l = id + "," + dias + "," + est + "," + pod + resto;
      }
    }
    out.print(l);
    out.print('\n');
    linea++;
  }
  in.close();
  out.close();

  if (!hecho) { LittleFS.remove(ARCHIVO_TMP); return false; }
  LittleFS.remove(ARCHIVO);
  LittleFS.rename(ARCHIVO_TMP, ARCHIVO);
  return true;
}

void volcarCsv() {
  File f = LittleFS.open(ARCHIVO, FILE_READ);
  if (!f) { Serial.println("# No hay archivo"); return; }
  Serial.println(">>>CSV");
  while (f.available()) Serial.write(f.read());
  Serial.println("<<<CSV");
  f.close();
}

void listarUltimas(int n) {
  int total = contarFilas();
  if (total == 0) { Serial.println("# Dataset vacio"); return; }
  int desde = max(1, total - n + 1);
  File f = LittleFS.open(ARCHIVO, FILE_READ);
  if (!f) return;
  int linea = 0;
  Serial.printf("# Filas %d a %d de %d (id,dias,estado,podrido,...):\n", desde, total, total);
  while (f.available()) {
    String l = f.readStringUntil('\n');
    l.trim();
    if (!l.length()) continue;
    if (linea >= desde) Serial.printf("# %d: %s\n", linea, l.c_str());
    linea++;
  }
  f.close();
}

// ============================================================
//  Captura de una fila
// ============================================================
String esperarLinea() {
  while (!Serial.available()) delay(10);
  String s = Serial.readStringUntil('\n');
  s.trim();
  return s;
}

bool esCancelar(const String& s) { return s == "x" || s == "X"; }

void imprimirParte(int i, const Parte& p) {
  Serial.printf("#   %s: RGBcal=%.3f %.3f %.3f | H=%.1f S=%.3f V=%.3f | C=%u %s\n",
                NOMBRE_PARTE[i], p.rc, p.gc, p.bc, p.h, p.s, p.v, p.c,
                p.ok ? "OK" : "<- AVISO: saturada o muy oscura");
}

bool precalentadoManual = false;   // true = ignorar la espera de 15 min

bool mqListo() {
  return precalentadoManual || millis() >= PRECALENTAMIENTO_MS;
}

void capturar() {
  if (!tcsOk) { Serial.println("# TCS34725 no detectado"); return; }
  if (!cal.okBlanco || !cal.okOscuro) { Serial.println("# Falta calibrar color (k y w)"); return; }
  if (r0 <= 0) Serial.println("# AVISO: sin R0 del MQ-135 (b), mq_ratio saldra 0");
  if (!mqListo())
    Serial.println("# AVISO: MQ-135 aun precalentando, la fila quedara con valido=0");

  mostrarSesion();
  Parte partes[N_PARTES];

  for (int i = 0; i < N_PARTES; i++) {
    while (true) {
      Serial.printf("# Parte %d/%d (%s): coloca el sensor y presiona Enter (x=cancelar)\n",
                    i + 1, N_PARTES, NOMBRE_PARTE[i]);
      if (esCancelar(esperarLinea())) { Serial.println("# Captura cancelada"); return; }

      partes[i] = medirParte();
      imprimirParte(i, partes[i]);

      Serial.println("# Enter=aceptar  r=repetir  x=cancelar");
      String a = esperarLinea();
      if (esCancelar(a)) { Serial.println("# Captura cancelada"); return; }
      if (a == "r" || a == "R") continue;
      break;
    }
  }

  float mv = leerMQ135mV();
  float rs = calcularRs(mv);
  float ratio = (r0 > 0 && rs > 0) ? rs / r0 : 0;

  bool valido = (mqListo()) && (ratio > 0);
  for (int i = 0; i < N_PARTES; i++) valido = valido && partes[i].ok;

  char fila[700];
  int n = snprintf(fila, sizeof(fila), "%s,%.1f,%d,%d", ses.id, ses.dias, ses.estado, ses.podrido);
  // Orden: R_top,R_mid,R_bot, G_..., B_..., H_..., S_..., V_..., C_...
  for (int i = 0; i < N_PARTES; i++) n += snprintf(fila + n, sizeof(fila) - n, ",%.4f", partes[i].rc);
  for (int i = 0; i < N_PARTES; i++) n += snprintf(fila + n, sizeof(fila) - n, ",%.4f", partes[i].gc);
  for (int i = 0; i < N_PARTES; i++) n += snprintf(fila + n, sizeof(fila) - n, ",%.4f", partes[i].bc);
  for (int i = 0; i < N_PARTES; i++) n += snprintf(fila + n, sizeof(fila) - n, ",%.1f", partes[i].h);
  for (int i = 0; i < N_PARTES; i++) n += snprintf(fila + n, sizeof(fila) - n, ",%.3f", partes[i].s);
  for (int i = 0; i < N_PARTES; i++) n += snprintf(fila + n, sizeof(fila) - n, ",%.3f", partes[i].v);
  for (int i = 0; i < N_PARTES; i++) n += snprintf(fila + n, sizeof(fila) - n, ",%u", partes[i].c);
  snprintf(fila + n, sizeof(fila) - n, ",%.0f,%.2f,%.3f,%d", mv, rs, ratio, valido ? 1 : 0);

  File f = LittleFS.open(ARCHIVO, FILE_APPEND);
  if (!f) { Serial.println("# ERROR: no pude abrir el archivo"); return; }
  f.print(fila);
  f.print('\n');
  f.close();

  Serial.printf("# MQ-135: %.0f mV | Rs=%.2f kOhm | ratio=%.3f\n", mv, rs, ratio);
  Serial.printf("# Fila %d guardada%s\n", contarFilas(), valido ? "" : " (valido=0)");
  Serial.printf("# %s\n", fila);
}

// ============================================================
//  Comandos
// ============================================================
void ayuda() {
  Serial.println("# ---------------- COMANDOS (termina con Enter) ----------------");
  Serial.println("# m              capturar una fila (3 partes + MQ-135)");
  Serial.println("# n              nuevo banano (id automatico, dias=0, estado=minimo, podrido=no)");
  Serial.println("# id B007        fijar id a mano");
  Serial.println("# d 3 | d 2.5    fijar dias desde cosecha      d +  suma 1 dia");
  Serial.println("# e 4            fijar estado de maduracion    e  (solo) lista la escala");
  Serial.println("# pod si | pod no  marcar si el banano esta podrido");
  Serial.println("# s              ver sesion actual");
  Serial.println("# pre           alterna precalentamiento manual (omite espera de 15 min)");
  Serial.println("# l  | l 10      ultimas filas del dataset");
  Serial.println("# edit 12 estado 5   corregir fila 12 (campos: id, dias, estado, podrido)");
  Serial.println("# u              borrar la ultima fila     del 12  borrar la fila 12");
  Serial.println("# dump           imprimir todo el CSV (copialo a un .csv)");
  Serial.println("# borrar si      vaciar el dataset (no toca la calibracion)");
  Serial.println("# v              vista en vivo on/off (para posicionar el sensor)");
  Serial.println("# k / w / b / p  calibrar oscuro / blanco / R0 MQ-135 / ver calibracion");
  Serial.println("# h              esta ayuda");
}

void listarEstados() {
  Serial.println("# Escala de estado:");
  for (int i = ESTADO_MIN; i <= ESTADO_MAX; i++) Serial.printf("#   %s\n", ESTADOS[i - ESTADO_MIN]);
}

void procesarLinea(String l) {
  l.trim();
  if (!l.length()) return;
  int sp = l.indexOf(' ');
  String cmd = (sp < 0) ? l : l.substring(0, sp);
  String arg = (sp < 0) ? "" : l.substring(sp + 1);
  arg.trim();
  arg.replace(',', '.');   // acepta 2,5 como 2.5
  cmd.toLowerCase();

  float tmp[3], cc;

  if (cmd == "m") {
    capturar();
  } else if (cmd == "n") {
    contadorId++;
    idAuto(contadorId, ses.id);
    ses.dias = 0;
    ses.estado = ESTADO_MIN;
    ses.podrido = 0;
    guardarSesion();
    mostrarSesion();
  } else if (cmd == "id") {
    if (!arg.length() || arg.length() > 15 || arg.indexOf(',') >= 0 || arg.indexOf(' ') >= 0) {
      Serial.println("# Uso: id B007  (sin espacios ni comas, max 15 caracteres)");
    } else {
      arg.toCharArray(ses.id, sizeof(ses.id));
      guardarSesion();
      mostrarSesion();
    }
  } else if (cmd == "d") {
    if (arg == "+") ses.dias += 1;
    else if (arg.length() && (isDigit(arg[0]))) ses.dias = arg.toFloat();
    else { Serial.println("# Uso: d 3  |  d 2.5  |  d +"); return; }
    guardarSesion();
    mostrarSesion();
  } else if (cmd == "e") {
    if (!arg.length()) { listarEstados(); mostrarSesion(); return; }
    int v = arg.toInt();
    if (v < ESTADO_MIN || v > ESTADO_MAX) {
      Serial.printf("# Estado fuera de rango (%d..%d)\n", ESTADO_MIN, ESTADO_MAX);
      return;
    }
    ses.estado = v;
    guardarSesion();
    mostrarSesion();
  } else if (cmd == "pod") {
    String a = arg; a.toLowerCase();
    if (a == "si" || a == "1") ses.podrido = 1;
    else if (a == "no" || a == "0") ses.podrido = 0;
    else { Serial.println("# Uso: pod si | pod no"); return; }
    guardarSesion();
    mostrarSesion();
  } else if (cmd == "s") {
    mostrarSesion();
    Serial.printf("# Filas en el dataset: %d\n", contarFilas());
  } else if (cmd == "l") {
    listarUltimas(arg.length() ? max(1, (int)arg.toInt()) : 5);
  } else if (cmd == "pre") {
  precalentadoManual = !precalentadoManual;
  Serial.println(precalentadoManual ? "# Precalentamiento: OMITIDO (sensor marcado como caliente)"
                                    : "# Precalentamiento: automatico (15 min desde el arranque)");
  }else if (cmd == "edit") {
    int fila = 0; char campo[12] = {0}; char valor[24] = {0};
    if (sscanf(arg.c_str(), "%d %11s %23s", &fila, campo, valor) != 3) {
      Serial.println("# Uso: edit 12 estado 5   (campos: id, dias, estado, podrido)");
      return;
    }
    String c = String(campo); c.toLowerCase();
    String v = String(valor);
    int idx = (c == "id") ? 0 : (c == "dias") ? 1 : (c == "estado") ? 2 : (c == "podrido") ? 3 : -1;
    if (idx < 0) { Serial.println("# Campo invalido (id, dias, estado, podrido)"); return; }
    if (idx == 0 && v.indexOf(',') >= 0) { Serial.println("# El id no puede tener comas"); return; }
    if (idx == 1) { char b[16]; snprintf(b, sizeof(b), "%.1f", v.toFloat()); v = b; }
    if (idx == 2) {
      int e = v.toInt();
      if (e < ESTADO_MIN || e > ESTADO_MAX) { Serial.println("# Estado fuera de rango"); return; }
      v = String(e);
    }
    if (idx == 3) {
      v.toLowerCase();
      if (v == "si") v = "1";
      else if (v == "no") v = "0";
      if (v != "0" && v != "1") { Serial.println("# podrido: 0/1 o si/no"); return; }
    }
    Serial.println(modificarFila(fila, false, idx, v) ? "# Fila editada" : "# No encontre esa fila");
  } else if (cmd == "u") {
    int n = contarFilas();
    if (n == 0) { Serial.println("# Dataset vacio"); return; }
    Serial.println(modificarFila(n, true, 0, "") ? "# Ultima fila borrada" : "# Error al borrar");
  } else if (cmd == "del") {
    int fila = arg.toInt();
    if (fila < 1) { Serial.println("# Uso: del 12"); return; }
    Serial.println(modificarFila(fila, true, 0, "") ? "# Fila borrada" : "# No encontre esa fila");
  } else if (cmd == "dump") {
    volcarCsv();
  } else if (cmd == "borrar") {
    if (arg != "si") { Serial.println("# Esto borra TODO el dataset. Escribe: borrar si"); return; }
    LittleFS.remove(ARCHIVO);
    iniciarArchivo();
    Serial.println("# Dataset vaciado");
  } else if (cmd == "v") {
    vistaVivo = !vistaVivo;
    Serial.println(vistaVivo ? "# Vista en vivo: ON" : "# Vista en vivo: OFF");
  } else if (cmd == "k") {
    if (!tcsOk) { Serial.println("# TCS34725 no detectado"); return; }
    leerTcsPromedio(10, tmp, cc);
    memcpy(cal.oscuro, tmp, sizeof(tmp));
    cal.okOscuro = true;
    guardarCalibracion();
    Serial.printf("# Oscuro guardado: %.0f %.0f %.0f (C=%.0f)\n", tmp[0], tmp[1], tmp[2], cc);
  } else if (cmd == "w") {
    if (!tcsOk) { Serial.println("# TCS34725 no detectado"); return; }
    leerTcsPromedio(10, tmp, cc);
    memcpy(cal.blanco, tmp, sizeof(tmp));
    cal.okBlanco = true;
    guardarCalibracion();
    Serial.printf("# Blanco guardado: %.0f %.0f %.0f (C=%.0f)\n", tmp[0], tmp[1], tmp[2], cc);
    if (cc > 60000) Serial.println("# AVISO: sensor saturado");
  } else if (cmd == "b") {
    float acum = 0; int validas = 0;
    for (int i = 0; i < 20; i++) {
      float rs = calcularRs(leerMQ135mV());
      if (rs > 0) { acum += rs; validas++; }
      delay(50);
    }
    if (!validas) { Serial.println("# Lectura MQ-135 invalida"); return; }
    r0 = acum / validas;
    guardarCalibracion();
    Serial.printf("# R0 guardado: %.2f kOhm\n", r0);
  } else if (cmd == "p") {
    imprimirCalibracion();
  } else if (cmd == "h" || cmd == "?") {
    ayuda();
  } else {
    Serial.println("# Comando no reconocido (h = ayuda)");
  }
}

// ============================================================
//  Setup / Loop
// ============================================================
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1000);
  delay(2000);

  analogReadResolution(12);
  analogSetPinAttenuation(MQ135_PIN, ADC_11db);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Wire.begin(SDA_PIN, SCL_PIN);
  tcsOk = tcs.begin(TCS34725_ADDRESS, &Wire);

  if (!LittleFS.begin(true)) Serial.println("# ERROR: LittleFS no pudo iniciar");
  iniciarArchivo();

  cargarCalibracion();
  cargarSesion();

  Serial.println("=== Nodo banano-iot: DATASET (offline) ===");
  Serial.println(tcsOk ? "TCS34725: OK" : "TCS34725: NO detectado (revisa SDA, SCL, 3V3 y GND)");
  imprimirCalibracion();
  mostrarSesion();
  Serial.printf("# Filas en el dataset: %d\n", contarFilas());
  ayuda();
}

void loop() {
  if (Serial.available()) procesarLinea(Serial.readStringUntil('\n'));

  static unsigned long tVivo = 0;
  if (vistaVivo && tcsOk && millis() - tVivo >= 1000) {
    tVivo = millis();
    Parte p = medirParte();
    float mv = leerMQ135mV();
    float rs = calcularRs(mv);
    float ratio = (r0 > 0 && rs > 0) ? rs / r0 : 0;
    Serial.printf("# vivo: H=%.1f S=%.3f V=%.3f C=%u %s | mq_ratio=%.3f\n",
                  p.h, p.s, p.v, p.c, p.ok ? "OK" : "NO", ratio);
  }
}