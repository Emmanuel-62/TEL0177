#ifndef TEL0177_AT_CORE_H
#define TEL0177_AT_CORE_H

#include <Arduino.h>
#include <Stream.h>


#include "TEL0177_Config.h"
#include "TEL0177_Types.h"


class TEL0177_AT_Core {
  private:
    Stream*   _serial;
    char      _rxBuffer[TEL_RX_BUFFER_SIZE];

    // ==========================================================================
    // FONCTIONS PRIVEES
    // ==========================================================================
    /**
    * 
    */
    TEL_Status _waitForOKorERROR(const uint32_t);

    /**
    * 
    */
    bool _contains(const char*, const char*);

  public:
    // ==========================================================================
    // CONSTRUCTEUR
    // ==========================================================================
    /**
    * 
    */
    //TEL0177_AT_Core();  // Pas necessaire parce objet est global

    // ==========================================================================
    // INITIALISATION
    // ==========================================================================
    /**
    * 
    */
    void begin(Stream& serial);

    // ==========================================================================
    // ENVOIE
    // ==========================================================================
    /**
    * 
    */
    TEL_Status send(const char*, uint32_t = TEL_TIMEOUT_DEFAULT);

    /**
    * 
    */
    TEL_Status sendAndWait(const char*, const char*, uint32_t = TEL_TIMEOUT_DEFAULT);

    /**
    * 
    */
    TEL_Status sendAndRead(const char*, const char*, uint16_t , uint32_t = TEL_TIMEOUT_DEFAULT);

    /**
    * 
    */
    TEL_Status sendRaw(const char*, uint16_t = 0);

    // ==========================================================================
    // LECTURE
    // ==========================================================================
    /**
    * 
    */
    bool waitFor(const char*, uint32_t = TEL_TIMEOUT_DEFAULT);

    /**
    * 
    */
    bool readLine(char*, uint16_t, uint32_t = TEL_TIMEOUT_DEFAULT);

    /**
    * 
    */
    uint16_t readAll(char*, uint16_t, uint32_t = TEL_TIMEOUT_DEFAULT);

    // ==========================================================================
    // UTILITAIRES
    // ==========================================================================
    /**
    * 
    */
    void flush();

    /**
    * 
    */
    bool isAvailable();

    /**
    * 
    */
    bool isAlive();

};

#endif // TEL0177_AT_CORE_H