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
    Serial.print("received: ");
    Serial.println(minus);
    if(minus == 45)
    {
      negative = true;
    }
    else if(minus >= 48 && minus <= 57)
    {
      value *= 10;
      value += minus - 48;
    }
    else if(negative && minus == 59)
    {
      value *= -1;
      negative = false;
      Serial.println(value);
      value = 0;
    }
      
    
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
