#ifndef TEL0177_MQTT_H
#define TEL0177_MQTT_H

#include "../at/TEL0177_AT_Core.h"
#include "../TEL0177_Config.h"
#include "../TEL0177_Types.h"

/**
 * @file TEL0177_MQTT.h
 * @brief Gestion MQTT du module TEL0177.
 *
 * Cette classe encapsule les commandes AT liées au MQTT :
 * configuration client, connexion au broker, publication,
 * abonnement, désabonnement et réception de messages entrants.
 */

/**
 * @brief Callback appelé lorsqu'un message MQTT est reçu.
 *
 * @param topic Topic MQTT du message reçu.
 * @param payload Contenu du message reçu.
 * @param length Longueur utile du payload.
 */
typedef void (*TEL_MQTT_Callback)(const char* topic, const char* payload, uint16_t length);

/**
 * @brief Callback brut appelé lorsqu'une ligne URC MQTT est reçue.
 *
 * Utile pour le debug ou pour analyser le format exact retourné par le modem.
 *
 * @param urc Ligne brute reçue depuis le modem.
 */
typedef void (*TEL_MQTT_RawCallback)(const char* urc);


class TEL0177_MQTT {
  private:
    /**
     * @brief Pointeur vers le cœur AT utilisé pour communiquer avec le modem.
     */
    TEL0177_AT_Core* _at = nullptr;

    /**
     * @brief Callback utilisateur appelé lorsqu'un message MQTT est décodé.
     */
    TEL_MQTT_Callback _callback = nullptr;

    /**
     * @brief Callback utilisateur appelé avec l'URC brut reçu.
     */
    TEL_MQTT_RawCallback _rawCallback = nullptr;

    /**
     * @brief État local de la connexion MQTT.
     */
    bool _connected = false;

    /**
     * @brief Adresse du broker MQTT.
     */
    char _host[TEL_MQTT_HOST_SIZE] = {0};

    /**
     * @brief Identifiant client MQTT.
     */
    char _clientId[TEL_MQTT_CLIENT_ID_SIZE] = {0};

    /**
     * @brief Nom d'utilisateur MQTT.
     */
    char _user[TEL_MQTT_USER_SIZE] = {0};

    /**
     * @brief Mot de passe MQTT.
     */
    char _pass[TEL_MQTT_PASS_SIZE] = {0};

    /**
     * @brief Port du broker MQTT.
     */
    uint16_t _port = TEL_MQTT_DEFAULT_PORT;

    /**
     * @brief Keep-alive MQTT en secondes.
     */
    uint16_t _keepAlive = TEL_MQTT_KEEP_ALIVE;

    /**
     * @brief Configure l'adresse et le port du broker dans le modem.
     * @param host Adresse du broker MQTT.
     * @param port Port du broker MQTT.
     * @return Code d'état TEL_Status.
     */
    TEL_Status _configBroker(const char* host, uint16_t port);

    /**
     * @brief Configure les identifiants MQTT dans le modem.
     * @param clientId Identifiant client MQTT.
     * @param user Nom d'utilisateur MQTT, optionnel.
     * @param pass Mot de passe MQTT, optionnel.
     * @return Code d'état TEL_Status.
     */
    TEL_Status _configCredentials(const char* clientId, const char* user, const char* pass);

    /**
     * @brief Analyse une ligne URC MQTT reçue du modem.
     * @param urc Ligne brute reçue.
     * @param msg Structure de destination à remplir.
     * @return true si le message a été correctement analysé, false sinon.
     */
    bool _parseIncomingMessage(const char* urc, TEL_MQTT_Message& msg);

    /**
     * @brief Traite une ligne MQTT reçue et appelle les callbacks si nécessaire.
     * @param urc Ligne brute reçue.
     */
    void _handleRecv(const char* urc);

  public:
    /**
     * @brief Initialise la couche MQTT avec le cœur AT.
     * @param at Référence vers l'objet TEL0177_AT_Core déjà initialisé.
     */
    void begin(TEL0177_AT_Core& at);

    /**
     * @brief Configure l'identité du client MQTT.
     * @param clientId Identifiant client MQTT.
     * @param user Nom d'utilisateur MQTT, optionnel.
     * @param pass Mot de passe MQTT, optionnel.
     * @param keepAlive Keep-alive MQTT en secondes.
     */
    void setClientConfig(
      const char* clientId,
      const char* user = nullptr,
      const char* pass = nullptr,
      uint16_t keepAlive = TEL_MQTT_KEEP_ALIVE
    );

    /**
     * @brief Se connecte à un broker MQTT.
     * @param host Adresse du broker MQTT.
     * @param port Port du broker MQTT.
     * @param timeout Délai maximal d'attente.
     * @return TEL_OK si la connexion réussit, sinon un code d'erreur.
     */
    TEL_Status connect(const char* host, uint16_t port, uint32_t timeout = TEL_TIMEOUT_MQTT);

    /**
     * @brief Déconnecte le client MQTT du broker.
     * @return Code d'état TEL_Status.
     */
    TEL_Status disconnect();

    /**
     * @brief Vérifie si la connexion MQTT est active.
     * @return true si connecté, false sinon.
     */
    bool isConnected();

    /**
     * @brief Tente une reconnexion MQTT avec les derniers paramètres connus.
     * @return Code d'état TEL_Status.
     */
    TEL_Status reconnect();

    /**
     * @brief Publie un message MQTT.
     * @param topic Topic de destination.
     * @param payload Message à publier.
     * @param qos Niveau QoS MQTT.
     * @param retain true pour conserver le message côté broker.
     * @param timeout Délai maximal d'attente.
     * @return Code d'état TEL_Status.
     */
    TEL_Status publish(
      const char* topic,
      const char* payload,
      uint8_t qos = 0,
      bool retain = false,
      uint32_t timeout = TEL_TIMEOUT_MQTT
    );

    /**
     * @brief S'abonne à un topic MQTT.
     * @param topic Topic à écouter.
     * @param qos Niveau QoS demandé.
     * @return Code d'état TEL_Status.
     */
    TEL_Status subscribe(const char* topic, uint8_t qos = 0);

    /**
     * @brief Se désabonne d'un topic MQTT.
     * @param topic Topic à ne plus écouter.
     * @return Code d'état TEL_Status.
     */
    TEL_Status unsubscribe(const char* topic);

    /**
     * @brief Traite les événements MQTT entrants.
     *
     * À appeler régulièrement dans la boucle principale pour recevoir
     * les messages MQTT et détecter les déconnexions.
     */
    void loop();

    /**
     * @brief Traite les événements MQTT pendant une durée donnée.
     * @param durationMs Durée de traitement en millisecondes.
     */
    void loopFor(uint32_t durationMs);

    /**
     * @brief Définit le callback appelé lorsqu'un message MQTT est reçu.
     * @param callback Fonction utilisateur à appeler.
     */
    void setCallback(TEL_MQTT_Callback callback);

    /**
     * @brief Définit le callback appelé avec les URC MQTT bruts.
     * @param callback Fonction utilisateur à appeler.
     */
    void setRawCallback(TEL_MQTT_RawCallback callback);
};

#endif // TEL0177_MQTT_H