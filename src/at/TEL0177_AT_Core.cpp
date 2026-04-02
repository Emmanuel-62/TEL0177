#include "TEL0177_AT_Core.h"

#include <Arduino.h>

// ================================================================================================
// CONSTRUCTEUR
// ================================================================================================
/*
 *
 */
 /*
TEL0177_AT_Core::TEL0177_AT_Core():_serial(0) {
  this->_serial = new Stream();
  this->_rxBuffer[];
}
*/

// ================================================================================================
// INITIALISATION
// ================================================================================================
/** @brief 
 *  @param serial 
 *  @return void
*/
void TEL0177_AT_Core::begin(Stream &serial) {
  this->_serial = &serial;
  flush();
}


// ================================================================================================
// ENVOIE
// ================================================================================================
/** @brief 
 *  @param command 
 *  @param timeout 
 *  @return 
*/
TEL_Status TEL0177_AT_Core::send(const char* command, uint32_t timeout) {
  if(!this->_serial || !command) return TEL_INVALID_PARAM;

  flush();

  this->_serial->print(command);
  this->_serial->print("\r\n");

  return _waitForOKorERROR(timeout);
}

/** @brief 
 *  @param command 
 *  @param expected 
 *  @param timeout 
 *  @return 
*/
TEL_Status TEL0177_AT_Core::sendAndWait(const char* command, const char* expected, uint32_t timeout){
  if(!this->_serial || !command || !expected) return TEL_INVALID_PARAM;

  flush();

  this->_serial->print(command);
  this->_serial->print("\r\n");

  return waitFor(expected, timeout) ? TEL_OK : TEL_TIMEOUT;
}

/** @brief 
 *  @param command 
 *  @param buffer 
 *  @param bufferSize 
 *  @param timeout 
 *  @return 
*/
TEL_Status TEL0177_AT_Core::sendAndRead(const char* command, const char* buffer, uint16_t bufferSize, uint32_t timeout){
  if(!this->_serial || !command || !buffer) return TEL_INVALID_PARAM;

  flush();

  this->_serial->print(command);
  this->_serial->print("\r\n");

  uint16_t len = readAll(buffer, bufferSize, timeout);

  if(len == 0) return TEL_NO_RESPONSE;

  if(_contains(buffer, "ERROR")) return TEL_ERROR;

  return TEL_OK;
}

/** @brief 
 *  @param data 
 *  @param timeout 
 *  @return 
*/
TEL_Status TEL0177_AT_Core::sendRaw(const char* data, uint16_t length){
  if(!this->_serial || !data) return TEL_INVALID_PARAM;

  if(length == 0) length = strlen(data);

  this->_serial->write((const uint8_t*)data, length);

  return TEL_OK;
}

// ================================================================================================
// LECTURE
// ================================================================================================
/** @brief 
 *  @param expected 
 *  @param timeout 
 *  @return 
*/
bool TEL0177_AT_Core::waitFor(const char* expected, uint32_t timeout){
  if(!this->_serial || !expected) return false;

  uint16_t idx   = 0;
  uint32_t start = millis();

  memset(this->_rxBuffer, 0, sizeof(this->_rxBuffer));

  while(millis() - start < timeout){
    if(this->_serial->available()){
      char c = (char)this->_serial->read();

      if(idx < TEL_RX_BUFFER_SIZE - 1){
        this->_rxBuffer[idx++] = c;
        this->_rxBuffer[idx]   = '\0';
      }

      if(_contains(this->_rxBuffer, expected)) {
        return true;
      }
    }
  }

  return false;
}

/** @brief 
 *  @param buffer 
 *  @param size 
 *  @param timeout 
 *  @return 
*/
bool TEL0177_AT_Core::readLine(char* buffer, uint16_t size, uint32_t timeout){
  if(!this->_serial || !buffer) return false;

  memset(buffer, 0, size);

  uint16_t idx   = 0;
  uint32_t start = millis();

  while((millis() - start) < timeout){
    if(this->_serial->available()){
      char c = (char)this->_serial->read();

      if(c == '\n'){
        if(idx > 0 && buffer[idx - 1] == '\r'){
          buffer[idx - 1] = '\0';
        }
        return (idx > 0);
      }

      if(idx < size - 1){
        buffer[idx++] = c;
        buffer[idx]   = '\0';
      }
    }
  }

  return false;
}

/** @brief 
 *  @param buffer 
 *  @param size 
 *  @param timeout 
 *  @return 
*/
uint16_t TEL0177_AT_Core::readAll(char* buffer, uint16_t size, uint32_t timeout){
  if(!this->_serial || !buffer) return 0;

  memset(buffer, 0, size);

  uint16_t idx   = 0;
  uint32_t start = millis();

  while(millis() - start < timeout){
    if(this->_serial->available()){
      char c = (char)this->_serial->read();

      if(idx < size - 1){
        buffer[idx++] = c;
        buffer[idx] = '\0';
      }
      start = millis();
    }
  }

  return idx;
}


// ================================================================================================
// UTILITAIRES
// ================================================================================================
/** @brief  Flushes the serial buffer by clearing all pending data and resetting the receive buffer.
 *  @return void
*/
void TEL0177_AT_Core::flush() {
  if(!this->_serial) return false;

  while(this->_serial->available()) this->_serial->read();
  
  memset(this->_rxBuffer, 0, sizeof(this->_rxBuffer));
}

/** @brief  
 *  @return bool
*/
bool TEL0177_AT_Core::isAvailable() {
  return this->_serial && (this->_serial->available() > 0);
}

/** @brief  
 *  @return bool
*/
bool TEL0177_AT_Core::isAlive() {
  return (send("AT") == TEL_OK);
}


// ================================================================================================
// FONCTIONS PRIVEES
// ================================================================================================

/** @brief 
 *  @param timeout 
 *  @return 
*/
TEL_Status TEL0177_AT_Core::_waitForOKorERROR(const uint32_t timeout) {
  uint16_t idx   = 0;
  uint32_t start = millis();

  memset(this->_rxBuffer, 0, sizeof(this->_rxBuffer));

  while(millis() - start < timeout){
    if(this->_serial->available()){
      char c = (char)this->_serial->read();
      if(idx < TEL_RX_BUFFER_SIZE - 1){
        this->_rxBuffer[idx++] = c;
        this->_rxBuffer[idx]   = '\0';
      }

      if(_contains(this->_rxBuffer, "\r\nOK\r\n") || _contains(this->_rxBuffer, "\nOK\n")){
        return TEL_OK;
      }
      
      if(_contains(this->_rxBuffer, "ERROR")){
        return TEL_ERROR;
      }
    }
  }

  return TEL_TIMEOUT;
}

/** @brief  Searches for the presence of a substring within a string.
 *  @param  haystack The string to search in (the main string being examined).
 *  @param  needle The substring to search for (the pattern to find within haystack).
 *  @return true if needle is found within haystack, false otherwise.
*/
bool TEL0177_AT_Core::_contains(const char* haystack, const char* needle) {
  return (strstr(haystack, needle) != nullptr);
}