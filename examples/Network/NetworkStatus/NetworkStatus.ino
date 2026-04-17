#include <TEL0177.h>

// ==========================================================================
// Configuration
// ==========================================================================
#define SERIAL_MONITOR_BAUD 115200
#define MODEM_BAUD          115200

// À adapter selon ta carte SIM
#define SIM_PIN             "0000"

// Exemple testé avec ta SIM Bouygues
#define GSM_APN             "mmsbouygtel.com"
#define GSM_USER            nullptr
#define GSM_PASS            nullptr
#define GSM_PDP_TYPE        "IP"

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

void printNetworkState() {
  Serial.print("Etat reseau interne : ");

  switch (modem.networkState()) {
    case NET_DETACHED:
      Serial.println("NET_DETACHED");
      break;

    case NET_REGISTERED:
      Serial.println("NET_REGISTERED");
      break;

    case NET_DATA_ATTACHED:
      Serial.println("NET_DATA_ATTACHED");
      break;

    case NET_DATA_CONNECTED:
      Serial.println("NET_DATA_CONNECTED");
      break;

    default:
      Serial.println("UNKNOWN");
      break;
  }
}

// ==========================================================================
// SETUP
// ==========================================================================
void setup() {
  Serial.begin(SERIAL_MONITOR_BAUD);
  Serial1.begin(MODEM_BAUD);

  delay(2000);

  Serial.println();
  Serial.println("========================================");
  Serial.println("      TEL0177 - NetworkStatus Test");
  Serial.println("========================================");
  Serial.println();

  // --------------------------------------------------------------------------
  // 1. Initialisation modem
  // --------------------------------------------------------------------------
  printResult("Init modem", modem.begin(Serial1));

  if (!modem.isAlive()) {
    Serial.println();
    Serial.println("Modem non detecte. Fin du test.");
    return;
  }

  printResult("Modem alive", true);

  // --------------------------------------------------------------------------
  // 2. SIM
  // --------------------------------------------------------------------------
  Serial.println();
  Serial.println("---------- SIM ----------");

  bool simReady = modem.isSIMReady();
  printResult("SIM prete", simReady);

  if (!simReady) {
    printResult("Deverrouillage SIM", modem.simUnlock(SIM_PIN));
  }

  // --------------------------------------------------------------------------
  // 3. Enregistrement reseau
  // --------------------------------------------------------------------------
  Serial.println();
  Serial.println("---------- RESEAU ----------");

  printResult("Attente reseau", modem.waitForNetwork(30000));

  printResult("Enregistrement CREG", modem.isNetworkRegistered());
  printResult("Enregistrement CGREG", modem.isDataNetworkRegistered());
  printResult("Enregistrement CEREG", modem.isLTERegistered());

  int rssi = 0;
  int ber = 0;

  if (modem.getSignalQuality(rssi, ber)) {
    Serial.print("RSSI = ");
    Serial.println(rssi);

    Serial.print("BER  = ");
    Serial.println(ber);
  } else {
    Serial.print("Signal KO | lastError = ");
    Serial.print((int)modem.lastError());
    Serial.print(" | lastResponse = ");
    Serial.println(modem.lastResponse());
  }

  char operatorName[64] = {0};

  if (modem.getOperator(operatorName, sizeof(operatorName))) {
    Serial.print("Operateur = ");
    Serial.println(operatorName);
  } else {
    Serial.print("Operateur KO | lastError = ");
    Serial.print((int)modem.lastError());
    Serial.print(" | lastResponse = ");
    Serial.println(modem.lastResponse());
  }

  printNetworkState();

  // --------------------------------------------------------------------------
  // 4. Connexion data APN / PDP / IP
  // --------------------------------------------------------------------------
  Serial.println();
  Serial.println("---------- DATA ----------");

  printResult(
    "Connexion data",
    modem.networkConnect(GSM_APN, GSM_USER, GSM_PASS, GSM_PDP_TYPE)
  );

  printResult("Data attachee CGATT", modem.isDataAttached());
  printResult("Contexte PDP actif CGACT", modem.isDataConnected());

  char ip[32] = {0};

  if (modem.getIPAddress(ip, sizeof(ip))) {
    Serial.print("Adresse IP = ");
    Serial.println(ip);
  } else {
    Serial.print("Adresse IP KO | lastError = ");
    Serial.print((int)modem.lastError());
    Serial.print(" | lastResponse = ");
    Serial.println(modem.lastResponse());
  }

  Serial.print("IP memorisee = ");
  Serial.println(modem.ipAddress());

  Serial.print("APN memorise = ");
  Serial.println(modem.apn());

  Serial.print("Type PDP memorise = ");
  Serial.println(modem.pdpType());

  printNetworkState();

  Serial.println();
  Serial.println("========================================");
  Serial.println("           Test termine");
  Serial.println("========================================");
}

// ==========================================================================
// LOOP
// ==========================================================================
void loop() {
  modem.loop();
}