#include "TEL0177.h"

TEL0177 modem(Serial1);

// Variables globales pour les tests
bool allTestsPassed = true;
int testsPassed = 0;
int testsFailed = 0;

// ============================================
// FONCTIONS HELPER POUR LES TESTS
// ============================================
void printSeparator() {
    Serial.println("\n========================================");
}

void printTestHeader(const char* category) {
    printSeparator();
    Serial.print(">>> ");
    Serial.print(category);
    Serial.println(" <<<");
    printSeparator();
}

void printTest(const char* testName, bool success) {
    Serial.print("[");
    if (success) {
        Serial.print("✓");
        testsPassed++;
    } else {
        Serial.print("✗");
        testsFailed++;
        allTestsPassed = false;
    }
    Serial.print("] ");
    Serial.println(testName);
}

void printTestWithValue(const char* testName, const String& value, bool success = true) {
    Serial.print("[");
    if (success && value.length() > 0) {
        Serial.print("✓");
        testsPassed++;
    } else {
        Serial.print("✗");
        testsFailed++;
        allTestsPassed = false;
        success = false;
    }
    Serial.print("] ");
    Serial.print(testName);
    Serial.print(": ");
    if (success) {
        Serial.println(value);
    } else {
        Serial.println("(VIDE OU ERREUR)");
    }
    delay(500);
}

void printFinalResults() {
    printSeparator();
    Serial.println("=== RÉSULTATS FINAUX ===");
    Serial.print("Tests réussis: ");
    Serial.println(testsPassed);
    Serial.print("Tests échoués: ");
    Serial.println(testsFailed);
    Serial.print("Total: ");
    Serial.println(testsPassed + testsFailed);
    
    if (allTestsPassed) {
        Serial.println("\n🎉 TOUS LES TESTS SONT PASSÉS !");
    } else {
        Serial.println("\n⚠️  CERTAINS TESTS ONT ÉCHOUÉ");
    }
    printSeparator();
}

