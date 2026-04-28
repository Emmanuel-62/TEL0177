#include <TEL0177.h>

// ==========================================================================
// Configuration
// ==========================================================================
#define SIM_PIN         "0000"
#define GSM_APN         "mmsbouygtel.com"

#define MQTT_HOST       "mqtt.qameleo.fr"
#define MQTT_PORT       1935
#define MQTT_CLIENT_ID  "Emmanuel"
#define MQTT_USER       "gamasenseit"
#define MQTT_PASS       "gamasenseit"
#define MQTT_TOPIC      "gamasenseit"
#define MQTT_TOPIC_TIME "myTimer"

TEL0177 modem;

// ==========================================================================
// Debug
// ==========================================================================
void printTitle(const char* title) {
  Serial.println();
  Serial.println("=============================================================");
  Serial.println(title);
  Serial.println("=============================================================");
}

void printResult(const char* operation, bool ok) {
  Serial.print(operation);
  Serial.print(" : ");

  if (ok) {
    Serial.println("OK");
  } else {
    Serial.println("ERREUR");
    Serial.print("lastError = ");
    Serial.println((int)modem.lastError());

    Serial.print("lastResponse = ");
    Serial.println(modem.lastResponse());
  }
}

bool requireStep(const char* operation, bool ok) {
  printResult(operation, ok);

  if (!ok) {
    Serial.println("ARRET DU TEST A CETTE ETAPE");
  }

  return ok;
}

// ==========================================================================
// Callbacks MQTT
// ==========================================================================
void onMqttMessage(const char* topic, const char* payload, uint16_t length) {
  Serial.println();
  Serial.println("---------------- MQTT MESSAGE RECU ----------------");
  Serial.print("Topic   = ");
  Serial.println(topic);

  Serial.print("Payload = ");
  Serial.println(payload);

  Serial.print("Length  = ");
  Serial.println(length);
  Serial.println("---------------------------------------------------");
}

void onMqttRaw(const char* urc) {
  Serial.println();
  Serial.println("---------------- MQTT RAW URC ----------------");
  Serial.println(urc);
  Serial.println("----------------------------------------------");
}

// ==========================================================================
// Petite écoute MQTT
// ==========================================================================
void listen(uint32_t durationMs) {
  unsigned long start = millis();

  while (millis() - start < durationMs) {
    modem.loop();
    delay(10);
  }
}

// ==========================================================================
// Test informations modem
// ==========================================================================
void testModemInfo() {
  printTitle("INFOS MODULE / SIM");

  char buffer[96];

  memset(buffer, 0, sizeof(buffer));
  if (modem.getManufacturer(buffer, sizeof(buffer))) {
    Serial.print("Manufacturer = ");
    Serial.println(buffer);
  } else {
    printResult("getManufacturer()", false);
  }

  memset(buffer, 0, sizeof(buffer));
  if (modem.getModel(buffer, sizeof(buffer))) {
    Serial.print("Model = ");
    Serial.println(buffer);
  } else {
    printResult("getModel()", false);
  }

  memset(buffer, 0, sizeof(buffer));
  if (modem.getFirmware(buffer, sizeof(buffer))) {
    Serial.print("Firmware = ");
    Serial.println(buffer);
  } else {
    printResult("getFirmware()", false);
  }

  memset(buffer, 0, sizeof(buffer));
  if (modem.getIMEI(buffer, sizeof(buffer))) {
    Serial.print("IMEI = ");
    Serial.println(buffer);
  } else {
    printResult("getIMEI()", false);
  }

  memset(buffer, 0, sizeof(buffer));
  if (modem.getICCID(buffer, sizeof(buffer))) {
    Serial.print("ICCID = ");
    Serial.println(buffer);
  } else {
    printResult("getICCID()", false);
  }

  memset(buffer, 0, sizeof(buffer));
  if (modem.getIMSI(buffer, sizeof(buffer))) {
    Serial.print("IMSI = ");
    Serial.println(buffer);
  } else {
    printResult("getIMSI()", false);
  }
}

