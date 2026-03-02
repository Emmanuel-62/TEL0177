#ifndef TEL0177_H
#define TEL0177_H

#include <Arduino.h>
#include <Stream.h>
#include "TEL0177_AT.h"

class TEL0177 {
  private:
    TEL0177_AT_Core _at;

    /**
    * 
    */
    TEL_Status      _initModem();


  public:

    TEL0177_Network  network;
    TEL0177_HTTP     http;
    TEL0177_SMS      sms;
    TEL0177_MQTT     mqtt;

    /**
    * 
    */
    TEL_Status  begin(Stream& serial);

    /**
    * 
    */
    void        loop();

    /**
    * 
    */
    TEL_Status  reset();

    /**
    * 
    */
    bool        isAlive();
};

#endif // TEL0177_H