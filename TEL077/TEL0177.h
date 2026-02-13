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

  public:
    
    TEL0177(Stream& serial);

    //================================================================
    //===========================/ Init /=============================
    //================================================================     
    bool begin(uint32_t = TEL0177_DEFAULT_BAUD);
    bool end();
    bool isReady();
    bool checkSIMReady();

    //================================================================
    //===========================/ Infos /============================
    //================================================================    
    String getIMEI();
    String getModuleInfoString();
    void printDiagnostics();

    //================================================================
    //===========================/ SIM /==============================
    //================================================================
    bool isSimReady();
    bool unlockSim(const String&);

    //================================================================
    //===========================/ Réseau /===========================
    //================================================================
    bool connectNetwork(uint32_t = TEL0177_MQTT_CONNECT_TIMEOUT);
    bool isNetworkConnected();
    
    //================================================================
    //===========================/ MQTT /=============================
    //================================================================
    bool mqttBegin(const char*, uint16_t, const char*, const char* = "", const char* = "");
    bool mqttConnect(bool = true, uint16_t = TEL0177_MQTT_KEEPALIVE);
    bool mqttDisconnect();
    bool mqttIsConnected();
    bool mqttPublish(const char*, const char*);
    bool mqttSubscribe(const char*, uint8_t = TEL0177_MQTT_QOS);
    
    //================================================================
    //=======================/ Utilitaires /==========================
    //================================================================
    bool reset();
    String sendATCommand(const char*);
};

#endif // TEL0177_H