#include <SoftwareSerial.h>
SoftwareSerial fromESP(5,6);

void setup() 
{
  Serial.begin(115200);
  fromESP.begin(9600);
}

void loop()
{
  if (fromESP.available())
  {
    char inChar = fromESP.read();
    Serial.print(inChar);
  }
}
