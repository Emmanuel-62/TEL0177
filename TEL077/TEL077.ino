#include "TEL0177.h"

TEL0177 modem(Serial1);

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    delay(2000);
    
    Serial.println("=== TEL0177 Test ===");
    
    // Test AT
    if(modem.test()) {
        Serial.println("✓ Module répond");
    } else {
        Serial.println("✗ Pas de réponse");
        while(1);
    }
    
    // Infos module
    Serial.print("Fabricant: ");
    Serial.println(modem._at.getManufacturer());
    delay(1000);
    
    Serial.print("Modèle: ");
    Serial.println(modem._at.getModel());
    delay(1000);

    Serial.print("Version information: ");
    Serial.println(modem._at.getFirmwareVersion());
    delay(1000);
    
    Serial.print("IMEI: ");
    Serial.println(modem._at.getIMEI());
    delay(1000);

    Serial.print("CCID: ");
    Serial.println(modem._at.getICCID());
    delay(1000);
    
    Serial.print("CPIN?: ");
    Serial.println(modem._at.getSimStatus());
    delay(1000);

    Serial.print("Code PIN : ");
    Serial.println(modem._at.mqttCheckSimReady("0000"));
    delay(1000);

    Serial.print("CPIN?: ");
    Serial.println(modem._at.getSimStatus());
    delay(1000);

    Serial.print("IMSI: ");
    Serial.println(modem._at.getIMSI());
    delay(1000);

    Serial.print("All Info: ");
    Serial.println(modem._at.getProductInfo());
    delay(1000);

    Serial.print("Software version: ");
    Serial.println(modem._at.getSwVersion());
    delay(1000);

    Serial.print("Hardware version: ");
    Serial.println(modem._at.getHwVersion());
    delay(1000);

    Serial.print("Various information: ");
    Serial.println(modem._at.getAllInfo());
    delay(1000);

    Serial.print("Boot: ");
    Serial.println(modem._at.getBootCause());
    delay(1000);

    Serial.print("TE-TA baud: ");
    Serial.println(modem._at.getUartBaud());
    delay(1000);

    Serial.print("Network registration: ");
    Serial.println(modem._at.getNetworkRegStatus());
    delay(1000);

    Serial.print("Signal quality: ");
    Serial.println(modem._at.getSignalQuality());
    delay(1000);

    Serial.print("Sim: ");
    Serial.println(modem._at.getSimStatus());
    delay(1000);

    Serial.print("Attached : ");
    Serial.println(modem._at.getDataAttachStatus());
    delay(1000);

    //-------------------------------------------------------------------------------------------------
    Serial.println("");
    Serial.println("");
    Serial.println("--> MQTT");

    Serial.print("MQTT Config : ");
    Serial.println(modem._at.mqttConfig("Emmanuel","gamasenseit","gamasenseit"));
    delay(1000);

    Serial.print("MQTT Connection : ");
    Serial.println(modem._at.mqttTcpConnect("mqtt.qameleo.fr","1935"));
    delay(1000);

    Serial.print("MQTT Session Connection : ");
    Serial.println(modem._at.mqttConnect("1","60"));
    delay(1000);

    Serial.print("MQTT Subscribe Topic : ");
    Serial.println(modem._at.mqttSubscribe("gamasenseit", 0));
    delay(5000);

    Serial.print("MQTT Subscribe Topic : ");
    modem._at.mqttPublish("gamasenseit", 1, 0, "Emmanuel message pour ça marche faut dire 'Bonjour boss'");
    delay(1000);

    //-------------------------------------------------------------------------------------------------

    delay(5000);


    

    Serial.println("********************* / FIN / ************************");

    Serial.print("RESET : ");
    if(modem._at.reset()){
        Serial.println("OK");
    }else{
        Serial.println("Error !!!");
    }
}

void loop() {
    delay(5000);
}