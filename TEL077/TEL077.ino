#include "TEL0177.h"


TEL0177 tel (Serial1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  while(!Serial){;}

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("----------------------");
  if(tel.test()){
    Serial.println("bbbbbbbbbbbbbbbbbbbbbbbb");
  }else{
    Serial.println("mmmmmmmmmmmmmmmmmmmmmmmm");
  }
  delay(1000);

}
