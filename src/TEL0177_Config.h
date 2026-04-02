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
#define TEL_LOG_ERROR(fmt, ...)                 do { char _buf[256]; snprintf(_buf, sizeof(_buf), fmt, ##__VA_ARGS__); Serial.print(_buf); } while(0)
#define TEL_LOGLN_ERROR(fmt, ...)               do { char _buf[256]; snprintf(_buf, sizeof(_buf), fmt, ##__VA_ARGS__); Serial.println(_buf); } while(0)

// 3 - Debug
#define TEL_DEBUG(x)                            Serial.print(x)
#define TEL_DEBUGLN(x)                          Serial.println(x)


// ====================================================================
// TIMEOUTS
// ====================================================================
#define TEL_TIMEOUT_DEFAULT                     3000  
#define TEL_TIMEOUT_DEFAULT_AT                  2000  
#define TEL_TIMEOUT_NETWORK                     15000  
#define TEL_TIMEOUT_HTTP                        30000   
#define TEL_TIMEOUT_MQTT                        10000   
#define TEL_TIMEOUT_SMS                         10000  
#define TEL_TIMEOUT_BOOT                        5000   


#define TEL_TIMEOUT_AT_WAIT_RESPONSE            1000   


// ====================================================================
// RESPONSE COMMAND AT
// ====================================================================
#define TEL_RESPONSE_OK                         "OK"
#define TEL_RESPONSE_ERROR                      "ERROR"


// ====================================================================
// RETRY
// ====================================================================
#define TEL_MAX_RETRIES                          3


// ====================================================================
// BUFFERS
// ====================================================================
#define TEL_RX_BUFFER_SIZE                        1024    
#define TEL_LINE_BUFFER_SIZE                      256     
#define TEL_CMD_BUFFER_SIZE                       256    
#define TEL_HTTP_BODY_SIZE                        1024    
#define TEL_MQTT_PAYLOAD_SIZE                     512     


#define TEL_SIZE_LENGTH                           0
// ====================================================================
// 
// ====================================================================

// 1  - RETRY
#define TEL_RETRY_DELAY_MS                        500

// 2  - RESEAU
#define TEL_PDP_CONTEXT_ID                        1        

// 3  - MQTT
#define TEL_MQTT_DEFAULT_PORT                     1883
#define TEL_MQTT_KEEP_ALIVE                       60





#endif //TEL0177_CONFIG_H