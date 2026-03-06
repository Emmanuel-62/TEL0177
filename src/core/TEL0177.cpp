#include "TEL0177.h"

// ================================================================================================
// CONSTRUCTEUR
// ================================================================================================
bool TEL0177::begin(Stream &serial){
  this->_at.begin(serial);

  //sms.begin(_at);
  //http.begin(_at);
  this->_mqtt.begin(_at);
  this->_network.begin(_at);

  delay(TEL_TIMEOUT_BOOT);

  uint8_t retries = 0;

  while(!this->_at.isAlive() && retries < TEL_MAX_RETRIES){
    delay(TEL_RETRY_DELAY_MS);
    retries++;
  }

  if(retries >= TEL_MAX_RETRIES){
    return false;
  }

  return true;
}

// ================================================================================================
// 
// ================================================================================================
/** @brief 
 *  @param timeout 
 *  @return 
*/
void TEL0177::loop(){
  this->_mqtt.loop();
}

/** @brief 
 *  @param timeout 
 *  @return 
*/
bool TEL0177::reset(){
  TEL_Status s = _at.send("AT+RESET", TEL_TIMEOUT_NETWORK);
  if (s == TEL_OK) {
    delay(TEL_TIMEOUT_BOOT);
    return true;
  }
  return false;
}

/** @brief 
 *  @param timeout 
 *  @return 
*/
bool TEL0177::isAlive(){
  return this->_at.isAlive();
}

bool TEL0177::simUnlock(const char * pin_code){
  if(!pin_code) return false;

  TEL_Status response = this->_network.unlockSIM(pin_code);

  if(response != TEL_OK) {
    _display("simUnlock", response);
    return false;
  }

  return true;
}

// ================================================================================================
// MQTT
// ================================================================================================

bool TEL0177::mqttsetServer(const char* clientId, const char* user, const char* pass, uint16_t keepAlive){
  if(!clientId) return false;
  this->_mqtt.setServer(clientId, user, pass, keepAlive);
  return true;
}

bool TEL0177::mqttconnect(const char* host, uint16_t port, uint32_t timeout){
  if(!host) return false;
  TEL_Status s = this->_mqtt.connect(host, port, timeout);
  if(s != TEL_OK){
    _display("mqttconnect", s);
    return false;
  }
  return true;
}

bool TEL0177::mqttreconnect(){
  TEL_Status s = this->_mqtt.reconnect();
  if(s != TEL_OK){
    _display("mqttreconnect", s);
    return false;
  }
  return true;
}

bool TEL0177::mqttdisconnect(){
  TEL_Status s = this->_mqtt.disconnect();
  if(s != TEL_OK){
    _display("mqttdisconnect", s);
    return false;
  }
  return true;
}

bool TEL0177::mqttisConnected(){
  return this->_mqtt.isConnected();
}

bool TEL0177::mqttpublish(const char* topic, const char* payload, uint8_t qos, bool retain, uint32_t timeout){
  if(!topic || !payload) return false;
  TEL_Status s = this->_mqtt.publish(topic, payload, qos, retain, timeout);
  if(s != TEL_OK){
    _display("mqttpublish", s);
    return false;
  }
  return true;
}

bool TEL0177::mqttsubscribe(const char* topic, uint8_t qos){
  if(!topic) return false;
  TEL_Status s = this->_mqtt.subscribe(topic, qos);
  if(s != TEL_OK){
    _display("mqttsubscribe", s);
    return false;
  }
  return true;
}

bool TEL0177::mqttunsubscribe(const char* topic){
  if(!topic) return false;
  TEL_Status s = this->_mqtt.unsubscribe(topic);
  if(s != TEL_OK){
    _display("mqttunsubscribe", s);
    return false;
  }
  return true;
}

bool TEL0177::mqttsetCallback(TEL_MQTT_Callback callback){
  if(!callback) return false;
  this->_mqtt.setCallback(callback);
  return true;
}

// ================================================================================================
// FONCTIONS PRIVEES
// ================================================================================================

/** @brief 
 *  @param timeout 
 *  @return 
*/
void TEL0177::_display(const char* expression, TEL_Status code_error){
  switch(code_error) {
    case TEL_ERROR:
      TEL_LOGLN_ERROR("ERROR --> %s de type TEL_Status : TEL_ERROR", expression);
      break;
    case TEL_TIMEOUT:
      TEL_LOGLN_ERROR("ERROR --> %s de type TEL_Status : TEL_TIMEOUT", expression);
      break;
    case TEL_BUSY:
      TEL_LOGLN_ERROR("ERROR --> %s de type TEL_Status : TEL_BUSY", expression);
      break;
    case TEL_NO_RESPONSE:
      TEL_LOGLN_ERROR("ERROR --> %s de type TEL_Status : TEL_NO_RESPONSE", expression);
      break;
    case TEL_INVALID_PARAM:
      TEL_LOGLN_ERROR("ERROR --> %s de type TEL_Status : TEL_INVALID_PARAM", expression);
      break;
    case TEL_NOT_CONNECTED:
      TEL_LOGLN_ERROR("ERROR --> %s de type TEL_Status : TEL_NOT_CONNECTED", expression);
      break;
    default:
      TEL_LOGLN_ERROR("ERROR --> %s de type TEL_Status : No specific", expression);
  }
}