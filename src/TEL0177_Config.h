#ifndef TEL0177_CONFIG_H
#define TEL0177_CONFIG_H

#include <Arduino.h>
#include <stdio.h>

/**
 * @file TEL0177_Config.h
 * @brief Définit les constantes globales, tailles de buffers, timeouts
 *        et macros de journalisation de la librairie TEL0177.
 */


// ====================================================================
// PORT DE LOG
// ====================================================================

/**
 * @brief Port série utilisé pour les logs de la librairie.
 *
 * Peut être redéfini avant l'inclusion du fichier si nécessaire.
 */
#ifndef TEL_LOG_PORT
#define TEL_LOG_PORT Serial
#endif


// ====================================================================
// ACTIVATION DES TRACES
// ====================================================================

/**
 * @brief Active ou désactive les logs standards.
 * 1 = activé, 0 = désactivé.
 */
#ifndef TEL_ENABLE_LOG
#define TEL_ENABLE_LOG 1
#endif

/**
 * @brief Active ou désactive les logs de debug.
 * 1 = activé, 0 = désactivé.
 */
#ifndef TEL_ENABLE_DEBUG
#define TEL_ENABLE_DEBUG 1
#endif


// ====================================================================
// VITESSE SÉRIE PAR DÉFAUT
// ====================================================================

/**
 * @brief Vitesse série par défaut du module.
 */
#define TEL_DEFAULT_BAUD 115200


// ====================================================================
// MACROS DE LOG
// ====================================================================

#if TEL_ENABLE_LOG
  #define TEL_LOG(x)                do { TEL_LOG_PORT.print(x); } while (0)
  #define TEL_LOGLN(x)              do { TEL_LOG_PORT.println(x); } while (0)
  #define TEL_LOG_ERROR(fmt, ...)   do { char _buf[256]; snprintf(_buf, sizeof(_buf), fmt, ##__VA_ARGS__); TEL_LOG_PORT.print(_buf); } while (0)
  #define TEL_LOGLN_ERROR(fmt, ...) do { char _buf[256]; snprintf(_buf, sizeof(_buf), fmt, ##__VA_ARGS__); TEL_LOG_PORT.println(_buf); } while (0)
#else
  #define TEL_LOG(x)                do {} while (0)
  #define TEL_LOGLN(x)              do {} while (0)
  #define TEL_LOG_ERROR(fmt, ...)   do {} while (0)
  #define TEL_LOGLN_ERROR(fmt, ...) do {} while (0)
#endif

#if TEL_ENABLE_DEBUG
  #define TEL_DEBUG(x)              do { TEL_LOG_PORT.print(x); } while (0)
  #define TEL_DEBUGLN(x)            do { TEL_LOG_PORT.println(x); } while (0)
#else
  #define TEL_DEBUG(x)              do {} while (0)
  #define TEL_DEBUGLN(x)            do {} while (0)
#endif


// ====================================================================
// TIMEOUTS
// ====================================================================

/**
 * @brief Timeout générique par défaut.
 */
#define TEL_TIMEOUT_DEFAULT             3000UL

/**
 * @brief Timeout pour les commandes AT simples.
 */
#define TEL_TIMEOUT_DEFAULT_AT          2000UL

/**
 * @brief Timeout pour les opérations réseau.
 */
#define TEL_TIMEOUT_NETWORK             15000UL

/**
 * @brief Timeout pour les requêtes HTTP.
 */
#define TEL_TIMEOUT_HTTP                30000UL

/**
 * @brief Timeout pour les opérations MQTT.
 */
#define TEL_TIMEOUT_MQTT                10000UL

/**
 * @brief Timeout pour les opérations SMS.
 */
#define TEL_TIMEOUT_SMS                 10000UL

/**
 * @brief Temps d’attente après démarrage ou reset du module.
 */
#define TEL_TIMEOUT_BOOT                5000UL

/**
 * @brief Délai d’attente entre deux lectures de réponse AT.
 */
#define TEL_TIMEOUT_AT_WAIT_RESPONSE    1000UL


// ====================================================================
// RÉPONSES AT STANDARD
// ====================================================================

/**
 * @brief Réponse AT indiquant un succès.
 */
#define TEL_RESPONSE_OK                 "OK"

/**
 * @brief Réponse AT indiquant une erreur.
 */
#define TEL_RESPONSE_ERROR              "ERROR"


// ====================================================================
// RETRY
// ====================================================================

/**
 * @brief Nombre maximal de tentatives pour certaines opérations.
 */
#define TEL_MAX_RETRIES                 3

/**
 * @brief Délai entre deux tentatives.
 */
#define TEL_RETRY_DELAY_MS              500UL


// ====================================================================
// TAILLES DES BUFFERS
// ====================================================================

/**
 * @brief Taille du buffer de réception brut.
 */
#define TEL_RX_BUFFER_SIZE              1024

/**
 * @brief Taille maximale d’une ligne lue depuis le modem.
 */
#define TEL_LINE_BUFFER_SIZE            256

/**
 * @brief Taille maximale d’une commande AT construite.
 */
#define TEL_CMD_BUFFER_SIZE             256

/**
 * @brief Taille maximale du corps HTTP.
 */
#define TEL_HTTP_BODY_SIZE              1024

/**
 * @brief Taille maximale du topic MQTT.
 */
#define TEL_MQTT_TOPIC_SIZE             128

/**
 * @brief Taille maximale du payload MQTT.
 */
#define TEL_MQTT_PAYLOAD_SIZE           512


// ====================================================================
// PARAMÈTRES RÉSEAU ET MQTT
// ====================================================================

/**
 * @brief Identifiant du contexte PDP utilisé pour la session data.
 */
#define TEL_PDP_CONTEXT_ID              1

/**
 * @brief Port MQTT par défaut.
 */
#define TEL_MQTT_DEFAULT_PORT           1883

/**
 * @brief Intervalle Keep Alive MQTT en secondes.
 */
#define TEL_MQTT_KEEP_ALIVE             60

#endif // TEL0177_CONFIG_H