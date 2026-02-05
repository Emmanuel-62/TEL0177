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

    bool sendCommand(const char*, const char* = "OK", uint32_t = TEL0177_AT_TIMEOUT);
    bool sendCommand(const String&, const char* = "OK", uint32_t = TEL0177_AT_TIMEOUT);
    bool waitResponse(const char*, uint32_t = TEL0177_AT_TIMEOUT);
    String readResponse(uint32_t = TEL0177_AT_TIMEOUT);



    void clearBuffer();

    bool testAT();
};

#endif //TEL0177_AT_H