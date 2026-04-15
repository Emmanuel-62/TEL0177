#include "TEL0177_MQTT.h"


#include <string.h>
#include <stdio.h>

/**
 * 
 */
void TEL0177_MQTT::begin(TEL0177_AT_Core& at){
  this->_at = &at;
  this->_callback = nullptr;
  this->_rawCallback = nullptr;

  this->_connected = false;

  memset(this->_host,     0, sizeof(this->_host));
  memset(this->_clientId, 0, sizeof(this->_clientId));
  memset(this->_user,     0, sizeof(this->_user));
  memset(this->_pass,     0, sizeof(this->_pass));

  this->_port = TEL_MQTT_DEFAULT_PORT;
  this->_keepAlive = TEL_MQTT_KEEP_ALIVE;
}

/**
 * 
 */
void TEL0177_MQTT::setClientConfig(const char* clientId, const char* user, const char* pass, uint16_t keepAlive){
  if(!clientId || clientId[0] == '\0') return;

  memset(this->_clientId, 0, sizeof(this->_clientId));
  memset(this->_user,     0, sizeof(this->_user));
  memset(this->_pass,     0, sizeof(this->_pass));

  strncpy(this->_clientId, clientId, sizeof(_clientId) - 1);

  if(user) strncpy(this->_user,     user,     sizeof(_user)     - 1);
  if(pass) strncpy(this->_pass,     pass,     sizeof(_pass)     - 1);

  this->_keepAlive = keepAlive;
}

/**
 * 
 */
TEL_Status TEL0177_MQTT::_configCredentials(const char* clientId, const char* user, const char* pass){
  if (!this->_at || !clientId || clientId[0] == '\0') {
    return TEL_INVALID_PARAM;
  }

  char command[TEL_CMD_BUFFER_SIZE] = {0};
  int nb = snprintf(command, sizeof(command), "AT+MCONFIG=\"%s\",\"%s\",\"%s\"", clientId, user ? user : "", pass ? pass : "");

  if(nb < 0 || nb >= (int)sizeof(command)) {
    return TEL_BUFFER_OVERFLOW;
  }

  return (this->_at->send(command, TEL_TIMEOUT_MQTT));
}

/**
 * 
 */
TEL_Status TEL0177_MQTT::_configBroker(const char* host, uint16_t port){
  if(!this->_at || !host || host[0] == '\0'|| port == 0) {
    return TEL_INVALID_PARAM;
  }

  char command[TEL_CMD_BUFFER_SIZE] = {0};
  int nb = snprintf(command, sizeof(command), "AT+MIPSTART=\"%s\",\"%d\"", host, port);

  if(nb < 0 || nb >= (int)sizeof(command)) {
    return TEL_BUFFER_OVERFLOW;
  }

  return (this->_at->sendAndWait(command, "CONNECT OK", TEL_TIMEOUT_MQTT));
}

/**
 * 
 */
TEL_Status TEL0177_MQTT::connect(const char* host, uint16_t port, uint32_t timeout){
  if(!this->_at || !host || host[0] == '\0' || port == 0) {
    return TEL_INVALID_PARAM;
  }

  char command[TEL_CMD_BUFFER_SIZE] = {0};
  memset(this->_host, 0, sizeof(this->_host));

  strncpy(this->_host, host, sizeof(this->_host) - 1);
  this->_host[sizeof(this->_host) - 1] = '\0';
  this->_port = port;

  if(this->_clientId[0] == '\0') {
    return TEL_INVALID_PARAM;
  }

  TEL_Status res = this->_configCredentials(this->_clientId, this->_user, this->_pass);
  if(res != TEL_OK) return res;

  res = this->_configBroker(this->_host, this->_port);
  if(res != TEL_OK) return res;

  int nb = snprintf(command, sizeof(command), "AT+MCONNECT=%d,%d", TEL_MQTT_CLEAN_SESSION, this->_keepAlive);

  if(nb < 0 || nb >= (int)sizeof(command)) {
    return TEL_BUFFER_OVERFLOW;
  }

  res = this->_at->sendAndWait(command, "CONNACK OK", timeout);
  if(res != TEL_OK) return res ;

  this->_connected = true;
  return TEL_OK;
}

/**
 * 
 */
TEL_Status TEL0177_MQTT::disconnect(){
  if(!this->_at) return TEL_INVALID_PARAM;
  if(!this->_connected) return TEL_OK;

  TEL_Status res = this->_at->send("AT+MDISCONNECT", TEL_TIMEOUT_MQTT);
  if(res != TEL_OK) return res;

  this->_connected = false;
  return TEL_OK;
}

/**
 * 
 */
bool TEL0177_MQTT::isConnected(){
  if(!this->_at) return false;

  char res[TEL_LINE_BUFFER_SIZE] = {0};
  TEL_Status s = this->_at->sendAndReadRaw("AT+MQTTSTATU", res, sizeof(res), TEL_TIMEOUT_MQTT);

  if(s != TEL_OK) {
    this->_connected = false;
    return false;
  }

  this->_connected = strstr(res, "MQTTSTATU :1") != nullptr ||
                     strstr(res, "MQTTSTATU:1") != nullptr;

  return this->_connected;
}

/**
 * 
 */
TEL_Status TEL0177_MQTT::reconnect() {
  if(!this->_at) return TEL_INVALID_PARAM;
  if(this->_host[0] == '\0' || this->_port == 0) return TEL_INVALID_PARAM;

  if(this->_connected) {
    TEL_Status d = this->disconnect();
    if(d != TEL_OK) return d;
  }

  return this->connect(this->_host, this->_port, TEL_TIMEOUT_MQTT);
}

