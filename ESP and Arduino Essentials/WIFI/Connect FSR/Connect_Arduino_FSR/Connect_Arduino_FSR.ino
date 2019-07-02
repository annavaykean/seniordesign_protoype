// Sending Data Between Arduino and NodeMCU
// Arduino Code

#include <SoftwareSerial.h>
#include <ArduinoJson.h>  // JSON to connect Arduino and NodeMCU
SoftwareSerial s(5,6);    // (RX, TX) to receive and transmit
// Connect 5->D5 & 6->D6
 
//int fsrPin = 0;     // the FSR and 10K pulldown are connected to a0
//int fsrReading;     // the analog reading from the FSR resistor divider
 
void setup(void) 
{
  
  s.begin(9600);          // s represents pin connection

  // We'll send debugging information via the Serial monitor
//  Serial.begin(9600);     // Serial to communicate other device
}
 
void loop(void) 
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


//  fsrReading = analogRead(fsrPin);  
// 
//  Serial.print("Analog reading = ");
//  Serial.print(fsrReading);     // the raw analog reading
// 
//  // We'll have a few threshholds, qualitatively determined
//  if (fsrReading < 10) {
//    Serial.println(" - No pressure");
//  } else if (fsrReading < 200) {
//    Serial.println(" - Light touch");
//  } else if (fsrReading < 500) {
//    Serial.println(" - Light squeeze");
//  } else if (fsrReading < 800) {
//    Serial.println(" - Medium squeeze");
//  } else {
//    Serial.println(" - Big squeeze");
//  }
//  delay(1000);

} 
