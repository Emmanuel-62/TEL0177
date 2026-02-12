#ifndef TEL0177_AT_H
#define TEL0177_AT_H

#include <Arduino.h>
#include <Stream.h>

#include "TEL0177_Config.h"

class TEL0177_AT {
  private:
    Stream& _serial;

    String parseFirstLine(const String&);
    String escapeQuotes(const char*);

  public:
    TEL0177_AT(Stream&);

    // ============================================
    // FONCTIONS DE BASE 
    // ============================================
    String sendCommand(const char*, const char* = "OK", uint32_t = TEL0177_AT_TIMEOUT);
    String sendCommand(const String&, const char* = "OK", uint32_t = TEL0177_AT_TIMEOUT);

    void clearBuffer();

    // ============================================
    // COMMANDES AT DE BASE
    // ============================================
    bool getAt();
    bool reset();
    bool factoryReset();

    // ============================================
    // INFORMATION MODULE
    // ============================================
    String getManufacturer();
    String getModel();
    String getFirmwareVersion();
    String getIMEI();
    String getICCID();

    // ============================================
    // FONCTIONS DEMANDE CODE PIN
    // ============================================
    String getIMSI();
    String getProductInfo();
    String getSwVersion();
    String getHwVersion();
    String getAllInfo();
    String getBootCause();
    String getUartBaud();
    String getNetworkRegStatus();
    String getSignalQuality();
    String getDataAttachStatus();

    // ============================================
    // FONCTIONS DE VERIFICATIONS
    // ============================================
    bool setSimPin(const String&);
    bool setSimStatus();
    bool tcpClose();
    bool networkDetach();
    bool powerOff();

    // ============================================
    // FONCTIONS MQTT
    // ============================================
    bool commandMqttConfig(const char*, const char*, const char*);
    bool commandMqttTcpConnect(const char*, const uint16_t);
    bool commandMqttConnect(bool cleanSession, uint16_t keepalive);
    bool commandMqttSubscribe(const char*, const uint8_t);
    bool commandMqttPublish(const char*, const char*, uint8_t = 0, bool = false);
    bool commandMqttDisconnect(); 
};

#endif //TEL0177_AT_H