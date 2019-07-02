/*
 * Posted on https://randomnerdtutorials.com
 * created by http://playground.arduino.cc/Code/NewPing
*/

#include <SoftwareSerial.h>
SoftwareSerial s(5,6);    // (RX, TX) to receive and transmit
                          // Connect 5->D5 & 6->D6
                          // s represents the transfer comm
                          
#include <NewPing.h>
 
#define TRIGGER_PIN 9
#define ECHO_PIN 10
#define MAX_DISTANCE 2000

// NewPing setup of pins and maximum distance
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
 
void setup() 
{
  s.begin(115200);          // Receive and Trasmit
                            // The Higher the baud rate the more responsive
  Serial.begin(9600);       // Serial Monitor
}
 
void loop() 
{
//   delay(50);
   unsigned int distance = sonar.ping_cm();
   Serial.print("Distance: ");
   Serial.println(distance);
   s.write(distance);
   delay(1000);
}
