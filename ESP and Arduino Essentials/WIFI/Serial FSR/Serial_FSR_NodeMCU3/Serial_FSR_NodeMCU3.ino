// Sending Data Between Arduino and NodeMCU
// NodeMCU Code

// ************************* Headers and Libraries *************************

#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);    // (RX, TX) to receive and transmit
                            // Connect 5->D5 & 6->D6
                            // s represents the transfer comm
int cogX;             // Sensor data reading
int cogY;
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
//  s.write("s");
////  Serial.print("Analog reading = ");
////  Serial.println(fsrReading);
//
//  // Setting the s.read will reset values
////  fsrReading = s.read();
//
//  if (s.available())
//  {
//
//    // Show the Sensor readings
//    // Serial Monitor inside loop read values along with Arduino
//    Serial.print("Sensor Reading: ");
//    Serial.println(cogX);
//    // Setting the s.read inside does NOT reset values
//    cogX = s.read();
//  }
//  
//  delay(1000);

  byte n = s.available();
  if (n != 0)
  {
    char x = s.read();
    Serial.print(x);
  }

  // https://forum.arduino.cc/index.php?topic=605324.0
}
