#include <TEL0177.h>

// ==========================================================================
// Configuration série
// ==========================================================================
#define SERIAL_MONITOR_BAUD 115200
#define MODEM_BAUD          115200

// ==========================================================================
// Configuration SIM / réseau mobile
// ==========================================================================
#define SIM_PIN             "0000"

// APN à adapter selon la carte SIM utilisée
#define GSM_APN             "mmsbouygtel.com"
#define GSM_USER            nullptr
#define GSM_PASS            nullptr
#define GSM_PDP_TYPE        "IP"

// ==========================================================================
// Configuration MQTT
// ==========================================================================
#define MQTT_HOST           "mqtt.qameleo.fr"
#define MQTT_PORT           1935
#define MQTT_CLIENT_ID      "Emmanuel_sub"
#define MQTT_USER           "gamasenseit"
#define MQTT_PASS           "gamasenseit"
#define MQTT_TOPIC          "gamasenseit"
#define MQTT_QOS            0

TEL0177 modem;

// ==========================================================================
// Utilitaires
// ==========================================================================
void printResult(const char* label, bool ok) {
  Serial.print(label);
  Serial.print("... ");
  Serial.print(ok ? "OK" : "KO");

  if (!ok) {
    Serial.print(" | lastError = ");
    Serial.print((int)modem.lastError());
    Serial.print(" | lastResponse = ");
    Serial.print(modem.lastResponse());
  }

  Serial.println();
}

// ==========================================================================
// Callback MQTT décodé
// ==========================================================================
void onMqttMessage(const char* topic, const char* payload, uint16_t length) {
  Serial.println();
  Serial.println("========================================");
  Serial.println("[MQTT] Message recu");
  Serial.print("Topic   : ");
  Serial.println(topic);
  Serial.print("Length  : ");
  Serial.println(length);
  Serial.print("Payload : ");
  Serial.println(payload);
  Serial.println("========================================");
}

// ==========================================================================
// Callback brut pour debug
// ==========================================================================
void onMqttRaw(const char* urc) {
  Serial.print("[MQTT RAW] ");
  Serial.println(urc);
}

bool setupModemNetwork() {
  printResult("Init modem", modem.begin(Serial2));

  if (!modem.isAlive()) {
    Serial.println("Modem non detecte.");
    return false;
  }

  bool simReady = modem.isSIMReady();
  printResult("SIM prete", simReady);

  if (!simReady) {
    printResult("Deverrouillage SIM", modem.simUnlock(SIM_PIN));
  }

  printResult("Attente reseau", modem.waitForNetwork(30000));

  int rssi = 0;
  int ber = 0;

  if (modem.getSignalQuality(rssi, ber)) {
    Serial.print("RSSI = ");
    Serial.print(rssi);
    Serial.print(" | BER = ");
    Serial.println(ber);
  }

  printResult(
    "Connexion data",
    modem.networkConnect(GSM_APN, GSM_USER, GSM_PASS, GSM_PDP_TYPE)
  );

  Serial.print("IP = ");
  Serial.println(modem.ipAddress());

  return modem.lastError() == TEL_OK;
}

bool setupMqtt() {
  printResult("Callback MQTT", modem.mqttsetCallback(onMqttMessage));
  printResult("Callback RAW", modem.mqttsetRawCallback(onMqttRaw));

  printResult(
    "Config client MQTT",
    modem.mqttsetClient(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS)
  );

  printResult(
    "Connexion MQTT",
    modem.mqttconnect(MQTT_HOST, MQTT_PORT)
  );

  printResult(
    "Subscribe MQTT",
    modem.mqttsubscribe(MQTT_TOPIC, MQTT_QOS)
  );

  return modem.lastError() == TEL_OK;
}

// ==========================================================================
// SETUP
// ==========================================================================
void setup() {
  Serial.begin(SERIAL_MONITOR_BAUD);
  Serial2.begin(MODEM_BAUD);

  delay(2000);

  Serial.println();
  Serial.println("========================================");
  Serial.println("      TEL0177 - MQTT Subscribe Test");
  Serial.println("========================================");
  Serial.println();

  if (!setupModemNetwork()) {
    Serial.println("Echec configuration reseau/data.");
    return;
  }

  if (!setupMqtt()) {
    Serial.println("Echec configuration MQTT.");
    return;
  }

  Serial.println();
  Serial.println("Ecoute MQTT en cours...");
  Serial.println("Publie un message sur le topic pour tester le callback.");
}

// ==========================================================================
// LOOP
// ==========================================================================
const unsigned long MQTT_CHECK_INTERVAL_MS = 5000;
unsigned long lastMqttCheck = 0;

void loop() {
  // 1) Toujours traiter les URC MQTT en priorité
  modem.loop();

  // 2) Vérification de connexion seulement toutes les 5s
  unsigned long now = millis();
  if (now - lastMqttCheck < MQTT_CHECK_INTERVAL_MS) {
    return;
  }
  lastMqttCheck = now;

  if (!modem.mqttisConnected()) {
    Serial.println("MQTT deconnecte, tentative reconnexion...");

    if (modem.mqttreconnect()) {
      Serial.println("Reconnect OK");
      printResult("Re-subscribe MQTT", modem.mqttsubscribe(MQTT_TOPIC, MQTT_QOS));
    } else {
      Serial.print("Reconnect KO | lastError = ");
      Serial.println((int)modem.lastError());
    }
  }
}