#ifndef TEL0177_H
#define TEL0177_H

#include <Arduino.h>
#include <Stream.h>

#include "TEL0177_Types.h"
#include "TEL0177_Config.h"
#include "TEL0177_AT_Core.h"
#include "TEL0177_Network.h"
#include "TEL0177_MQTT.h"
#include "TEL0177_System.h"


class TEL0177 {
  private:
    TEL0177_AT_Core _at;

    TEL0177_Network  _network;
    //TEL0177_HTTP     _http;
    //TEL0177_SMS      _sms;
    TEL0177_MQTT     _mqtt;

    /**
    * 
    */
    //TEL_Status      _initModem();

    /**
    * 
    */
    void _display(const char*, TEL_Status);

  public:

    // ==========================================================================
    // INITIALISATION
    // ==========================================================================
    /**
    * 
    */
    bool begin(Stream& serial);


    // ==========================================================================
    // 
    // ==========================================================================
    /**
    * 
    */
    void loop();

    /**
    * 
    */
    bool reset();

    /**
    * 
    */
    bool isAlive();

    /**
    *
    */
    bool simUnlock(const char *);

    // ==========================================================================
    // MQTT
    // ==========================================================================.
    /**
    *
    */
    bool mqttconnect(const char*, uint16_t, uint32_t = TEL_TIMEOUT_MQTT);

    bool mqttsetServer(const char*, const char* = nullptr, const char* = nullptr, uint16_t = TEL_MQTT_KEEP_ALIVE);

    bool mqttreconnect();

    bool mqttdisconnect();

    bool mqttisConnected();

    bool mqttpublish(const char*,const char*,uint8_t = 0,bool = false,uint32_t = TEL_TIMEOUT_MQTT);

    bool mqttsubscribe(const char* , uint8_t = 0);

    bool mqttunsubscribe(const char*);

    bool mqttsetCallback(TEL_MQTT_Callback);
};

#endif // TEL0177_H