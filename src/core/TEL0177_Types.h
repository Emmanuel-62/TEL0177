#ifndef TEL0177_TYPES_H
#define TEL0177_TYPES_H

#include <Arduino.h>

enum class TEL_Status {
  OK = 0,
  ERROR,
  TIMOUT,
  INVALID_RESPONSE,
  NETWORK_ERROR
};

#endif // TEL0177_TYPES_H