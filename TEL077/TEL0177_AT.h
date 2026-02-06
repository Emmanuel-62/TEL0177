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
    // FONCTION DE BASE 
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
    String getModuleModel();
    String getModuleVersion();
};

#endif //TEL0177_AT_H