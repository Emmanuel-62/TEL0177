#ifndef TEL0177_CONFIG_H
#define TEL0177_CONFIG_H


// ====================================================================
// SETTINGS SERIAL MONITOR DEFAULT BAUD 
// ====================================================================
#define TEL_DEFAULT_BAUD                        115200


// ====================================================================
// DISPLAY ON SERIAL MONITOR
// ====================================================================
// 1 - Standard display
#define TEL_LOG(x)                              Serial.print(x)
#define TEL_LOGLN(x)                            Serial.println(x)

// 2 - Error display
#define TEL_LOG_ERROR(x)                        Serial.print(x)
#define TEL_LOGLN_ERROR(x)                      Serial.println(x)


// ====================================================================
// TIMEOUTS
// ====================================================================
#define TEL_DEFAULT_TIMEOUT                     5000
#define TEL_DEFAULT_AT_TIMEOUT                  2000
#define TEL_AT_WAIT_RESPONSE                    1000           


// ====================================================================
// RESPONSE COMMAND AT
// ====================================================================
#define TEL_RESPONSE_OK                         "OK"
#define TEL_RESPONSE_ERROR                      "ERROR"


// ====================================================================
// RETRY
// ====================================================================
#define TEL_MAX_RETRIES                          3


