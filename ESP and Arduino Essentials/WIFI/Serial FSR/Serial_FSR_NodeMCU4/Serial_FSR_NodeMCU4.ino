// Sending Data Between Arduino and NodeMCU
// NodeMCU Code

// ************************* Headers and Libraries *************************
#include <String.h>
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);    // (RX, TX) to receive and transmit
                            // Connect 5->D5 & 6->D6
                            // s represents the transfer comm
signed int cogX;            
signed int cogY;
boolean negative = false;
int value = 0;

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
  if (s.available() > 0)
  { 
    byte minus = s.read();
    if(minus == '-')
    {
      negative = true;
    }
    else if(minus >= '0' && minus <= '9')
    {
      value *= 10;
      value += minus - '0';
    }
    if(negative)
    {
      value *= -1;
      negative = false;
    }
    
    Serial.println(minus);
    
//    cogX = s.read();
//    cogY = s.read();
    
//    Serial.print("CogX: ");
//    Serial.print(cogX);
//    Serial.print("\t");
//    Serial.print("CogY: ");
//    Serial.print(cogY);
//    Serial.println();
  }
  
  delay(1000);
}