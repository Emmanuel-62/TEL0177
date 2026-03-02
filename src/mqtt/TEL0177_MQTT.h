#ifndef TEL0177_MQTT_H
#define TEL0177_MQTT_H


#include <Arduino.h>


#include "../at/TEL0177_AT_Core.h"
#include "../core/TEL0177_Types.h"
#include "../core/TEL0177_Config.h"


// ==========================================================================
// 
// ==========================================================================
typedef void (*TEL_MQTT_Callback)(const char* urc);

typedef void (*TEL_MQTT_Callback)(const char* topic, const char* payload, uint16_t length);

//typedef void (*TEL_MQTT_RawCallback)(const char* urc);


class TEL0177_MQTT {
  private:
    TEL0177_AT_Core*  _at;
    TEL_MQTT_Callback _callback;
    bool              _connected;

    char              _host[64];
    char              _clientId[64];
    char              _user[32];
    char              _pass[64];

    uint16_t          _port;
    uint16_t          _keepAlive;
    
    // ==========================================================================
    // FONCTIONS PRIVEES
    // ==========================================================================
    /**
    * 
    */
    TEL_Status _configBroker(const char*, uint16_t );

    /**
    * 
    */
    TEL_Status _configCredentials(const char*, const char*, const char*);

    /**
    * 
    */
    void       _handleRecv(const char*);

  public:
    // ==========================================================================
    // 
    // ==========================================================================
    /**
    * 
    */
    void       begin(TEL0177_AT_Core& );

    /**
    * 
    */
    void       setCallback(TEL_MQTT_Callback);
};

#endif // TEL0177_MQTT_H