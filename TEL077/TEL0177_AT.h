#ifndef TEL0177_AT_H
#define TEL0177_AT_H

#include <Arduino.h>
#include <Stream.h>

#include "TEL0177_Config.h"

class TEL0177_AT {
  private:
    Stream& _serial;

  public:
    TEL0177_AT(Stream&);

    // ============================================
    // FONCTIONS DE BASE 
    // ============================================
    String sendCommand(const char*, const char* = "OK", uint32_t = TEL0177_AT_TIMEOUT);
    String sendCommand(const String&, const char* = "OK", uint32_t = TEL0177_AT_TIMEOUT);

    String parseFirstLine(const String&);

    void clearBuffer();

    // ============================================
    // COMMANDES AT DE BASE
    // ============================================
    bool test();
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
    String getIMSI();

    String getProductInfo();
    String getSwVersion();
    String getHwVersion();
    String getAllInfo();
    String getBootCause();
    String getUartBaud();
    String getNetworkRegStatus();
    String getSignalQuality();
    String getSimStatus();
    String getDataAttachStatus();

    // ============================================
    // FONCTIONS MQTT
    // ============================================
    String mqttCheckSimReady(const String&);
    String mqttCheckAttached();
    String mqttConfig(const String&, const String&, const String&);
    String mqttTcpConnect(const String&, const String&);
    String mqttConnect(const String&, const String&);
    String mqttSubscribe(const String&, const int);
    void mqttPublish(const String&, const int, const int, const String&);
};

#endif //TEL0177_AT_H