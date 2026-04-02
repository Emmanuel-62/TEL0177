#ifndef TEL0177_TYPES_H
#define TEL0177_TYPES_H

// ==========================================================================
// CODE DE RETOUR UNIFIES
// ==========================================================================
typedef enum {
    TEL_OK = 0,         // Succès
    TEL_ERROR,          // Erreur générique
    TEL_TIMEOUT,        // Pas de réponse dans le délai imparti
    TEL_BUSY,           // Modem occupé
    TEL_NO_RESPONSE,    // Aucun octet reçu
    TEL_INVALID_PARAM,  // Paramètre invalide passé à la fonction
    TEL_NOT_CONNECTED   // Action requiert une connexion réseau absente
} TEL_Status;


// ==========================================================================
// CODE DE RETOUR UNIFIES
// ==========================================================================
typedef enum {
    NET_DETACHED = 0,   // Pas attaché au réseau GSM
    NET_ATTACHED,       // Attaché au réseau GSM (pas de data)
    NET_CONNECTED       // Contexte data ouvert, IP disponible
} TEL_NetState;


// ==========================================================================
// Structure message MQTT entrant
// ==========================================================================
typedef struct {
    char    topic[128];
    char    payload[512];
    uint8_t qos;
    uint16_t length;
} TEL_MQTT_Message;


#endif // TEL0177_TYPES_H