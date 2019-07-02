// Sending Data Between Arduino and NodeMCU
// NodeMCU Code

#include <SoftwareSerial.h>       // Communication through Serial
#include <ESP8266WiFi.h>
SoftwareSerial NodeMCU(D2,D3);    // (RX, TX) to receive and transmit
// Connect 3->D3 & 2->D2

void setup() 
{
  Serial.begin(9600);
  NodeMCU.begin(4800);
  pinMode(D2,INPUT);
  pinMode(D3,OUTPUT);
}

void loop() 
{
  int i = 10;
  NodeMCU.print(i);
  NodeMCU.println("\n");
  delay(1000);
}
