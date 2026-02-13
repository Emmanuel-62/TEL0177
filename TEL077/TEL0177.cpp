#include "TEL0177.h"

// ============================================
// CONSTRUCTEUR
// ============================================
TEL0177::TEL0177(Stream& serial)
  : _at(serial), 
    _initialized(false),
    _networkConnected(false),
    _mqttConnected(false) {}

// ============================================
// INITIALISATION COMPLÈTE
// ============================================
bool TEL0177::begin(uint32_t baudRate) {
  bool init = false;

  // Test communication AT (3 tentatives)
  for (uint8_t i = 0; i < 3; ++i) {
    if (this->_at.getAt()) {
      init = true;
      break;
    }
    delay(TEL0177_MQTT_RETRY_TIMEOUT);
  }

  if (!init) {
    TEL0177_LOG("ÉCHEC ! --> ");
    TEL0177_LOGLN("Sur l'initialisation du module TEL0177 ");
    return false;
  }
  // Vérification de la SIM
   if(!checkSIMReady()){
    TEL0177_LOGLN("SIM est absente du Module !!!");
  }
  delay(500);

  this->_initialized = true;
  return true;
}

// ============================================
// END - Arrêt propre
// ============================================
bool TEL0177::end() {
  if (!this->_initialized) {
    return false;
  }

  if (_mqttConnected) {
    mqttDisconnect();
    _mqttConnected = false;
  }

  if (_networkConnected) {
    _networkConnected = false;
  }

  _initialized = false;

  TEL0177_LOGLN("Module arrêté proprement");
  return true;
}

// ============================================
// IS READY - Vérification état
// ============================================
bool TEL0177::isReady(){
  if(!_initialized){
    return false;
  }

  if(!this->_at.getAt()){
    _initialized = false;
    return false;
  }

  if(!checkSIMReady()){
    return false;
  }

  return true;
}

// ============================================
// CHECK SIM READY
// ============================================
bool TEL0177::checkSIMReady() {
  String simStatus = _at.getSimStatus();  
  return (simStatus.indexOf("READY") >= 0);  
}

//================================================================
//===========================/ Infos /============================
//================================================================    
String TEL0177::getIMEI(){
  String response = this->_at.getIMEI();
  return response;
}

String TEL0177::getModuleInfoString() {
    String infos = "";
    
    infos += "Manufacturer -> " + _at.getManufacturer() + "\n"; 
    infos += "Model -> " + _at.getModel() + "\n";
    infos += "Firmware -> " + _at.getFirmwareVersion() + "\n";
    infos += "Hardware -> " + _at.getHwVersion() + "\n";
    infos += "IMEI -> " + _at.getIMEI() + "\n";
    infos += "ICCID -> " + _at.getICCID() + "\n";
    
    if (checkSIMReady()) {
        infos += "IMSI -> " + _at.getIMSI() + "\n";
    } else {
        infos += "IMSI -> (PIN requis)\n";
    }
    
    return infos;
}

void TEL0177::printDiagnostics() {
    Serial.println("\n========== Diagnostics TEL0177 ==========");
    
    // Module
    Serial.println("--- Module ---");
    Serial.print("Fabricant : "); Serial.println(_at.getManufacturer());
    Serial.print("Modèle    : "); Serial.println(_at.getModel());
    Serial.print("Firmware  : "); Serial.println(_at.getFirmwareVersion());
    Serial.print("IMEI      : "); Serial.println(_at.getIMEI());
    
    // SIM
    Serial.println("\n--- SIM ---");
    Serial.print("ICCID     : "); Serial.println(_at.getICCID());
    Serial.print("IMSI      : "); Serial.println(_at.getIMSI());
    Serial.print("État      : "); Serial.println(_at.getSimStatus());
    
    // Réseau
    Serial.println("\n--- Réseau ---");
    Serial.print("Enregistrement : "); Serial.println(_at.getNetworkRegStatus());
    Serial.print("Signal         : "); Serial.println(_at.getSignalQuality());
    Serial.print("Data           : "); Serial.println(_at.getDataAttachStatus());
    
    // États
    Serial.println("\n--- États ---");
    Serial.print("Initialisé     : "); Serial.println(_initialized ? "Oui" : "Non");
    Serial.print("Réseau connecté: "); Serial.println(_networkConnected ? "Oui" : "Non");
    Serial.print("MQTT connecté  : "); Serial.println(_mqttConnected ? "Oui" : "Non");
    
    Serial.println("=========================================\n");
}