/**
 * 
 */
TEL_Status TEL0177_MQTT::publish(const char* topic, const char* payload, uint8_t qos, bool retain, uint32_t timeout){
  if(!this->_at || !topic || !payload || topic[0] == '\0' ) {
    return TEL_INVALID_PARAM;
  }

  if(!this->_connected) return TEL_NOT_CONNECTED;
  if(qos > 1) return TEL_INVALID_PARAM;
  char command[TEL_CMD_BUFFER_SIZE] = {0};

  int n = snprintf(command, sizeof(command), "AT+MPUB=\"%s\",%d,%d,\"%s\"", topic, qos, retain ? 1 : 0, payload);

  if (n < 0 || n >= (int)sizeof(command)) return TEL_BUFFER_OVERFLOW;

  return this->_at->send(command, timeout);
}

/**
 * 
 */
TEL_Status TEL0177_MQTT::subscribe(const char* topic, uint8_t qos){
  if(!this->_at || !topic || topic[0] == '\0' ) return TEL_INVALID_PARAM;
  if(!this->_connected) return TEL_NOT_CONNECTED;
  if(qos > 1) return TEL_INVALID_PARAM;

  char command[TEL_CMD_BUFFER_SIZE] = {0};

  int n = snprintf(command, sizeof(command), "AT+MSUB=\"%s\",%d", topic, qos);
  if (n < 0 || n >= (int)sizeof(command)) return TEL_BUFFER_OVERFLOW;

  return this->_at->sendAndWait(command, "SUBACK", TEL_TIMEOUT_MQTT);
}

/**
 * 
 */
TEL_Status TEL0177_MQTT::unsubscribe(const char* topic){
  if(!this->_at || !topic || topic[0] == '\0' ) return TEL_INVALID_PARAM;
  if(!this->_connected) return TEL_NOT_CONNECTED;

  char command[TEL_CMD_BUFFER_SIZE] = {0};

  int n = snprintf(command, sizeof(command), "AT+MUNSUB=\"%s\"", topic);
  if (n < 0 || n >= (int)sizeof(command)) return TEL_BUFFER_OVERFLOW;

  return this->_at->sendAndWait(command, "UNSUBACK", TEL_TIMEOUT_MQTT);
}

/**
 * 
 */
void TEL0177_MQTT::setCallback(TEL_MQTT_Callback callback){
  this->_callback = callback;
}

/**
 * 
 */
void TEL0177_MQTT::setRawCallback(TEL_MQTT_RawCallback callback){
  this->_rawCallback = callback;
}

/**
 * 
 */
void TEL0177_MQTT::loop(){
  if(!this->_at) return;
  if(!this->_at->isAvailable()) return;

  char line[TEL_LINE_BUFFER_SIZE] = {0};

  if(this->_at->readLine(line, sizeof(line), 100)) {

    // Le rawCallback reçoit toutes les lignes MQTT importantes
    if(this->_rawCallback) {
      this->_rawCallback(line);
    }

    if(strstr(line, "+MSUB:")) {
      _handleRecv(line);
    }
    else if(strstr(line, "+MQTTDISCONN:")) {
      this->_connected = false;
    }
  }
}

/**
 * 
 */
void TEL0177_MQTT::loopFor(uint32_t durationMs) {
  uint32_t start = millis();
  while (millis() - start < durationMs) {
    loop();
  }
}

/**
 * 
 */
bool TEL0177_MQTT::_parseIncomingMessage(const char* urc, TEL_MQTT_Message& msg) {
  if (!urc) return false;

  memset(&msg, 0, sizeof(msg));

  const char* p = strstr(urc, "+MSUB:");
  if (!p) return false;

  p = strchr(p, ':');
  if (!p) return false;
  p++;

  while (*p == ' ') p++;

  // Topic
  const char* topicStart = p;
  const char* topicEnd = strchr(topicStart, ',');
  if (!topicEnd) return false;

  uint16_t topicLen = topicEnd - topicStart;

  // Supprime les guillemets éventuels
  if (topicLen >= 2 && topicStart[0] == '"' && topicStart[topicLen - 1] == '"') {
    topicStart++;
    topicLen -= 2;
  }

  if (topicLen >= sizeof(msg.topic)) {
    topicLen = sizeof(msg.topic) - 1;
  }

  strncpy(msg.topic, topicStart, topicLen);
  msg.topic[topicLen] = '\0';

  // Length
  p = topicEnd + 1;
  while (*p == ' ') p++;

  uint16_t payloadLen = atoi(p);

  const char* payloadStart = strchr(p, ',');
  if (!payloadStart) return false;
  payloadStart++;

  while (*payloadStart == ' ') payloadStart++;

  // Payload
  uint16_t maxPayload = sizeof(msg.payload) - 1;
  if (payloadLen > maxPayload) {
    payloadLen = maxPayload;
  }

  strncpy(msg.payload, payloadStart, payloadLen);
  msg.payload[payloadLen] = '\0';

  msg.length = payloadLen;
  msg.qos = 0;

  return true;
}

/**
 * 
 */
void TEL0177_MQTT::_handleRecv(const char* urc) {
  if (!urc || !this->_callback) return;

  TEL_MQTT_Message msg;

  if (!this->_parseIncomingMessage(urc, msg)) {
    return;
  }

  this->_callback(msg.topic, msg.payload, msg.length);
}