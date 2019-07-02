#include <SoftwareSerial.h>
SoftwareSerial toUno(D6, D5); //Rx, Tx

void setup()
{
  toUno.begin(9600);
}

void loop()
{
  toUno.println("From the ESP8266");
  delay(2000);
}
