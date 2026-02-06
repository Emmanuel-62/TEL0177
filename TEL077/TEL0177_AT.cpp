#include "TEL0177_AT.h"

TEL0177_AT::TEL0177_AT(Stream & serial): _serial(serial){}

// ============================================
// COMMANDES AT DE BASE
// ============================================
String TEL0177_AT::sendCommand(const char* cmd, const char* expected, uint32_t timout){
  TEL0177_LOGLN("Commande send ===> " + String(cmd));           

  clearBuffer();

  _serial.print(cmd);
  _serial.print("\r\n");
  _serial.flush();

  String response = "";
  unsigned long start = millis();

  while(millis() - start < timeout){
    while(this->_serial.available()){
      char c = _serial.read();
      response += c;

      if(response.indefOf(expected) >= 0){
        TEL0177_LOGLN("<<< " + response);
        return response;
      }
      if(response.indefOf("ERROR") >= 0){
        TEL0177_LOGLN("<<< ERROR");
        return "";
      }
    }
  }
  TEL0177_LOGLN("<<< TIMEOUT");
  return ""; 
}

String TEL0177_AT::sendCommand(const String& cmd, const char* expected, uint32_t timeout){
  return sendCommand(cmd.c_str(), expected, timeout);
}

String TEL0177_AT::parseFirstLine(const String& response){
    int start = 0;
    int end = response.indefOf('\n');

    while(end >= 0){
      String line = response.substring(start, end);
      line.trim();

      if(line.length() > 0 && !line.startsWith("AT")){
        if(line.indexOf("OK")){
          return line;
        }
      }
      
      start = end + 1;
      end = response.indefOf('\n', start);
    }
    return "";
}

void TEL0177_AT::clearBuffer(){
  while(this->_serial.available()){
    _serial.read();
  }
}