//================================================================
//===========================/ SIM /==============================
//================================================================
bool TEL0177::isSimReady() {
    return checkSIMReady();
}

bool TEL0177::unlockSim(const String& pin){
  bool response = this->_at.setSimPin(pin);
  return response;
}

//================================================================
//===========================/ Réseau /===========================
//================================================================
bool TEL0177::connectNetwork(uint32_t timeout){
  if(!_initialized){return false;}

  if(!checkSIMReady()){return false;}

  unsigned long start = millis();
  while(millis() - start < timeout){
    String status = this->_at.getNetworkRegStatus();
    if(status.indexOf(",1") >= 0 || status.indexOf(",5") >= 0){
      break;
    }
    if(status.indexOf(",3") >= 0){
      return false;
    }
    delay(TEL0177_AT_TIMEOUT);
  }

  if (millis() - start >= timeout) {
    return false;
  }
  start = millis();

  while(millis() - start < timeout){
    String attachStatus = this->_at.getDataAttachStatus();

    if(attachStatus.indexOf("1") >= 0){
      _networkConnected = true;
      return true;
    }
    
    delay(TEL0177_AT_TIMEOUT);
  }

  return false;
}


bool TEL0177::isNetworkConnected(){
  if (!_networkConnected) return false;

  String status = this->_at.getNetworkRegStatus();
  bool connected = (status.indexOf(",1") >= 0 || status.indexOf(",5") >= 0);

  if(!connected){
    _networkConnected = false;
  }
  return connected;
}

//================================================================
//=======================/ MQTT /=================================
//================================================================
bool TEL0177::mqttBegin(const char* server, uint16_t port, const char* clientId, const char* user, const char* password){
  bool mqttConfig = this->_at.mqttConfig(clientId, user, password);
  bool mqtttcpConnect = this->_at.mqttTcpConnect(server, port);

  if(!(mqttConfig && mqtttcpConnect)){
    return false;
  }
  this->_mqttConnected = true;
  return true;
}

bool TEL0177::mqttConnect(bool cleanSession, uint16_t keepAlive) {
    
    if (this->_at.mqttConnect(cleanSession, keepAlive)) {
        _mqttConnected = true;
        return true;
    }
    
    TEL0177_LOGLN("ÉCHEC");
    return false;
}

bool TEL0177::mqttDisconnect(){
  return this->_at.mqttDisconnect();
}

bool TEL0177::mqttIsConnected(){
  return _mqttConnected;
}

bool TEL0177::mqttPublish(const char* topic, const char* message){
  if (!_mqttConnected) {
        TEL0177_LOGLN("MQTT non connecté");
        return false;
    }
  bool messagePublish = this->_at.mqttPublish(topic, message, 0, false);
  return messagePublish;
}

bool TEL0177::mqttSubscribe(const char* topic, uint8_t qos){
  if (!_mqttConnected) {
        TEL0177_LOGLN("MQTT non connecté");
        return false;
    }
  bool response = this->_at.mqttSubscribe(topic, qos);
  return response;
}

//================================================================
//=======================/ UTILITAIRES /==========================
//================================================================
bool TEL0177::reset(){
  return this->_at.reset();
}
String TEL0177::sendATCommand(const char* cmd){
  String response = this->_at.sendCommand(cmd);
  return response;
}