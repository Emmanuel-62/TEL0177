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

  snprintf(command, sizeof(command), "AT+MIPSTART=\"%s\",%d", host, port);

  return this->_at->send(command);
}


/** @brief 
 *  @param command 
 *  @param timeout 
 *  @return TEL_Status
*/
TEL_Status TEL0177_MQTT::_configCredentials(const char* clientId, const char* user, const char* pass){

  if(!clientId && !user || !pass) return TEL_INVALID_PARAM;

  char command[TEL_CMD_BUFFER_SIZE];

  snprintf(command, sizeof(command), "AT+MCONFIG=\"%s\" , \"%s\" , \"%s\"", clientId, user, pass);

  return this->_at->send(command);
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

    _callback(topic, payload, pLen);
}