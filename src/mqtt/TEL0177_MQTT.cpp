#include "TEL0177_MQTT.h"


#include <string.h>
#include <stdio.h>


// ================================================================================================
// 
// ================================================================================================
/** @brief 
 *  @param at 
 *  @return TEL_Status
*/
void TEL0177_MQTT::begin(TEL0177_AT_Core & at){
  this->_at         = &at;
  this->_callback   = nullptr;
  this->_connected  = false;

  memset(_host, 0, sizeof(_host));
  memset(_clientId, 0, sizeof(_clientId));
  memset(_user, 0, sizeof(_user));
  memset(_pass, 0, sizeof(_pass));

  this->_port       = TEL_MQTT_DEFAULT_PORT;
  this->_keepAlive  = TEL_MQTT_KEEP_ALIVE;
}

/** @brief 
 *  @param at 
 *  @return TEL_Status
*/
TEL_Status TEL0177_MQTT::connect(const char* host, uint16_t port, uint32_t timeout){
  if(!host || !this->_clientId[0]) return TEL_INVALID_PARAM;
  
  strncpy(this->_host, host, sizeof(_host) - 1);
  this->_port = port;

  TEL_Status response = _configCredentials(this->_clientId, this->_user, this->_pass);
  if(response != TEL_OK) return response;

  response = _configBroker(host, port);
  if(response != TEL_OK) return response;

  char command[TEL_CMD_BUFFER_SIZE];
  snprintf(command, sizeof(command), "AT+MCONNECT=1,%d", this->_keepAlive);

  response = this->_at->send(command, timeout);
  if(response != TEL_OK) return response;

  if(!this->_at->waitFor("CONNACK OK", timeout)) return TEL_TIMEOUT;

  this->_connected = true;
  return TEL_OK;
}

/** @brief 
 *  @param at 
 *  @return TEL_Status
*/
void TEL0177_MQTT::setServer(const char* clientId, const char* user, const char* pass, uint16_t keepAlive){
  if(clientId) strncpy(this->_clientId, clientId, sizeof(_clientId) - 1);
  if(user)     strncpy(this->_user,     user,     sizeof(_user)     - 1);
  if(pass)     strncpy(this->_pass,     pass,     sizeof(_pass)     - 1);

  this->_keepAlive = keepAlive;
}

/** @brief 
 *  @return TEL_Status
*/
TEL_Status TEL0177_MQTT::disconnect(){
  TEL_Status response = this->_at->send("AT+MDISCONNECT");
  if (response != TEL_OK) return response;

  response = this->_at->send("AT+MIPCLOSE");
  if (response != TEL_OK) return response;

  this->_connected = false;
  return response;
}

/** @brief 
 *  @return TEL_Status
*/
bool TEL0177_MQTT::isConnected(){
  if(!this->_connected) return false;

  char response[TEL_LINE_BUFFER_SIZE] = {0};
  this->_at->sendAndRead("AT+MQTTSTATU", response, sizeof(response));

  this->_connected = (strstr(response, "MQTTSTATU :1") != nullptr);

  return this->_connected;
}

/** @brief 
 *  
 *  @return TEL_Status
 */
TEL_Status TEL0177_MQTT::reconnect() {
    if (this->_connected) {
        TEL_Status resp = this->disconnect();
        if (resp != TEL_OK) return resp;
    }

    TEL_Status resp = _configBroker(this->_host, this->_port);
    if (resp != TEL_OK) return resp;

    resp = _configCredentials(this->_clientId, this->_user, this->_pass);
    if (resp != TEL_OK) return resp;

    char command[TEL_CMD_BUFFER_SIZE];
    snprintf(command, sizeof(command), "AT+MCONNECT=1,%d", this->_keepAlive);

    resp = this->_at->sendAndWait(command, "CONNACK OK", TEL_TIMEOUT_MQTT);
    if (resp != TEL_OK) return resp;

    this->_connected = true;
    return TEL_OK;
}

/** @brief 
 *  @param at 
 *  @return TEL_Status
*/
TEL_Status TEL0177_MQTT::unsubscribe(const char* topic){
  if(!topic) return TEL_INVALID_PARAM;
  if(!this->_connected) return TEL_NOT_CONNECTED;

  char command[TEL_CMD_BUFFER_SIZE];
  snprintf(command, sizeof(command), "AT+MUNSUB=\"%s\"", topic);

  return this->_at->sendAndWait(command, "UNSUBACK", TEL_TIMEOUT_MQTT);

}

