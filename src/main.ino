#include <SoftwareSerial.h>
SoftwareSerial BTSerial(5, 6); //RX|TX
int message;

void setup(){
  Serial.begin(9600);
  BTSerial.begin(9600);

  Serial.println("AT commands: ");
}

void loop(){
  if(BTSerial.available()) {
    Serial.write(BTSerial.read());
  }


  if(Serial.available()) {
    message = Serial.read();
    BTSerial.write(message);
    Serial.write(message);
  }
}
