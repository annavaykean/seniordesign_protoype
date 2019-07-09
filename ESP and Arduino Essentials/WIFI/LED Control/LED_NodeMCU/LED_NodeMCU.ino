// NodeMCU Code for Receiving end

#include <SoftwareSerial.h>
SoftwareSerial toUno(D6, D5); //Rx, Tx

void setup()
{
  Serial.begin(115200);
  toUno.begin(9600);
}

void loop()
{
  toUno.println("From the ESP8266");
  toUno.write();
  delay(1000);
}
