#include "TEL0177_Network.h"

// ================================================================================================
// FONCTIONS PRIVEES
// ================================================================================================
/** @brief 
 *  @param at 
 *  @return void
*/
void TEL0177_Network::begin(TEL0177_AT_Core& at) {
  this->_at    = &at;
  this->_state = NET_DETACHED;
  memset(this->_ip, 0, sizeof(_ip));
  memset(this->_apn, 0, sizeof(_apn));
}


// ================================================================================================
// 
// ================================================================================================
/** @brief 
 *  @param pin 
 *  @return TEL_Status
*/
TEL_Status TEL0177_Network::unlockSIM(const char* pin){
  if(!pin) return TEL_INVALID_PARAM;

  char response [TEL_LINE_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndRead("AT+CPIN?", response, sizeof(response));
  if(s != TEL_OK) return s;

  if(strstr(response, "READY")) return TEL_OK;

  if(strstr(response, "SIM PIN")) {
    char command[TEL_CMD_BUFFER_SIZE];
    snprintf(command, sizeof(command), "AT+CPIN=\%s\"", pin);

    return this->_at->send(command);
  }
}

/** @brief 
 *  @param timeout 
 *  @return TEL_Status
*/
TEL_Status TEL0177_Network::attach(uint32_t timeout){
  TEL_Status s = _waitForNetworkReg(timeout);

  if(s != TEL_OK) return s;

  s = this->_at->send("AT+CGATT=1", timeout);
  if(s != TEL_OK) return s;

  this->_state = NET_ATTACHED;

  return TEL_OK;
}

/** @brief 
 *  @param timeout 
 *  @return TEL_Status
*/
TEL_Status TEL0177_Network::detach(){
  TEL_Status s = this->_at->send("AT+CGATT=0");
  if(s == TEL_OK){
    this->_state = NET_DETACHED;
    return TEL_OK;
  }

  return s;
}

/** @brief 
 *  @param timeout 
 *  @return TEL_Status
*/
bool TEL0177_Network::isAttached(){
  char response[TEL_LINE_BUFFER_SIZE] = {0};

  TEL_Status s = this->_at->sendAndRead("AT+CGATT=0", response, sizeof(response));
  if(s != TEL_OK){
    return false;
  }
  return (strstr(response, "+CGATT: 1") != nullptr);
}


// ================================================================================================
// FONCTIONS PRIVEES
// ================================================================================================
/** @brief 
 *  @param command 
 *  @param timeout 
 *  @return TEL_Status
*/
TEL_Status TEL0177_Network::_configureAPN(const char* apn){
  char command[TEL_CMD_BUFFER_SIZE];

  snprintf(command, sizeof(command), "AT+CGDCONT=%d,\"IP\",\"%s\"", TEL_PDP_CONTEXT_ID, apn);

  strncpy(this->_apn, apn, sizeof(apn) - 1);
  return this->_at->send(command);
}

/** @brief 
 *  @param timeout 
 *  @return TEL_Status
*/
TEL_Status TEL0177_Network::_waitForNetworkReg(uint32_t timeout){
  char response[TEL_LINE_BUFFER_SIZE] = {0};
  uint32_t start = millis();

  while(millis() - start < timeout){
    this->_at->sendAndRead("AT+CREG?", response, sizeof(response));

    if(strstr(response, ",1") || strstr(response, ",5")) return TEL_OK;

    delay(TEL_RETRY_DELAY_MS);
  }

  return TEL_TIMEOUT;
}
