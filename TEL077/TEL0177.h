#ifndef TEL0177_H
#define TEL0177_H

#include <Stream.h>

#include "TEL0177_AT.h"


class TEL0177 {
  private:
    Stream & _serial;
    TEL0177_AT _at;

  public:
    TEL0177(Stream &);
};

#endif //TEL0177_H