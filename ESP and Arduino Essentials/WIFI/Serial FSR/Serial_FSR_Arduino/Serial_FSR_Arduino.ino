// Sending Data Between Arduino and NodeMCU
// Arduino Code

// ************************* Headers and Libraries *************************

#include <SoftwareSerial.h>
SoftwareSerial s(5,6);    // (RX, TX) to receive and transmit
                          // Connect 5->D5 & 6->D6
                          // s represents the transfer comm

// *********************** Pin connection of Sensors ***********************

int fsrPin = 0;     // the FSR and 10K pulldown are connected to a0
int fsrReading;     // the analog reading from the FSR resistor divider


// ******************* Setting conditions for connection ******************* 

void setup() 
{
  s.begin(9600);        // Receive and Trasmit
  Serial.begin(9600);   // Serial Monitor
}

// ****************** Analog Readings from the FSR Sensor ****************** 

void loop() 
{
  fsrReading = analogRead(fsrPin);  
 
  Serial.print("Analog reading = ");
  Serial.print(fsrReading);     // the raw analog reading
  
  // Simple method
  if (s.available()>0)
  {
    // We'll have a few threshholds, qualitatively determined
    if (fsrReading < 10) 
    { Serial.println(" - No pressure"); } 
      
    else if (fsrReading < 200) 
    { Serial.println(" - Light touch"); } 
      
    else if (fsrReading < 500) 
    { Serial.println(" - Light squeeze"); } 
      
    else if (fsrReading < 800) 
    { Serial.println(" - Medium squeeze"); } 
      
    else 
    { Serial.println(" - Big squeeze"); }

    // Writes sensor values
    s.write(fsrReading);
  }
  
  delay(1000);
}
