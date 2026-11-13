#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_TCS34725.h>
#include "secrets.h"

// ---------- Pines ----------
const int MQ135_PIN = 4;
const int SDA_PIN   = 8;
const int SCL_PIN   = 9;
const int LED_PIN   = 5;

// ---------- Parámetros ----------
const int  MQ_MUESTRAS = 20;
const unsigned long PRECALENTAMIENTO_MS = 15UL * 60UL * 1000UL;
const unsigned long PERIODO_LECTURA_MS   = 1000;   // lectura + serial
const unsigned long PERIODO_ULTIMO_MS    = 5000;   // actualiza /ultimo
const unsigned long PERIODO_HISTORIAL_MS = 60000;  // agrega a /historial

Adafruit_TCS34725 tcs(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);
bool tcsOk = false;
float lineaBase = -1;
float ultimoMq = 0;

String idToken;
unsigned long tokenExpiraMs = 0;

// ---------- Sensores ----------
float leerMQ135() {
  long suma = 0;
  for (int i = 0; i < MQ_MUESTRAS; i++) {
    suma += analogRead(MQ135_PIN);
    delay(5);
  }
  return suma / (float)MQ_MUESTRAS;
}

// ---------- WiFi ----------
void conectarWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  Serial.printf("Conectando a %s", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? "\nWiFi OK" : "\nWiFi FALLO");
}

// ---------- Firebase Auth (REST) ----------
bool iniciarSesion() {
  WiFiClientSecure client;
  client.setInsecure();  // para pruebas; luego se puede validar el certificado
  HTTPClient http;
  http.setTimeout(8000);

  String url = String("https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=") + FB_API_KEY;
  if (!http.begin(client, url)) return false;
  http.addHeader("Content-Type", "application/json");

  String body = String("{\"email\":\"") + FB_USER_EMAIL +
                "\",\"password\":\"" + FB_USER_PASS +
                "\",\"returnSecureToken\":true}";
  int code = http.POST(body);
  String resp = http.getString();
  http.end();

  if (code != 200) {
    Serial.printf("# Login Firebase fallo: HTTP %d\n", code);
    return false;
  }
  JsonDocument doc;
  if (deserializeJson(doc, resp)) return false;

  idToken = doc["idToken"].as<String>();
  long expiraEn = atol(doc["expiresIn"] | "3600");
  tokenExpiraMs = millis() + (unsigned long)(expiraEn - 300) * 1000UL;  // renueva 5 min antes
  Serial.println("# Sesion Firebase OK");
  return true;
}

bool asegurarToken() {
  if (idToken.isEmpty() || (long)(millis() - tokenExpiraMs) >= 0) {
    return iniciarSesion();
  }
  return true;
}

// ---------- Realtime Database (REST) ----------
int fbEnviar(const char* metodo, const String& ruta, const String& json) {
  if (!asegurarToken()) return -1;

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  http.setTimeout(8000);

  String url = String(FB_DB_URL) + ruta + ".json?auth=" + idToken;
  if (!http.begin(client, url)) return -1;
  http.addHeader("Content-Type", "application/json");
  int code = http.sendRequest(metodo, json);
  http.end();
  return code;
}

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  delay(2000);

  analogReadResolution(12);
  analogSetPinAttenuation(MQ135_PIN, ADC_11db);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Wire.begin(SDA_PIN, SCL_PIN);
  tcsOk = tcs.begin(TCS34725_ADDRESS, &Wire);

  Serial.println("=== Nodo banano-iot ===");
  Serial.println(tcsOk ? "TCS34725: OK" : "TCS34725: NO detectado (revisa SDA, SCL, 3V3 y GND)");
  Serial.println("MQ-135: precalentando 15 min. Envia 'b' para guardar la linea base en aire limpio.");

  conectarWiFi();
  Serial.println("t_s,estado,mq_raw,mq_delta,R,G,B,C,r_norm,g_norm,b_norm");
}

// ---------- Loop ----------
void loop() {
  static unsigned long tLectura = 0, tUltimo = 0, tHistorial = 0;

  // Comando por serial: 'b' guarda la línea base
  while (Serial.available()) {
    char c = Serial.read();
    if (c == 'b' || c == 'B') {
      lineaBase = ultimoMq;
      Serial.printf("# Linea base guardada: %.0f\n", lineaBase);
    }
  }

  unsigned long ahora = millis();
  if (ahora - tLectura < PERIODO_LECTURA_MS) return;
  tLectura = ahora;

  // --- Lectura ---
  float mq = leerMQ135();
  ultimoMq = mq;
  float delta = (lineaBase >= 0) ? (mq - lineaBase) : 0;

  uint16_t r = 0, g = 0, b = 0, c = 0;
  float rn = 0, gn = 0, bn = 0;
  if (tcsOk) {
    tcs.getRawData(&r, &g, &b, &c);
    if (c > 0) {
      rn = r * 255.0f / c;
      gn = g * 255.0f / c;
      bn = b * 255.0f / c;
    }
  }
  const char* estado = (ahora < PRECALENTAMIENTO_MS) ? "precalentando" : "listo";

  Serial.printf("%lu,%s,%.0f,%.0f,%u,%u,%u,%u,%.0f,%.0f,%.0f\n",
                ahora / 1000, estado, mq, delta, r, g, b, c, rn, gn, bn);

  // --- Envío ---
  conectarWiFi();
  if (WiFi.status() != WL_CONNECTED) return;

  char json[320];
  snprintf(json, sizeof(json),
    "{\"estado\":\"%s\",\"mq_raw\":%.0f,\"mq_delta\":%.0f,\"linea_base\":%.0f,"
    "\"r\":%u,\"g\":%u,\"b\":%u,\"c\":%u,"
    "\"r_norm\":%.1f,\"g_norm\":%.1f,\"b_norm\":%.1f,"
    "\"ts\":{\".sv\":\"timestamp\"}}",
    estado, mq, delta, lineaBase, r, g, b, c, rn, gn, bn);

  if (ahora - tUltimo >= PERIODO_ULTIMO_MS) {
    tUltimo = ahora;
    int code = fbEnviar("PUT", String("/nodos/") + NODO_ID + "/ultimo", json);
    Serial.printf("# ultimo -> HTTP %d\n", code);
  }
  if (ahora - tHistorial >= PERIODO_HISTORIAL_MS) {
    tHistorial = ahora;
    int code = fbEnviar("POST", String("/nodos/") + NODO_ID + "/historial", json);
    Serial.printf("# historial -> HTTP %d\n", code);
  }
}