// ============================================
// SETUP - TOUS LES TESTS
// ============================================
void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    delay(3000);
    
    Serial.println("\n\n");
    printSeparator();
    Serial.println("=== TEST COMPLET TEL0177 ===");
    Serial.println("Date: " + String(__DATE__) + " " + String(__TIME__));
    printSeparator();
    delay(2000);

    // ============================================
    // TEST 1: COMMUNICATION DE BASE
    // ============================================
    printTestHeader("1. COMMUNICATION DE BASE");
    
    Serial.print("[...] Test AT... ");
    bool atOk = modem._at.getAt();
    if (atOk) {
        Serial.println("✓ OK");
        testsPassed++;
    } else {
        Serial.println("✗ ÉCHEC");
        Serial.println("\n⚠️  MODULE NE RÉPOND PAS !");
        Serial.println("Vérifiez:");
        Serial.println("  - Câblage TX/RX");
        Serial.println("  - Alimentation du module");
        Serial.println("  - Baud rate (115200)");
        testsFailed++;
        while(1); // Arrêt si pas de communication
    }
    delay(1000);

    // ============================================
    // TEST 2: INFORMATIONS MODULE
    // ============================================
    printTestHeader("2. INFORMATIONS MODULE");
    
    printTestWithValue("Fabricant", modem._at.getManufacturer());
    printTestWithValue("Modèle", modem._at.getModel());
    printTestWithValue("Firmware", modem._at.getFirmwareVersion());
    printTestWithValue("IMEI", modem._at.getIMEI());
    printTestWithValue("Produit Info", modem._at.getProductInfo());
    printTestWithValue("Version SW", modem._at.getSwVersion());
    printTestWithValue("Version HW", modem._at.getHwVersion());
    printTestWithValue("Toutes infos", modem._at.getAllInfo());
    printTestWithValue("Cause boot", modem._at.getBootCause());
    printTestWithValue("UART Baud", modem._at.getUartBaud());

    // ============================================
    // TEST 3: CARTE SIM
    // ============================================
    printTestHeader("3. CARTE SIM");
    
    String simStatus = modem._at.getSimStatus();
    printTestWithValue("État SIM (avant PIN)", simStatus);
    
    // Vérifier si PIN requis
    if (simStatus.indexOf("SIM PIN") >= 0) {
        Serial.println("\n[INFO] PIN requis, envoi du code PIN...");
        bool pinOk = modem._at.setSimPin("0000");
        printTest("Envoi code PIN (0000)", pinOk);
        delay(2000);
        
        simStatus = modem._at.getSimStatus();
        printTestWithValue("État SIM (après PIN)", simStatus);
    } else if (simStatus.indexOf("READY") >= 0) {
        Serial.println("\n[INFO] SIM déjà déverrouillée");
    } else {
        Serial.println("\n[WARN] État SIM inconnu: " + simStatus);
    }
    
    printTestWithValue("ICCID", modem._at.getICCID());
    printTestWithValue("IMSI", modem._at.getIMSI());

    // ============================================
    // TEST 4: RÉSEAU
    // ============================================
    printTestHeader("4. RÉSEAU CELLULAIRE");
    
    printTestWithValue("Enregistrement réseau", modem._at.getNetworkRegStatus());
    printTestWithValue("Qualité signal", modem._at.getSignalQuality());
    printTestWithValue("Attachement data", modem._at.getDataAttachStatus());
    
    // Attendre enregistrement réseau
    Serial.println("\n[INFO] Attente enregistrement réseau (max 60s)...");
    unsigned long startWait = millis();
    bool registered = false;
    
    while (millis() - startWait < 60000) {
        String regStatus = modem._at.getNetworkRegStatus();
        
        if (regStatus.indexOf(",1") >= 0 || regStatus.indexOf(",5") >= 0) {
            registered = true;
            break;
        }
        
        Serial.print(".");
        delay(2000);
    }
    Serial.println();
    
    printTest("Enregistrement réseau", registered);
    
    if (registered) {
        // Attendre attachement data
        Serial.println("\n[INFO] Attente attachement data (max 30s)...");
        startWait = millis();
        bool attached = false;
        
        while (millis() - startWait < 30000) {
            String attachStatus = modem._at.getDataAttachStatus();
            
            if (attachStatus.indexOf("1") >= 0) {
                attached = true;
                break;
            }
            
            Serial.print(".");
            delay(2000);
        }
        Serial.println();
        
        printTest("Attachement data", attached);
    } else {
        Serial.println("\n[WARN] Réseau non disponible, tests MQTT ignorés");
        testsFailed++;
    }

    // ============================================
    // TEST 5: MQTT
    // ============================================
    printTestHeader("5. MQTT");
    
    if (registered) {
        // Configuration MQTT
        bool mqttConfigOk = modem._at.mqttConfig("TestClient_TEL0177", "gamasenseit", "gamasenseit");
        printTest("Configuration MQTT", mqttConfigOk);
        delay(1000);
        
        // Connexion TCP
        bool tcpOk = modem._at.mqttTcpConnect("mqtt.qameleo.fr", 1935);
        printTest("Connexion TCP MQTT", tcpOk);
        delay(2000);
        
        if (tcpOk) {
            // Connexion MQTT
            bool mqttConnectOk = modem._at.mqttConnect(true, 60);
            printTest("Session MQTT", mqttConnectOk);
            delay(2000);
            
            if (mqttConnectOk) {
                // Souscription
                bool subOk = modem._at.mqttSubscribe("gamasenseit/Emma_4_v2", 0);
                printTest("Souscription topic", subOk);
                delay(1000);
                
                // Publication
                Serial.println("\n[INFO] Publication message test...");
                bool pubOk = modem._at.mqttPublish(
                    "gamasenseit/Emma_4_v2", 
                    "Test TEL0177 - Message de test automatique", 
                    0, 
                    false
                );
                printTest("Publication message", pubOk);
                delay(1000);
                
                // Publication avec caractères spéciaux
                Serial.println("\n[INFO] Test caractères spéciaux...");
                bool pubSpecialOk = modem._at.mqttPublish(
                    "gamasenseit/Emma_4_v2", 
                    "Message avec 'guillemets simples' et caractères: éàç!@#", 
                    1, 
                    false
                );
                printTest("Publication caractères spéciaux", pubSpecialOk);
                delay(1000);
                
            } else {
                Serial.println("\n[WARN] Session MQTT échouée, tests pub/sub ignorés");
            }
        } else {
            Serial.println("\n[WARN] Connexion TCP échouée, tests MQTT ignorés");
        }
    }

    // ============================================
    // TEST 6: COMMANDES SYSTÈME
    // ============================================
    printTestHeader("6. COMMANDES SYSTÈME");
    
    Serial.println("\n[INFO] Ces tests nécessitent un redémarrage du module");
    Serial.println("[INFO] Ils sont optionnels, commentez-les si non souhaités\n");
    
    delay(2000);
    
    // Test reset (attention, redémarre le module!)
    Serial.println("[WARN] Test RESET dans 5 secondes...");
    delay(5000);
    
    bool resetOk = modem._at.reset();
    printTest("Reset module", resetOk);
    
    if (resetOk) {
        Serial.println("\n[INFO] Module redémarré, attente 10s...");
        delay(10000);
        
        // Re-tester communication après reset
        bool atAfterReset = modem._at.getAt();
        printTest("Communication après reset", atAfterReset);
    }

    // ============================================
    // RÉSULTATS FINAUX
    // ============================================
    delay(2000);
    printFinalResults();
    
    Serial.println("\n[INFO] Tests terminés, passage en mode monitoring...\n");
}

// ============================================
// LOOP - MONITORING
// ============================================
void loop() {
    static unsigned long lastCheck = 0;
    
    if (millis() - lastCheck > 10000) {
        Serial.println("\n--- Monitoring (toutes les 10s) ---");
        
        String signal = modem._at.getSignalQuality();
        Serial.print("Signal: ");
        Serial.println(signal);
        
        String network = modem._at.getNetworkRegStatus();
        Serial.print("Réseau: ");
        Serial.println(network);
        
        lastCheck = millis();
    }
}