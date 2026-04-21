#include "TEL0177.h"
#include <TEL0177.h>

// ==========================================================================
// Configuration
// ==========================================================================
#define MQTT_HOST       "mqtt.qameleo.fr"
#define MQTT_PORT       1935
#define MQTT_CLIENT_ID  "Emmanuel"
#define MQTT_USER       "gamasenseit"
#define MQTT_PASS       "gamasenseit"
#define MQTT_TOPIC      "gamasenseit"
#define MQTT_QOS        0
#define SIM_PIN         "0000"

TEL0177 modem;

// ==========================================================================
// Callback MQTT
// ==========================================================================
void onMqttMessage(const char* topic, const char* payload, uint16_t length) {
  Serial.println("========================================");
  Serial.print  ("  [MQTT] Nouveau message sur : ");
  Serial.println(topic);
  Serial.print  ("  Taille   : ");
  Serial.print  (length);
  Serial.println(" bytes");
  Serial.print  ("  Contenu  : ");
  Serial.println(payload);
  Serial.println("========================================");
}

// ==========================================================================
// SETUP
// ==========================================================================
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(2000);

  Serial.println("=== TEST CALLBACK MQTT ===\n");

  // 1. Init modem
  Serial.print("Init modem... ");
  Serial.println(modem.begin(Serial2) ? "OK" : "KO");

  // 2. SIM PIN
  Serial.print("SIM PIN...    ");
  Serial.println(modem.simUnlock(SIM_PIN) ? "OK" : "KO");

  // Attente réseau
  Serial.println("Attente reseau (5s)...");
  delay(5000);

  // 3. Callback
  Serial.print("Callback...   ");
  Serial.println(modem.mqttsetCallback(onMqttMessage) ? "OK" : "KO");

  // 4. Config broker
  Serial.print("setServer...  ");
  Serial.println(modem.mqttsetServer(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS) ? "OK" : "KO");

  // 5. Connexion
  Serial.print("Connect...    ");
  Serial.println(modem.mqttconnect(MQTT_HOST, MQTT_PORT) ? "OK" : "KO");

  // 6. Subscribe
  Serial.print("Subscribe...  ");
  Serial.println(modem.mqttsubscribe(MQTT_TOPIC, MQTT_QOS) ? "OK" : "KO");

  Serial.println("\nEcoute en cours... (messages affiches via callback)");
}

// ==========================================================================
// LOOP — traitement des messages entrants
// ==========================================================================
void loop() {
  modem.loop();
}