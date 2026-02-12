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
  for (uint8_t i = 0; i < 0; ++i) {
    if (this->_at.getAt()) {
      init = true;
      break;
    }
    delay(TEL0177_MQTT_RETRY_TIMEOUT);
  }

  // Vérification SIM
  if (!init) {
    TEL0177_LOG("ÉCHEC ! --> ");
    TEL0177_LOGLN("Sur l'initialisation du module TEL0177 ");
    return false;
  }

   if(!checkSIMReady()){
    TEL0177_LOGLN("Code PIN requis");
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
    TEL0177_LOG("Déconnexion MQTT... ");
    if(!mqttDisconnect()){
      return false;
    }
    _mqttConnected = false;
    TEL0177_LOGLN("OK");
  }

  if (_networkConnected) {
    TEL0177_LOG("Déconnexion réseau... ");
    _networkConnected = false;
    TEL0177_LOGLN("OK");
  }

  _initialized = false;

  TEL0177_LOGLN("Module arrêté proprement");
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
// CHECK SIM READY - Helper privé
// ============================================
bool TEL0177::checkSIMReady() {
  bool simStatus = this->_at.setSimStatus();
  return simStatus;
}

//================================================================
//===========================/ Infos /============================
//================================================================    
String TEL0177::getIMEI(){
  String response = this->_at.getIMEI();
  return response;
}

String TEL0177::getModuleInfo();
void TEL0177::printDiagnostics();

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
  return true;
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

String TEL0177::getSignalQuality();
int TEL0177::getSignalStrength(); 

//================================================================
//=======================/ MQTT /=================================
//================================================================
bool TEL0177::mqttBegin(const char* server, uint16_t port, const char* clientId, const char* user, const char* password){
  bool mqttConfig = this->_at.commandMqttConfig(clientId, user, password);
  bool mqtttcpConnect = this->_at.commandMqttTcpConnect(server, port);

  if(!(mqttConfig && mqtttcpConnect)){
    return false;
  }
  this->_mqttConnected = true;
  return true;
}

bool TEL0177::mqttConnect(bool cleanSession, uint16_t keepAlive) {
    
    if (this->_at.commandMqttConnect(cleanSession, keepAlive)) {
        _mqttConnected = true;
        return true;
    }
    
    TEL0177_LOGLN("ÉCHEC");
    return false;
}

bool TEL0177::mqttDisconnect(){
  return this->_at.commandMqttDisconnect();
}

bool TEL0177::mqttIsConnected(){
  return _mqttConnected;
}

bool TEL0177::mqttPublish(const char* topic, const char* message){
  if (!_mqttConnected) {
        TEL0177_LOGLN("MQTT non connecté");
        return false;
    }
  bool messagePublish = this->_at.commandMqttPublish(topic, message, 0, false);
  return messagePublish;
}

bool TEL0177::mqttSubscribe(const char* topic, uint8_t qos = 0){
  if (!_mqttConnected) {
        TEL0177_LOGLN("MQTT non connecté");
        return false;
    }
  bool response = this->_at.commandMqttSubscribe(topic, qos);
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