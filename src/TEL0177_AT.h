#ifndef TEL0177_AT_H
#define TEL0177_AT_H

#include <Arduino.h>
#include <Stream.h>
#include "TEL0177_Config.h"

class TEL0177_AT {
  private:
    Stream& _serial;
    
    String parseFirstLine(const String &);
    String escapeQuotes(const char *);

  public:
    TEL0177_AT(Stream &);

    // ============================================
    // FONCTIONS DE BASE
    // ============================================
    String sendCommand(const char *, const char * = "OK", uint32_t = TEL0177_AT_TIMEOUT);
    String sendCommand(const String &, const char * = "OK", uint32_t = TEL0177_AT_TIMEOUT);
    void clearBuffer();

    // ============================================
    // COMMANDES AT DE BASE
    // ============================================
    bool testAt();
    bool reset();
    bool factoryReset();

    // ============================================
    // INFORMATIONS MODULE
    // ============================================
    String getManufacturer();
    String getModel();
    String getFirmwareVersion();
    String getSwVersion();
    String getHwVersion();
    String getIMEI();
    String getICCID();
    String getIMSI();
    String getProductInfo();
    String getAllInfo();
    String getBootCause();
    String getUartBaud();

    // ============================================
    // GESTION SIM
    // ============================================
    String getSimStatus();
    bool checkSimReady();
    bool setSimPin(const String&);

    // ============================================
    // RÉSEAU
    // ============================================
    String getNetworkRegStatus();
    String getSignalQuality();
    String getDataAttachStatus();
    bool detachNetwork();

    // ============================================
    // CONNEXION TCP
    // ============================================
    bool tcpDisconnect();

    // ============================================
    // FONCTIONS MQTT
    // ============================================
    bool mqttConfig(const char*, const char*, const char*);
    bool mqttTcpConnect(const char*, const uint16_t);
    bool mqttConnect(bool cleanSession, uint16_t keepalive);
    bool mqttSubscribe(const char*, const uint8_t);
    bool mqttPublish(const char*, const char*, uint8_t = 0, bool = false);
    bool mqttUnsubscribe(const char*);
    bool mqttDisconnect();
    bool mqttStatus();

    // ============================================
    // ALIMENTATION
    // ============================================
    bool powerOff();
};

#endif // TEL0177_AT_H