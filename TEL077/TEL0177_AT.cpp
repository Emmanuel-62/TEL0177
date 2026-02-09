#include "TEL0177_AT.h"

TEL0177_AT::TEL0177_AT(Stream & serial): _serial(serial){}

// ============================================
// COMMANDES AT DE BASE
// ============================================
String TEL0177_AT::sendCommand(const char* cmd, const char* expected, uint32_t timeout){
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

      if(response.indexOf(expected) >= 0){
        TEL0177_LOGLN("<<< " + response);
        return response;
      }
      if(response.indexOf("ERROR") >= 0){
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
    int end = response.indexOf('\n');

    while(end >= 0){
      String line = response.substring(start, end);
      line.trim();

      if(line.length() > 0 && !line.startsWith("AT")){
        if(line.indexOf("OK")){
          return line;
        }
      }
      
      start = end + 1;
      end = response.indexOf('\n', start);
    }
    return "";
}

void TEL0177_AT::clearBuffer(){
  while(this->_serial.available()){
    _serial.read();
  }
}


// ============================================
// COMMANDES AT DE BASE
// ============================================
bool TEL0177_AT::test(){
  String response = sendCommand("AT");
  return response.indexOf("OK");
}

bool TEL0177_AT::reset(){
  String response = sendCommand("AT+RESET");
  return response.indexOf("OK");
}

bool TEL0177_AT::factoryReset(){
  String response = sendCommand("AT+RSTSET");
  return response.indexOf("OK");
}


// ============================================
// INFORMATION MODULE
// ============================================
String TEL0177_AT::getManufacturer(){
  String response = sendCommand("AT+CGMI");
  return parseFirstLine(response);
}

String TEL0177_AT::getModel(){
  String response = sendCommand("AT+CGMM");
  return parseFirstLine(response);
}

String TEL0177_AT::getFirmwareVersion(){
  String response = sendCommand("AT+CGMR");
  return parseFirstLine(response);
}

String TEL0177_AT::getIMEI(){
  String response = sendCommand("AT+CGSN");
  return parseFirstLine(response);
}

String TEL0177_AT::getICCID(){
  String response = sendCommand("AT+CCID");
  return parseFirstLine(response);
}

String TEL0177_AT::getIMSI(){
  String response = sendCommand("AT+CIMI");
  return parseFirstLine(response);
}

//------------------------------------------------------
String TEL0177_AT::getProductInfo(){
  String response = sendCommand("ATI");
  return parseFirstLine(response);
}

String TEL0177_AT::getSwVersion(){
  String response = sendCommand("AT+VER");
  return parseFirstLine(response);
}

String TEL0177_AT::getHwVersion(){
  String response = sendCommand("AT^HVER");
  return response;
}

String TEL0177_AT::getAllInfo(){
  String response = sendCommand("AT*I");
  return response;
}
String TEL0177_AT::getBootCause(){
  String response = sendCommand("AT*EXINFO?");
  return response;
}
String TEL0177_AT::getUartBaud(){
  String response = sendCommand("AT+IPR?");
  return response;
}
String TEL0177_AT::getNetworkRegStatus(){
  String response = sendCommand("AT+CREG?");
  return response;
}
String TEL0177_AT::getSignalQuality(){
  String response = sendCommand("AT+CSQ");
  return response;
}
String TEL0177_AT::getSimStatus(){
  String response = sendCommand("AT+CPIN?");
  return response;
}
String TEL0177_AT::getDataAttachStatus(){
  String response = sendCommand("AT+CGATT?");
  return response;
}

// ============================================
// FONCTIONS MQTT
// ============================================
String TEL0177_AT::mqttCheckSimReady(const String& pinCode){
  String response = sendCommand("AT+CPIN = " + pinCode);
  return response;
}

String TEL0177_AT::mqttCheckAttached(){
  String response = sendCommand("AT+CGATT?");
  return response;
}

String TEL0177_AT::mqttConfig(const String& clientId, const String& user, const String& passWord){
  String response = sendCommand("AT+MCONFIG = " + clientId + "," + user + "," + passWord);
  return response;
}

String TEL0177_AT::mqttTcpConnect(const String& serverMQTT, const String& port){
  String response = sendCommand("AT+MIPSTART = " + serverMQTT + "," + port);
  return response;
}

String TEL0177_AT::mqttConnect(const String& cleanSession, const String& keepalive){
  String response = sendCommand("AT+MCONNECT = " + cleanSession + "," + keepalive);
  return response;
}

String TEL0177_AT::mqttSubscribe(const String& topic, const int qos){
  String response = sendCommand("AT+MSUB = " + topic + "," + qos);
  return response;
}

void TEL0177_AT::mqttPublish(const String& topic, const int qos, const int retain, const String& message){
  String response = sendCommand("AT+MPUB = " + topic + "," + qos + "," + retain + "," + message);
}