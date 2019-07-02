// Sending Data Between Arduino and NodeMCU
// Arduino Code

#include <SoftwareSerial.h>       // Communication through Serial
SoftwareSerial ArduinoUno(3,2);   // (RX, TX) to receive and transmit
// Connect 3->D3 & 2->D2

void setup() 
{
  Serial.begin(9600);
  ArduinoUno.begin(4800);
}

void loop() 
{
  while (ArduinoUno.available()>0)
  {
    float val = ArduinoUno.parseFloat();
    if (ArduinoUno.read() == '\n')
    {
      Serial.println(val);
    }
  }
  delay(1000);
}
