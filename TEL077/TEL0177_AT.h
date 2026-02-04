#ifndef TEL0177_AT_H
#define TEL0177_AT_H

#include <Arduino.h>
#include <Stream.h>

class TEL0177_AT {
  private:
    Stream & _serial;

  public:
    TEL0177_AT(Stream &);

    bool sendCommandAT(String &, uint32_t);
};

#endif //TEL0177_AT_H