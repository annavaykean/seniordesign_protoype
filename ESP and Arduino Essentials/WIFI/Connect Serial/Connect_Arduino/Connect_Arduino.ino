// Sending Data Between Arduino and NodeMCU
// Arduino Code

#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(5,6);  // (RX, TX)
// Connect 5->D5 & 6->D6

void setup() 
{
  s.begin(9600);
}

void loop() 
{
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["data1"] = 100;
  root["data2"] = 200;

  if (s.available()>0)
  {
    root.printTo(s);
  }
  delay(1000);
}
