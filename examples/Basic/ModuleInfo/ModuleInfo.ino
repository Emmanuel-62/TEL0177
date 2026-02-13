#include <TEL0177.h>

TEL0177 modem(Serial1);
String pin = "0000";


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(2000);

  Serial.println("===> Module TEL0177 !!! <===");

  if(!modem.begin()){
    Serial.println("❌ L'initialisation a échoué ❌");
  }else{
    Serial.println("✔️ L'initialisation réussi ✔️");
  }

  Serial.println(modem.getModuleInfoString());
  delay(1000);
  modem.unlockSim(pin); // Code Pin de la SIM
  delay(1000);
  Serial.println(modem.getModuleInfoString());
  delay(1000);


}

void loop() {
  // put your main code here, to run repeatedly:
  delay(5000);

}