// ==========================================================================
// Test SIM + réseau
// ==========================================================================
bool testSIMAndNetwork() {
  printTitle("SIM + RESEAU MOBILE");

  if (!requireStep("modem.isAlive()", modem.isAlive())) {
    return false;
  }

  bool simReadyBefore = modem.isSIMReady();
  printResult("modem.isSIMReady() avant unlock", simReadyBefore);

  if (!simReadyBefore) {
    if (!requireStep("modem.simUnlock()", modem.simUnlock(SIM_PIN))) {
      return false;
    }
  }

  if (!requireStep("modem.isSIMReady() apres unlock", modem.isSIMReady())) {
    return false;
  }

  Serial.println("Attente reseau mobile...");

  if (!requireStep("modem.waitForNetwork()", modem.waitForNetwork())) {
    return false;
  }

  printResult("modem.isLTERegistered()", modem.isLTERegistered());
  printResult("modem.isDataNetworkRegistered()", modem.isDataNetworkRegistered());
  printResult("modem.isNetworkRegistered()", modem.isNetworkRegistered());

  int rssi = 0;
  int ber = 0;

  if (modem.getSignalQuality(rssi, ber)) {
    Serial.print("RSSI = ");
    Serial.println(rssi);

    Serial.print("BER  = ");
    Serial.println(ber);
  } else {
    printResult("modem.getSignalQuality()", false);
  }

  char op[64];
  memset(op, 0, sizeof(op));

  if (modem.getOperator(op, sizeof(op))) {
    Serial.print("Operateur = ");
    Serial.println(op);
  } else {
    printResult("modem.getOperator()", false);
  }

  return true;
}

// ==========================================================================
// Test connexion data
// ==========================================================================
bool testNetworkConnect() {
  printTitle("CONNEXION DATA / IP");

  if (!requireStep("modem.networkConnect()", modem.networkConnect(GSM_APN))) {
    return false;
  }

  printResult("modem.isDataAttached()", modem.isDataAttached());
  printResult("modem.isDataConnected()", modem.isDataConnected());

  Serial.print("networkState = ");
  Serial.println((int)modem.networkState());

  Serial.print("APN = ");
  Serial.println(modem.apn());

  Serial.print("PDP type = ");
  Serial.println(modem.pdpType());

  Serial.print("IP memorisee = ");
  Serial.println(modem.ipAddress());

  char ip[32];
  memset(ip, 0, sizeof(ip));

  if (modem.getIPAddress(ip, sizeof(ip))) {
    Serial.print("IP relue = ");
    Serial.println(ip);
  } else {
    printResult("modem.getIPAddress()", false);
  }

  return true;
}

// ==========================================================================
// Test connexion MQTT
// ==========================================================================
bool testMQTTConnect() {
  printTitle("CONNEXION MQTT");

  if (!requireStep("modem.mqttsetCallback()", modem.mqttsetCallback(onMqttMessage))) {
    return false;
  }

  if (!requireStep("modem.mqttsetRawCallback()", modem.mqttsetRawCallback(onMqttRaw))) {
    return false;
  }

  if (!requireStep("modem.mqttsetClient()", modem.mqttsetClient(
    MQTT_CLIENT_ID,
    MQTT_USER,
    MQTT_PASS
  ))) {
    return false;
  }

  if (!requireStep("modem.mqttconnect()", modem.mqttconnect(
    MQTT_HOST,
    MQTT_PORT
  ))) {
    return false;
  }

  if (!requireStep("modem.mqttisConnected()", modem.mqttisConnected())) {
    return false;
  }

  return true;
}

// ==========================================================================
// Test subscribe / publish / unsubscribe
// ==========================================================================
bool testMQTTSubscribePublish() {
  printTitle("MQTT SUBSCRIBE / PUBLISH / UNSUBSCRIBE");

  if (!requireStep("modem.mqttsubscribe(MQTT_TOPIC)", modem.mqttsubscribe(MQTT_TOPIC))) {
    return false;
  }

  if (!requireStep("modem.mqttsubscribe(MQTT_TOPIC_TIME)", modem.mqttsubscribe(MQTT_TOPIC_TIME))) {
    return false;
  }

  Serial.println("Ecoute MQTT pendant 5 secondes...");
  listen(5000);

  const char* msg1 = "TEL0177 test publish 1";

  Serial.print("Publication : ");
  Serial.println(msg1);

  if (!requireStep("modem.mqttpublish()", modem.mqttpublish(MQTT_TOPIC, msg1))) {
    return false;
  }

  Serial.println("Ecoute MQTT pendant 5 secondes apres publish...");
  listen(5000);

  if (!requireStep("modem.mqttunsubscribe(MQTT_TOPIC_TIME)", modem.mqttunsubscribe(MQTT_TOPIC_TIME))) {
    return false;
  }

  return true;
}

