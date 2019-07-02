// Sending Data Between Arduino and NodeMCU
// NodeMCU Code

// ************************* Headers and Libraries *************************

#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);    // (RX, TX) to receive and transmit
                            // Connect 5->D5 & 6->D6
                            // s represents the transfer comm
int fsrReading;             // Sensor data reading

// ******************* Setting conditions for connection ******************* 

void setup() 
{
  s.begin(115200);        // Receive and Trasmit
  Serial.begin(9600);     // Serial Monitor
}

// ****************** Analog Readings coming from Arduino ****************** 

void loop() 
{
  s.write("s");
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);

  if (s.available()>0)
  {
    // We'll have a few threshholds, qualitatively determined
//    if (fsrReading < 10) 
//    { Serial.println(" - No pressure"); } 
//      
//    else if (fsrReading < 200) 
//    { Serial.println(" - Light touch"); } 
//      
//    else if (fsrReading < 500) 
//    { Serial.println(" - Light squeeze"); } 
//      
//    else if (fsrReading < 800) 
//    { Serial.println(" - Medium squeeze"); } 
//      
//    else 
//    { Serial.println(" - Big squeeze"); }

    // Show the Sensor readings
    fsrReading = s.read();
  }
  
  delay(1000);
}
