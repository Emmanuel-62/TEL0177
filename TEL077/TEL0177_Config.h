#ifndef TEL0177_CONFIG_H
#define TEL0177_CONFIG_H


// ============================================
// TIMEOUTS (en millisecondes)
// ============================================
#define TEL0177_DEFAULT_TIMEOUT         5000
#define TEL0177_AT_TIMEOUT              2000
#define TEL0177_MQTT_TIMEOUT            2000


// ============================================
// PARAMÈTRES RÉSEAU
// ============================================
#define TEL0177_DEFAULT_BAUD            115200


#define TEL0177_LOG(x)      Serial.print(x)
#define TEL0177_LOGLN(x)    Serial.println(x)


#define TEL0177_MQTT_CONNECT_TIMEOUT    10000  
#define TEL0177_MQTT_PUBLISH_TIMEOUT     5000  
#define TEL0177_MQTT_SUBSCRIBE_TIMEOUT   5000  
#define TEL0177_MQTT_RETRY_TIMEOUT       5000
#define TEL0177_MQTT_KEEPALIVE             60
#define TEL0177_MQTT_QOS                    0
#define TEL0177_MQTT_RETAIN             false


#endif //TEL0177_CONFIG_H