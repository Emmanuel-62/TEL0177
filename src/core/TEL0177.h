#ifndef TEL0177_H
#define TEL0177_H

#include <Arduino.h>
#include <Stream.h>
#include "TEL0177_AT.h"

class TEL0177 {
  private:
    TEL0177_AT _at;
    bool _initialized;
    bool _networkConnected;
    bool _mqttConnected;

    bool checkSimReady();

  public:
    TEL0177(Stream& serial);

    // ============================================
    // INITIALISATION
    // ============================================
    bool begin(uint32_t = TEL0177_DEFAULT_BAUD);
    bool end();
    bool isReady();

    // ============================================
    // INFORMATIONS MODULE
    // ============================================
    String getIMEI();
    String getModuleInfoString();
    void printDiagnostics();

    // ============================================
    // GESTION SIM
    // ============================================
    bool isSimReady();
    bool unlockSim(const String&);

    // ============================================
    // RÉSEAU
    // ============================================
    bool connectNetwork(uint32_t = TEL0177_MQTT_CONNECT_TIMEOUT);
    bool isNetworkConnected();

    // ============================================
    // MQTT
    // ============================================
    bool mqttSetup(const char*, uint16_t, const char*, const char* = "", const char* = "");
    bool mqttConnect(bool = true, uint16_t = TEL0177_MQTT_KEEPALIVE);
    bool mqttDisconnect();
    bool isMqttConnected();
    bool mqttPublish(const char*, const char*);
    bool mqttSubscribe(const char*, uint8_t = TEL0177_MQTT_QOS);

    // ============================================
    // UTILITAIRES
    // ============================================
    bool reset();
    String sendATCommand(const char*);
};

#endif // TEL0177_H