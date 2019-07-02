// Sending Data Between Arduino and NodeMCU
// NodeMCU Code

// ************************* Headers and Libraries *************************

#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);    // (RX, TX) to receive and transmit
                            // Connect 5->D5 & 6->D6
                            // s represents the transfer comm
int distanceReading;             // Sensor data reading
//long count = millis();

// ******************* Setting conditions for connection ******************* 

void setup() 
{
  s.begin(115200);          // Receive and Trasmit
                            // The Higher the baud rate the more responsive
  Serial.begin(9600);       // Serial Monitor
}

// ****************** Analog Readings coming from Arduino ****************** 

void loop() 
{
  s.write("s");

  if (s.available()>0)
  {
    // Show the Sensor readings
    // Serial Monitor inside loop read values along with Arduino
    Serial.print("Distance Reading: ");
    Serial.println(distanceReading);
    // Setting the s.read inside does NOT reset values
    distanceReading = s.read();
  }
  
  delay(1000);
}
