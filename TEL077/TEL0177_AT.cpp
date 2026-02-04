#include "TEL0177_AT.h"
#include "TEL0177_Config.h"

TEL0177_AT::TEL0177_AT(Stream & serial): _serial(serial){}

bool TEL0177_AT::sendCommandAT(String & cmd, uint32_t timeout = TEL0177_AT_TIMEOUT){
  
  return true;
}