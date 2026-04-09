# Librairie TEL0177

## Structure de la librairie TEl0177

```text
    TEL0177/
    ├── src/
    │   │ 
    │   ├── TEL0177.h
    │   ├── TEL0177.cpp
    │   ├── TEL0177_Config.h
    │   ├── TEL0177_Types.h
    │   │
    │   ├── at/
    │   │   ├── TEL0177_AT_Core.h
    │   │   ├── TEL0177_AT_Core.cpp
    │   │
    │   ├── network/
    │   │   ├── TEL0177_Network.h
    │   │   ├── TEL0177_Network.cpp
    │   │
    │   ├── http/
    │   │   ├── TEL0177_HTTP.h
    │   │   ├── TEL0177_HTTP.cpp
    │   │
    │   ├── sms/
    │   │   ├── TEL0177_SMS.h
    │   │   ├── TEL0177_SMS.cpp
    │   │
    │   ├── mqtt/
    │   │   ├── TEL0177_MQTT.h
    │   │   ├── TEL0177_MQTT.cpp
    │
    ├── examples/
    │   └── Basic/
    │       └── ModuleInfo.ino
    │
    ├── keywords.txt              
    ├── library.properties        
    └── README.md                 
```

## Référence complète : Commandes AT → TEL0177_AT 

| Commande AT                                   | Utilité courte                                     |
| --------------------------------------------- | -------------------------------------------------- |
| AT                                            | Test de communication / commande de base           |
| AT+RESET                                      | Redémarrage logiciel du module                     |
| AT+RSTSET                                     | Rétablissement aux paramètres usine                |
| AT+CGMI                                       | Obtenir le fabricant                               |
| AT+CGMM                                       | Obtenir le modèle                                  |
| AT+CGMR                                       | Obtenir la version du firmware                     |
| AT+CGSN                                       | Récupérer l'IMEI                                   |
| AT+CCID                                       | Récupérer l'ICCID de la SIM                        |
| AT+CIMI                                       | Récupérer l'IMSI                                   |
| ATI                                           | Informations produit                               |
| AT+VER                                        | Version du logiciel                                |
| AT^HVER                                       | Version hardware                                   |
| AT*I                                          | Toutes les informations combinées                  |
| AT*EXINFO?                                    | Cause du dernier démarrage                         |
| AT+IPR?                                       | Voir le baud rate UART actuel                      |
| AT+CREG?                                      | Statut d'enregistrement réseau                     |
| AT+CSQ                                        | Qualité du signal (RSSI, BER)                      |
| AT+CPIN?                                      | Statut du PIN SIM                                  |
| AT+CPIN="xxxx"                                | Entrer le code PIN                                 |
| AT+MIPCLOSE                                   | Fermer la connexion TCP (utilisé pour MQTT)        |
| AT+CGATT=0                                    | Détacher le module du réseau de données            |
| AT+CPOWD=1                                    | Éteindre le module                                 |
| AT+MCONFIG="id","user","pass"                 | Configurer identifiants MQTT (client/user/pass)    |
| AT+MIPSTART="server","port"                   | Ouvrir connexion TCP vers le broker MQTT           |
| AT+MCONNECT=clean,keepalive                   | Établir la session MQTT (clean session, keepalive) |
| AT+MSUB="topic",qos                           | S'abonner à un topic MQTT                          |
| AT+MPUB="topic",qos,retain,"msg"              | Publier un message MQTT (topic, QoS, retain, msg)  |
| AT+MDISCONNECT                                | Déconnecter la session MQTT                        |
| AT+MUNSUB="topic"                             | Se désabonner d'un topic MQTT    