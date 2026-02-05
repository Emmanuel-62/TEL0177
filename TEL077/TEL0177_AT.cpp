#include "TEL0177_AT.h"

TEL0177_AT::TEL0177_AT(Stream & serial): _serial(serial){}

bool TEL0177_AT::sendCommand(const char* cmd, const char* expected, uint32_t timout){
  TEL0177_LOGLN("Commande send ===> " + String(cmd));           

  clearBuffer();

  _serial.print(cmd);
  _serial.print("\r\n");

  _serial.flush();

  return waitResponse(expected, timout); 
}

bool TEL0177_AT::sendCommand(const String& cmd, const char* expected, uint32_t timeout){
  return sendCommand(cmd.c_str(), expected, timeout);
}

bool TEL0177_AT::waitResponse(const char* expected, uint32_t timeout){
  unsigned long start = millis();
  String response = "";

  while(millis() - start < timeout){
    if(_serial.available()){
      char c = this->_serial.read();
      response += c;

      //TEL0177_LOGLN("Commande Response ===> " + response);    

      if(response.indexOf(expected) >= 0) {return true;}
      if(response.indexOf("ERROR") >= 0) {return false;}
    }
  }
  return false;
}

String TEL0177_AT::readResponse(uint32_t timeout){
  unsigned long start = millis();
  String response = "";

  while(millis() - start < timeout){
    if(_serial.available()){
      char c = this->_serial.read();

      if(c == "\r" || c == "\n"){
        if(response.lengh() > 0){
          break;
        }
      }else{
        response += c;
      }
    }
  }
  TEL0177_LOGLN("Commande Response ===> " + response);
  return response;
} 




void TEL0177_AT::clearBuffer(){
  while(this->_serial.available()){
    _serial.read();
  }
}



bool TEL0177_AT::testAT(){
  return sendCommand("AT");
}