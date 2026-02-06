#ifndef TEL0177_AT_H
#define TEL0177_AT_H

#include <Arduino.h>
#include <Stream.h>

#include "TEL0177_Config.h"

class TEL0177_AT {
  private:
    Stream& _serial;

    String sendCommand(const char*, const char* = "OK", uint32_t = TEL0177_AT_TIMEOUT);
    String sendCommand(const String&, const char* = "OK", uint32_t = TEL0177_AT_TIMEOUT);


    String parseFirstLine(const String&);


    bool waitResponse(const char*, uint32_t = TEL0177_AT_TIMEOUT);
    String readResponse(uint32_t = TEL0177_AT_TIMEOUT);

  public:
    TEL0177_AT(Stream&);

    // ============================================
    // COMMANDES AT DE BASE
    // ============================================
    bool reset();
    bool factoryReset();

    void clearBuffer();

    // ============================================
    // TEST COMMUNICATION
    // ============================================
    bool test();
    bool atTest();

    // ============================================
    // INFORMATION MODULE
    // ============================================
    String getManufacturer();
    String getModuleModel();
    String getModuleVersion();
};

#endif //TEL0177_AT_H