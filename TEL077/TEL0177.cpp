#include "TEL0177.h"


TEL0177::TEL0177(Stream & serial):_at(serial){}

bool TEL0177::begin(){
  return this->_at.getAt();
}