#include "TEL0177_AT.h"

TEL0177_AT::TEL0177_AT(Stream & serial): _serial(serial){}


// ============================================
// COMMANDES AT DE BASE
// ============================================
String TEL0177_AT::sendCommand(const char* cmd, const char* expected, uint32_t timeout){
  //TEL0177_LOGLN("Commande send ===> " + String(cmd));           

  clearBuffer();

  this->_serial.print(cmd);
  this->_serial.print("\r\n");
  this->_serial.flush();

  String response = "";
  unsigned long start = millis();
  unsigned long lastDataTime = millis();
  bool expectedFound = false;

  while(millis() - start < timeout){
    while(this->_serial.available()){
      char c = this->_serial.read();
      response += c;
      lastDataTime = millis();

      if (expected && response.indexOf(expected) >= 0){
        expectedFound = true;
      }
    }
    
    if(expectedFound && (millis() - lastDataTime > 1000)){
      return response;
    }
  }
  
  return response; 
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
        if(line != "OK"){
          return line;
        }
      }
      
      start = end + 1;
      end = response.indexOf('\n', start);
    }
    return "";
}

String TEL0177_AT::escapeQuotes(const char* str) {
    String result = String(str);
    result.replace("\"", "\\\"");
    return result;
}

void TEL0177_AT::clearBuffer(){
  while(this->_serial.available()){
    this->_serial.read();
  }
}


// ============================================
// COMMANDES AT DE BASE
// ============================================
bool TEL0177_AT::getAt(){
  String response = sendCommand("AT");
  return response.indexOf("OK") >= 0;
}

bool TEL0177_AT::reset(){
  String response = sendCommand("AT+RESET");
  return response.indexOf("OK") >= 0;
}

bool TEL0177_AT::factoryReset(){
  String response = sendCommand("AT+RSTSET");
  return response.indexOf("OK") >= 0;
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

   
// ============================================
// FONCTIONS DEMANDE CODE PIN
// ============================================
String TEL0177_AT::getIMSI(){
  String response = sendCommand("AT+CIMI");
  return parseFirstLine(response);
}

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
  return parseFirstLine(response);
}

String TEL0177_AT::getAllInfo(){
  String response = sendCommand("AT*I");
  return parseFirstLine(response);
}
String TEL0177_AT::getBootCause(){
  String response = sendCommand("AT*EXINFO?");
  return parseFirstLine(response);
}
String TEL0177_AT::getUartBaud(){
  String response = sendCommand("AT+IPR?");
  return parseFirstLine(response);
}
String TEL0177_AT::getNetworkRegStatus(){
  String response = sendCommand("AT+CREG?");
  return parseFirstLine(response);
}
String TEL0177_AT::getSignalQuality(){
  String response = sendCommand("AT+CSQ");
  return parseFirstLine(response);
}

String TEL0177_AT::getDataAttachStatus(){
  String response = sendCommand("AT+CGATT?");
  return parseFirstLine(response);
}


// ============================================
// FONCTIONS DE VERIFICATIONS
// ============================================
bool TEL0177_AT::setSimPin(const String& pinCode){
  String response = sendCommand("AT+CPIN=\"" + pinCode +"\"");
  return response.indexOf("OK") >= 0;
}

bool TEL0177_AT::setSimStatus(){
  String response = sendCommand("AT+CPIN?");
  return response.indexOf("OK") >= 0;
}

bool TEL0177_AT::tcpClose(){
  String response = sendCommand("AT+MIPCLOSE");
  return response.indexOf("OK") >= 0;
}  

// ============================================
// FONCTIONS MQTT
// ============================================
bool TEL0177_AT::mqttConfig(const char* clientId, const char* user, const char* passWord){
  String cmd = "AT+MCONFIG=\"" + String(clientId) + "\",\"" + String(user) + "\",\"" + String(passWord) +"\"";
  String response = sendCommand(cmd);
  return response.indexOf("OK") >= 0;
}


bool TEL0177_AT::mqttTcpConnect(const char* serverMQTT, const uint16_t port){
  if(serverMQTT == nullptr || strlen(serverMQTT) == 0){
    TEL0177_LOGLN("ERROR: Empty server address");
    return false;
  }

  if(port == 0 || port > 65535){
    TEL0177_LOGLN("ERROR: Invalid port");
    return false;
  }

  String cmd = "AT+MIPSTART=\"" + String(serverMQTT) + "\",\"" + port +"\"";
  String response = sendCommand(cmd, "OK", TEL0177_MQTT_CONNECT_TIMEOUT);
  return response.indexOf("OK") >= 0;
}

bool TEL0177_AT::mqttConnect(bool cleanSession, uint16_t keepAlive) {
    String cmd = "AT+MCONNECT=" + String(cleanSession ? 1 : 0) + "," + String(keepAlive);
    
    String response = sendCommand(cmd, "OK", TEL0177_MQTT_CONNECT_TIMEOUT);
    
    return (response.indexOf("CONNACK") >= 0 || response.indexOf("OK") >= 0);
}

bool TEL0177_AT::mqttSubscribe(const char* topic, const uint8_t qos){
  String response = sendCommand("AT+MSUB=\"" + String(topic) + "\",\"" + qos +"\"");
  return response.indexOf("+MSUB") >= 0;
}

bool TEL0177_AT::mqttPublish(const char* topic, const char* message, uint8_t qos, bool retain){
    if(!message || strlen(message) == 0) return false;
    
    String msgStr = String(message);
    msgStr.replace("\"", "'"); 
    
    String cmd = "AT+MPUB=\"" + String(topic) + "\",\"" + String(qos) + "\",\"" + String(retain ? 1 : 0) + "\",\"" + msgStr + "\"";
    String response = sendCommand(cmd.c_str(), "OK", TEL0177_MQTT_PUBLISH_TIMEOUT);
    
    return (response.indexOf("OK") >= 0);
}

bool TEL0177_AT::mqttDisconnect(){
  String response = sendCommand("AT+MDISCONNECT");
  return response.indexOf("OK") >= 0;
}