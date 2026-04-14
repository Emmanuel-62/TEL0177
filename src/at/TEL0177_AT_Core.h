#ifndef TEL0177_AT_CORE_H
#define TEL0177_AT_CORE_H

#include <Arduino.h>
#include <Stream.h>

#include "../TEL0177_Config.h"
#include "../TEL0177_Types.h"

/**
 * @file TEL0177_AT_Core.h
 * @brief Couche bas niveau de communication AT avec le module TEL0177.
 *
 * Cette classe encapsule l'envoi de commandes AT, l'attente de réponses,
 * la lecture sur le port série et quelques utilitaires de base.
 */
class TEL0177_AT_Core {
  private:
    /** 
     * @brief Flux série utilisé pour communiquer avec le modem.
     */
    Stream * _serial = nullptr;

    /**
     * @brief Buffer interne de réception.
     */
    char _rxBuffer[TEL_RX_BUFFER_SIZE] = {0};

    // ==========================================================================
    // FONCTIONS PRIVÉES
    // ==========================================================================
    /**
     * @brief Attend une réponse "OK" ou "ERROR" du modem.
     * @param timeout Délai maximal d'attente en millisecondes.
     * @return TEL_OK si "OK" est reçu, TEL_ERROR si "ERROR" est reçu,
     *         TEL_TIMEOUT si aucun résultat avant expiration.
     */
    TEL_Status _waitForOKorERROR(const uint32_t timeout);

    /**
     * @brief Vérifie si une sous-chaîne est présente dans une chaîne.
     * @param haystack Chaîne principale.
     * @param needle Sous-chaîne recherchée.
     * @return true si trouvée, false sinon.
     */
    bool _contains(const char* haystack, const char* needle);

  public:
    // ==========================================================================
    // INITIALISATION
    // ==========================================================================

    /**
     * @brief Associe le port série au cœur AT.
     * @param serial Flux série déjà initialisé.
     */
    void begin(Stream& serial);

    // ==========================================================================
    // ENVOI
    // ==========================================================================

    /**
     * @brief Envoie une commande AT terminée par CRLF et attend "OK" ou "ERROR".
     * @param command Commande AT à envoyer.
     * @param timeout Délai maximal d'attente.
     * @return Code d'état TEL_Status.
     */
    TEL_Status send(const char* command, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    /**
     * @brief Envoie une commande AT puis attend une sous-chaîne précise.
     * @param command Commande AT à envoyer.
     * @param expected Réponse attendue.
     * @param timeout Délai maximal d'attente.
     * @return TEL_OK si la réponse attendue est trouvée, sinon TEL_TIMEOUT.
     */
    TEL_Status sendAndWait(const char* command, const char* expected, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    /**
     * @brief Envoie une commande AT puis lit toute la réponse dans un buffer.
     * @param command Commande AT à envoyer.
     * @param buffer Buffer de destination.
     * @param bufferSize Taille du buffer.
     * @param timeout Délai maximal d'attente.
     * @return Code d'état TEL_Status.
     */
    TEL_Status sendAndRead(const char* command, char* buffer, uint16_t bufferSize, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    /**
     * @brief Envoie une commande AT puis lit toute la réponse dans un buffer brut
     */
    TEL_Status sendAndReadRaw(const char* command, char* buffer, uint16_t size, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    /**
     * @brief Envoie une commande AT puis lit toute la réponse dans un buffer et vérifie si c'est OK ou ERROR 
     */
    TEL_Status sendAndReadChecked(const char* command, char* buffer, uint16_t size, uint32_t timeout = TEL_TIMEOUT_DEFAULT);
    /**
     * @brief Envoie des données brutes sans ajouter CRLF.
     * @param data Données à envoyer.
     * @param length Longueur des données. Si 0, la longueur de la chaîne est utilisée.
     * @return Code d'état TEL_Status.
     */
    TEL_Status sendRaw(const char* data, uint16_t length = 0);

    // ==========================================================================
    // LECTURE
    // ==========================================================================

    /**
     * @brief Attend qu'une sous-chaîne apparaisse dans le flux entrant.
     * @param expected Sous-chaîne recherchée.
     * @param timeout Délai maximal d'attente.
     * @return true si trouvée, false sinon.
     */
    bool waitFor(const char* expected, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    /**
     * @brief Lit une ligne terminée par '\n'.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @param timeout Délai maximal d'attente.
     * @return true si une ligne valide est lue, false sinon.
     */
    bool readLine(char* buffer, uint16_t size, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    /**
     * @brief Lit tous les octets disponibles jusqu'à expiration du timeout.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @param timeout Délai maximal d'attente.
     * @return Nombre d'octets copiés dans le buffer.
     */
    uint16_t readAll(char* buffer, uint16_t size, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    // ==========================================================================
    // UTILITAIRES
    // ==========================================================================

    /**
     * @brief Vide le flux série entrant et réinitialise le buffer interne.
     */
    void flush();

    /**
     * @brief Indique si des données sont disponibles sur le port série.
     * @return true si des données sont disponibles, false sinon.
     */
    bool isAvailable();

    /**
     * @brief Vérifie si le modem répond à la commande "AT".
     * @return true si le modem répond correctement, false sinon.
     */
    bool isAlive();
};

#endif // TEL0177_AT_CORE_H