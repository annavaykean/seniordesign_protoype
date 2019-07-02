#include <SoftwareSerial.h>
SoftwareSerial fromESP(5,6);

void setup() 
{
  Serial.begin(115200);
  fromESP.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  
  if (fromESP.available())
  {
    char inChar = fromESP.read();
    Serial.print(inChar);
  }
}
