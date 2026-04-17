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
#define MQTT_CLIENT_ID      "Emmanuel"
#define MQTT_USER           "gamasenseit"
#define MQTT_PASS           "gamasenseit"
#define MQTT_TOPIC          "gamasenseit"
#define MQTT_QOS            0
#define MQTT_RETAIN         false

TEL0177 modem;

unsigned long lastPublish = 0;
unsigned long publishCount = 0;

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
  printResult(
    "Config client MQTT",
    modem.mqttsetClient(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS)
  );

  printResult(
    "Connexion MQTT",
    modem.mqttconnect(MQTT_HOST, MQTT_PORT)
  );

  return modem.mqttisConnected();
}

void publishMessage() {
  char payload[128] = {0};

  publishCount++;

  snprintf(
    payload,
    sizeof(payload),
    "TEL0177 publish test #%lu",
    publishCount
  );

  Serial.print("Publication : ");
  Serial.println(payload);

  printResult(
    "Publish MQTT",
    modem.mqttpublish(MQTT_TOPIC, payload, MQTT_QOS, MQTT_RETAIN)
  );
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
  Serial.println("       TEL0177 - MQTT Publish Test");
  Serial.println("========================================");
  Serial.println();

  if (!setupModemNetwork()) {
    Serial.println("Echec configuration reseau/data.");
    return;
  }

  if (!setupMqtt()) {
    Serial.println("Echec connexion MQTT.");
    return;
  }

  publishMessage();

  Serial.println();
  Serial.println("Publication periodique toutes les 10 secondes.");
}

// ==========================================================================
// LOOP
// ==========================================================================
void loop() {
  modem.loop();

  if (millis() - lastPublish >= 10000) {
    lastPublish = millis();

    if (!modem.mqttisConnected()) {
      Serial.println("MQTT deconnecte, tentative reconnexion...");
      printResult("Reconnect MQTT", modem.mqttreconnect());
    }

    if (modem.mqttisConnected()) {
      publishMessage();
    }
  }
}