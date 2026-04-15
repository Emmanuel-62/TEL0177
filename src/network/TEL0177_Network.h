#ifndef TEL0177_NETWORK_H
#define TEL0177_NETWORK_H

#include <Arduino.h>

#include "../at/TEL0177_AT_Core.h"
#include "../TEL0177_Types.h"
#include "../TEL0177_Config.h"

/**
 * @file TEL0177_Network.h
 * @brief Gestion SIM, réseau cellulaire et attachement data du module TEL0177.
 *
 * Cette classe encapsule les commandes AT liées à la carte SIM,
 * à l'enregistrement réseau, à la qualité du signal et à l'attachement data.
 */
class TEL0177_Network {
  private:
    /**
     * @brief Cœur AT utilisé pour communiquer avec le modem.
     */
    TEL0177_AT_Core* _at = nullptr;

    /**
     * @brief État réseau local mémorisé par la librairie.
     */
    TEL_NetState _state = NET_DETACHED;

    /**
     * @brief Indicateur de disponibilité de la carte SIM.
     */
    bool _simReady = false;

    /**
     * @brief Indicateur d'enregistrement sur le réseau mobile.
     */
    bool _networkReady = false;

    /**
     * @brief Indicateur d'attachement au service data.
     */
    bool _dataReady = false;

    /**
     * @brief Dernière adresse IP connue.
     */
    char _ip[TEL_IP_SIZE] = {0};

    /**
     * @brief APN actuellement configuré.
     */
    char _apn[TEL_APN_SIZE] = {0};

    /**
     * @brief Configure l'APN utilisé pour la connexion data.
     * @param apn Nom de l'APN opérateur.
     * @return Code d'état TEL_Status.
     */
    TEL_Status _configureAPN(const char* apn);

    /**
     * @brief Attend l'enregistrement du modem sur le réseau.
     * @param timeout Délai maximal d'attente en millisecondes.
     * @return TEL_OK si le modem est enregistré, sinon un code d'erreur.
     */
    TEL_Status _waitForNetworkReg(uint32_t timeout);

  public:
    /**
     * @brief Initialise la couche réseau avec le cœur AT.
     * @param at Référence vers l'objet TEL0177_AT_Core déjà initialisé.
     */
    void begin(TEL0177_AT_Core& at);

    /**
     * @brief Vérifie si la carte SIM est prête.
     *
     * Utilise typiquement la commande AT+CPIN?.
     *
     * @return TEL_OK si la SIM est prête, sinon un code d'erreur.
     */
    TEL_Status isSIMReady();

    /**
     * @brief Déverrouille la carte SIM avec un code PIN.
     * @param pin Code PIN de la carte SIM.
     * @return TEL_OK si le déverrouillage réussit, sinon un code d'erreur.
     */
    TEL_Status unlockSIM(const char* pin);

    /**
     * @brief Vérifie si le modem est enregistré sur le réseau opérateur.
     *
     * Utilise typiquement la commande AT+CREG?.
     *
     * @return TEL_OK si le modem est enregistré, sinon un code d'erreur.
     */
    TEL_Status isNetworkRegistered();

    /**
     * @brief Attend que le modem soit enregistré sur le réseau opérateur.
     * @param timeout Délai maximal d'attente en millisecondes.
     * @return TEL_OK si le réseau est disponible, sinon un code d'erreur.
     */
    TEL_Status waitForNetwork(uint32_t timeout = TEL_TIMEOUT_NETWORK);

    /**
     * @brief Vérifie si le modem est attaché au service data.
     *
     * Utilise typiquement la commande AT+CGATT?.
     *
     * @return TEL_OK si le modem est attaché à la data, sinon un code d'erreur.
     */
    TEL_Status isDataAttached();

    /**
     * @brief Attache le modem au service data.
     *
     * Utilise typiquement la commande AT+CGATT=1.
     *
     * @return TEL_OK si l'attachement data réussit, sinon un code d'erreur.
     */
    TEL_Status attachData();

    /**
     * @brief Détache le modem du service data.
     *
     * Utilise typiquement la commande AT+CGATT=0.
     *
     * @return TEL_OK si le détachement réussit, sinon un code d'erreur.
     */
    TEL_Status detachData();

    /**
     * @brief Configure l'APN et prépare la connexion data.
     * @param apn Nom de l'APN opérateur.
     * @param user Nom d'utilisateur APN, optionnel.
     * @param pass Mot de passe APN, optionnel.
     * @return TEL_OK si la configuration réussit, sinon un code d'erreur.
     */
    TEL_Status connectData(const char* apn, const char* user = nullptr, const char* pass = nullptr);

    /**
     * @brief Ferme ou désactive la connexion data.
     * @return TEL_OK si la déconnexion réussit, sinon un code d'erreur.
     */
    TEL_Status disconnectData();

    /**
     * @brief Vérifie si la connexion data est active.
     * @return TEL_OK si la data est active, sinon un code d'erreur.
     */
    TEL_Status isDataConnected();

    /**
     * @brief Lit la qualité du signal radio.
     *
     * Utilise typiquement la commande AT+CSQ.
     *
     * @param rssi Référence recevant la valeur RSSI.
     * @param ber Référence recevant la valeur BER.
     * @return TEL_OK si la lecture réussit, sinon un code d'erreur.
     */
    TEL_Status getSignalQuality(int& rssi, int& ber);

    /**
     * @brief Lit l'adresse IP actuellement attribuée au module.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return TEL_OK si une adresse IP est récupérée, sinon un code d'erreur.
     */
    TEL_Status getIPAddress(char* buffer, uint16_t size);

    /**
     * @brief Lit l'opérateur réseau actuel.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return TEL_OK si l'opérateur est récupéré, sinon un code d'erreur.
     */
    TEL_Status getOperator(char* buffer, uint16_t size);

    /**
     * @brief Lit l'IMEI du module.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return TEL_OK si l'IMEI est récupéré, sinon un code d'erreur.
     */
    TEL_Status getIMEI(char* buffer, uint16_t size);

    /**
     * @brief Lit l'ICCID de la carte SIM.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return TEL_OK si l'ICCID est récupéré, sinon un code d'erreur.
     */
    TEL_Status getICCID(char* buffer, uint16_t size);

    /**
     * @brief Lit l'IMSI de la carte SIM.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return TEL_OK si l'IMSI est récupéré, sinon un code d'erreur.
     */
    TEL_Status getIMSI(char* buffer, uint16_t size);

    /**
     * @brief Retourne l'état réseau mémorisé.
     * @return État réseau TEL_NetState.
     */
    TEL_NetState getState() const;
};

#endif // TEL0177_NETWORK_H