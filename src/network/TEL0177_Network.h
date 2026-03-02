#ifndef TEL0177_NETWORK_H
#define TEL0177_NETWORK_H

#include <Arduino.h>

#include "TEL0177_AT_Core.h"
#include "TEL0177_Types.h"
#include "TEL0177_Config.h"

class TEL0177_Network {
  private:
    TEL0177_AT_Core* _at;
    TEL_NetState     _state;

    char             _ip[16];
    char             _apn[32];

    /**
    * 
    */
    TEL_Status _configureAPN(const char* );

    /**
    * 
    */
    TEL_Status _waitForNetworkReg(uint32_t );

  public:

    // ==========================================================================
    // 
    // ==========================================================================
    /**
    * 
    */
    void       begin(TEL0177_AT_Core& );

    // ==========================================================================
    // 
    // ==========================================================================
    /**
    * 
    */
    TEL_Status unlockSIM(const char*);

    /**
    * 
    */
    TEL_Status attach(uint32_t = TEL_TIMEOUT_NETWORK);

    /**
    * 
    */
    TEL_Status detach();

    /**
    * 
    */
    bool       isAttached();
};

#endif // TEL0177_NETWORK_H
