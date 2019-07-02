// Sending Data Between Arduino and NodeMCU
// NodeMCU Code

#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);  // (RX, TX)
#include <ArduinoJson.h>
// Connect 5->D5 & 6->D6

void setup() 
{
  // Initialize Serial Port
  Serial.begin(9600);
  s.begin(9600);
  while (!Serial) continue;
}

void loop() 
{
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if ( root == JsonObject::invalid() )
    return;

  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  Serial.println("");
  Serial.print("\nData1 ");
  int data1=root["data1"];
  Serial.print(data1);
  Serial.print("\nData2 ");
  int data2=root["data2"];
  Serial.print(data2);
  Serial.println("");
  Serial.println("---------------xxxxx---------------\n");

  delay(1000);
}
