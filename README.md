# Librairie TEL0177

## Structure de la librairie TEl0177
```text
    TEL0177/
    ├── src/
    │   ├── TEL0177_Config.h      
    │   ├── TEL0177_Types.h       
    │   ├── TEL0177_AT.h          
    │   ├── TEL0177_AT.cpp        
    │   ├── TEL0177.h            
    │   └── TEL0177.cpp           
    │
    ├── examples/
    │   └── Basic/
    │       └── ModuleInfo.ino
    │
    ├── keywords.txt              
    ├── library.properties        
    └── README.md                 
```

## Référence complète : Commandes AT → Fonctions TEL0177_AT 

### Fonctions de base (3 fonctions)

| # | Fonction       | Type retour | Commande AT | Description                  |
| - | -------------- | ----------- | ----------- | ---------------------------- |
| 1 | getAt()        | bool        | AT          | Test de communication module |
| 2 | reset()        | bool        | AT+RESET    | Redémarrage soft module      |
| 3 | factoryReset() | bool        | AT+RSTSET   | Reset paramètres usine       |

### Informations module / SIM / réseau (15 fonctions)

| #  | Fonction              | Type retour | Commande AT | Exemple retour         |
| -- | --------------------- | ----------- | ----------- | ---------------------- |
| 4  | getManufacturer()     | String      | AT+CGMI     | "AirM2M"               |
| 5  | getModel()            | String      | AT+CGMM     | "Air780EU"             |
| 6  | getFirmwareVersion()  | String      | AT+CGMR     | "AirM2M_780EU_V1171"   |
| 7  | getIMEI()             | String      | AT+CGSN     | "869080071397105"      |
| 8  | getICCID()            | String      | AT+CCID     | "8933200818544034397F" |
| 9  | getIMSI()             | String      | AT+CIMI     | "208200854403439"      |
| 10 | getProductInfo()      | String      | ATI         | Infos produit          |
| 11 | getSwVersion()        | String      | AT+VER      | Version software       |
| 12 | getHwVersion()        | String      | AT^HVER     | Version hardware       |
| 13 | getAllInfo()          | String      | AT*I        | Toutes infos combinées |
| 14 | getBootCause()        | String      | AT*EXINFO?  | Cause dernier boot     |
| 15 | getUartBaud()         | String      | AT+IPR?     | Baud rate actuel       |
| 16 | getNetworkRegStatus() | String      | AT+CREG?    | Statut réseau          |
| 17 | getSignalQuality()    | String      | AT+CSQ      | "25,99" (RSSI,BER)     |
| 18 | getSimStatus()        | String      | AT+CPIN?    | "+CPIN: READY"         |

### Gestion SIM & Power (5 fonctions)

| #  | Fonction                 | Type retour | Commande AT    | Description          |
| -- | ------------------------ | ----------- | -------------- | -------------------- |
| 19 | setSimPin(const String&) | bool        | AT+CPIN="xxxx" | Entrer code PIN      |
| 20 | setSimStatus()           | bool        | AT+CPIN?       | Vérifier SIM (bool)  |
| 21 | tcpClose()               | bool        | AT+MIPCLOSE    | Fermer TCP MQTT      |
| 22 | networkDetach()          | bool        | AT+CGATT=0     | Détacher réseau data |
| 23 | powerOff()               | bool        | AT+CPOWD=1     | Éteindre module      |

### MQTT (7 fonctions)

| #  | Fonction                                          | Type retour | Commande AT                      | Paramètres                   |
| -- | ------------------------------------------------- | ----------- | -------------------------------- | ---------------------------- |
| 24 | mqttConfig(const char*,const char*,const char*)   | bool        | AT+MCONFIG="id","user","pass"    | ClientID, user, mot de passe |
| 25 | mqttTcpConnect(const char*,uint16_t)              | bool        | AT+MIPSTART="server","port"      | Broker, port                 |
| 26 | mqttConnect(bool,uint16_t)                        | bool        | AT+MCONNECT=clean,keepalive      | Session propre, keepalive    |
| 27 | mqttSubscribe(const char*,uint8_t)                | bool        | AT+MSUB="topic",qos              | Topic, QoS                   |
| 28 | mqttPublish(const char*,const char*,uint8_t,bool) | bool        | AT+MPUB="topic",qos,retain,"msg" | Topic, message, QoS, retain  |
| 29 | mqttDisconnect()                                  | bool        | AT+MDISCONNECT                   | Déconnexion MQTT             |
| 29 | mqttUnsubscribe()                                 | bool        | AT+MUNSUB="topic"                | Topic                        |

### Helpers internes (3 fonctions privées)

| Fonction                      | Commande AT | Rôle                                                      |
| ----------------------------- | ----------- | --------------------------------------------------------- |
| parseFirstLine(const String&) | -           | Parse réponse, ignore AT/OK/ERROR, extrait valeur après : |
| escapeQuotes(const char*)     | -           | Échappe les " pour AT+MPUB                                |
| clearBuffer()                 | -           | Vide buffer série avant commande                          |