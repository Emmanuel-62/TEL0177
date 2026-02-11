class TEL0177 {
  private:
    TEL0177_AT _at;
    bool _initialized;
    bool _networkConnected;
    bool _mqttConnected;

  public:
    // Construction
    TEL0177(Stream& serial);
    
    // Init
    bool begin(uint32_t baudRate = 115200);
    void end();
    bool isReady();
    
    // Infos
    String getIMEI();
    ModuleInfo getModuleInfo();
    void printDiagnostics();
    
    // SIM
    bool isSimReady();
    bool unlockSim(const char* pin);
    
    // Réseau
    bool connectNetwork(uint32_t timeout = 60000);
    bool isNetworkConnected();
    SignalQuality getSignalQuality();
    int getSignalStrength();  
    
    // MQTT
    bool mqttBegin(const char* server, uint16_t port, const char* clientId, const char* user = "", const char* password = "");
    bool mqttConnect(bool cleanSession = true, uint16_t keepAlive = 60);
    bool mqttDisconnect();
    bool mqttIsConnected();
    bool mqttPublish(const char* topic, const char* message);
    bool mqttSubscribe(const char* topic, uint8_t qos = 0);
    
    // Utilitaires
    bool reset();
    String sendATCommand(const char* cmd);
    TEL0177_AT& getAT();  
};