// ==========================================================================
// Test deconnexion MQTT
// ==========================================================================
bool testMQTTDisconnect() {
  printTitle("DECONNEXION MQTT");

  Serial.println("Verification MQTT avant deconnexion...");

  bool connectedBefore = modem.mqttisConnected();
  printResult("modem.mqttisConnected() avant disconnect", connectedBefore);

  Serial.println("Appel modem.mqttdisconnect()...");

  if (!requireStep("modem.mqttdisconnect()", modem.mqttdisconnect())) {
    return false;
  }

  delay(1000);

  bool connectedAfter = modem.mqttisConnected();

  Serial.print("MQTT connecte apres disconnect = ");
  Serial.println(connectedAfter ? "OUI" : "NON");

  if (connectedAfter) {
    Serial.println("ERREUR : MQTT semble encore connecte apres mqttdisconnect()");
    return false;
  }

  return true;
}

// ==========================================================================
// Test reconnexion MQTT
// ==========================================================================
bool testMQTTReconnect() {
  printTitle("RECONNEXION MQTT");

  Serial.println("Appel modem.mqttreconnect()...");

  if (!requireStep("modem.mqttreconnect()", modem.mqttreconnect())) {
    return false;
  }

  if (!requireStep("modem.mqttisConnected() apres reconnect", modem.mqttisConnected())) {
    return false;
  }

  if (!requireStep("modem.mqttsubscribe(MQTT_TOPIC)", modem.mqttsubscribe(MQTT_TOPIC))) {
    return false;
  }

  const char* msg2 = "TEL0177 test after reconnect";

  Serial.print("Publication apres reconnexion : ");
  Serial.println(msg2);

  if (!requireStep("modem.mqttpublish() apres reconnect", modem.mqttpublish(MQTT_TOPIC, msg2))) {
    return false;
  }

  Serial.println("Ecoute MQTT pendant 5 secondes apres reconnexion...");
  listen(5000);

  return true;
}

// ==========================================================================
// Test deconnexion réseau
// ==========================================================================
bool testNetworkDisconnect() {
  printTitle("DECONNEXION DATA / RESEAU");

  Serial.println("Deconnexion MQTT avant deconnecter la data...");

  if (modem.mqttisConnected()) {
    printResult("modem.mqttdisconnect()", modem.mqttdisconnect());
  } else {
    Serial.println("MQTT deja deconnecte");
  }

  Serial.println("Appel modem.networkDisconnect()...");

  if (!requireStep("modem.networkDisconnect()", modem.networkDisconnect())) {
    return false;
  }

  delay(1000);

  bool dataConnected = modem.isDataConnected();
  Serial.print("Data connectee apres networkDisconnect = ");
  Serial.println(dataConnected ? "OUI" : "NON");

  Serial.print("IP apres networkDisconnect = ");
  Serial.println(modem.ipAddress());

  return !dataConnected;
}

// ==========================================================================
// Cycle complet
// ==========================================================================
void fullTest() {
  printTitle("DEBUT TEST COMPLET TEL0177");

  testModemInfo();

  if (!testSIMAndNetwork()) {
    return;
  }

  if (!testNetworkConnect()) {
    return;
  }

  if (!testMQTTConnect()) {
    modem.networkDisconnect();
    return;
  }

  testMQTTSubscribePublish();

  if (!testMQTTDisconnect()) {
    Serial.println("Probleme pendant mqttdisconnect()");
    Serial.println("On tente quand meme networkDisconnect()");
    modem.networkDisconnect();
    return;
  }

  if (!testMQTTReconnect()) {
    Serial.println("Probleme pendant mqttreconnect()");
    modem.networkDisconnect();
    return;
  }

  if (!testNetworkDisconnect()) {
    Serial.println("Probleme pendant networkDisconnect()");
    return;
  }

  printTitle("FIN TEST COMPLET TEL0177");
}

// ==========================================================================
// SETUP
// ==========================================================================
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

  delay(2000);

  Serial.println();
  Serial.println("=============================================================");
  Serial.println("          TEL0177 - TEST CONNEXION / DECONNEXION");
  Serial.println("=============================================================");

  if (!modem.begin(Serial2)) {
    Serial.println("Erreur : modem non detecte");
    return;
  }

  Serial.println("Modem detecte");

  Serial.println("Envoi AT+RESET...");

  if (!modem.reset()) {
    Serial.println("Erreur : reset impossible");
    Serial.print("lastError = ");
    Serial.println((int)modem.lastError());
    Serial.print("lastResponse = ");
    Serial.println(modem.lastResponse());
    return;
  }

  Serial.println("Reset OK, attente stabilisation...");
  delay(15000);

  if (!modem.begin(Serial2)) {
    Serial.println("Erreur : modem non detecte apres reset");
    return;
  }

  modem.simUnlock(SIM_PIN);
  delay(5000);


  Serial.println("Modem pret apres reset");

  fullTest();
}

// ==========================================================================
// LOOP
// ==========================================================================
void loop() {
  modem.loop();
}