/** @brief 
 *  @param at 
 *  @return TEL_Status
*/
TEL_Status TEL0177_MQTT::subscribe(const char* topic, uint8_t qos){
  if(!topic) return TEL_INVALID_PARAM;
  if(!this->_connected) return TEL_NOT_CONNECTED;

  char command[TEL_CMD_BUFFER_SIZE];
  snprintf(command, sizeof(command), "AT+MSUB=\"%s\", %d", topic, qos);

  return this->_at->sendAndWait(command, "SUBACK", TEL_TIMEOUT_MQTT);
}

/** @brief 
 *  @param at 
 *  @return TEL_Status
*/
TEL_Status TEL0177_MQTT::publish(const char* topic ,const char* payload ,uint8_t qos, bool retain, uint32_t timeout){
  if(!topic || !payload) return TEL_INVALID_PARAM;
  if(!this->_connected) return TEL_NOT_CONNECTED;

  char command[TEL_CMD_BUFFER_SIZE];
  snprintf(command, sizeof(command), 
          "AT+MPUB=\"%s\",%d, %d, \"%s\"",
          topic, qos, retain ? 1 : 0, payload);

  TEL_Status response = this->_at->sendAndWait(command, "OK", timeout);
  if(response != TEL_OK) return response;

  if(qos >= 1){
    if (!this->_at->waitFor("PUBACK", timeout)) return TEL_TIMEOUT;
  }

  return TEL_OK;
}

/** @brief 
 *  @param at 
 *  @return TEL_Status
*/
void TEL0177_MQTT::loop(){
  if(!this->_at->isAvailable()) return;

  char line[TEL_LINE_BUFFER_SIZE] = {0};

  if(this->_at->readLine(line, sizeof(line), 100)){
    if(strstr(line, "+MSUB:")) {
      _handleRecv(line);
    }
    else if(strstr(line, "+MQTTDISCONN:")) {
      this->_connected = false;
    }
  }
}


/** @brief 
 *  @param callback 
 *  @return TEL_Status
*/
void TEL0177_MQTT::setCallback(TEL_MQTT_Callback callback){
  this->_callback = callback;
}


// ================================================================================================
// FONCTIONS PRIVEES
// ================================================================================================
/** @brief 
 *  @param command 
 *  @param timeout 
 *  @return TEL_Status
*/
TEL_Status TEL0177_MQTT::_configBroker(const char* host, uint16_t port){
  char command[TEL_CMD_BUFFER_SIZE];

  snprintf(command, sizeof(command), "AT+MIPSTART=\"%s\",\"%d\"", host, port);

  TEL_Status s = this->_at->send(command, TEL_TIMEOUT_MQTT);
  if(s != TEL_OK) return s;

  if(!this->_at->waitFor("CONNECT OK", TEL_TIMEOUT_MQTT)) return TEL_TIMEOUT;

  return TEL_OK;
}


/** @brief 
 *  @param command 
 *  @param timeout 
 *  @return TEL_Status
*/
TEL_Status TEL0177_MQTT::_configCredentials(const char* clientId, const char* user, const char* pass){

  if(!clientId) return TEL_INVALID_PARAM;

  char command[TEL_CMD_BUFFER_SIZE];

  snprintf(command, sizeof(command), "AT+MCONFIG=\"%s\",\"%s\",\"%s\"",
           clientId,
           user ? user : "",
           pass ? pass : "");

  return this->_at->sendAndWait(command, "OK", TEL_TIMEOUT_MQTT);
}


/** @brief 
 *  @param urc 
 *  @return void
*/
void TEL0177_MQTT::_handleRecv(const char* urc) {
    if (!_callback) return;

    char topic[128]   = {0};
    char payload[512] = {0};

    const char* start = strchr(urc, ':');
    if (!start) return;
    start++;
    while (*start == ' ') start++;

    const char* tStart = strchr(start, '"');
    if (!tStart) return;
    tStart++;
    const char* tEnd = strchr(tStart, '"');
    if (!tEnd) return;

    uint16_t tLen = (uint16_t)(tEnd - tStart);
    if (tLen >= sizeof(topic)) tLen = sizeof(topic) - 1;
    strncpy(topic, tStart, tLen);
    topic[tLen] = '\0';

    const char* p = tEnd + 1;
    p = strchr(p, ',');
    if (!p) return;
    p++; 
    p = strchr(p, ',');
    if (!p) return;
    p++;

    uint16_t pLen = strlen(p);
    while (pLen > 0 && (p[pLen-1] == '\r' || p[pLen-1] == '\n')) pLen--;
    if (pLen >= sizeof(payload)) pLen = sizeof(payload) - 1;
    strncpy(payload, p, pLen);
    payload[pLen] = '\0';

    //TEL_DEBUG("[MQTT] RCV ← ");
    //TEL_DEBUGLN(topic);

    this->_callback(topic, payload, pLen);
}