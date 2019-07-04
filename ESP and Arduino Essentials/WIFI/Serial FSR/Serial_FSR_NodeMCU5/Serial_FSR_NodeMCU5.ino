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
  if (s.available() > 0)
  { 
    cogX = 0;
    cogY = 0;
    negative = false;
    for(int i=0;i<2;i++) 
    {
    signed int value = 0;
    Serial.println("START");
    //reset cogX cogY vals
    negative = false;
   
    //get char
    byte recieved = s.read();
    Serial.print("RECIEVED: ");
    Serial.println(recieved);
    while(recieved != 59)
    {
    if(recieved == 45)
    {
   //   Serial.println("NEGATIVE VALUE DETECTED");
      negative = true;
    }
    else if(recieved >= 48 && recieved <= 57)
    {
   //   Serial.println("PROCESSING INTEGER...");
      value *= 10;
      value += recieved - 48;
    }
    recieved = s.read();
    Serial.print("RECIEVED: ");
    Serial.println(recieved);
    }
    if(negative && recieved == 59)
    {
      value *= -1;
      negative = false;
    }

    if(cogX == 0){
      cogX = value;
      Serial.print("CogX: ");
      Serial.println(cogX);
    } else if(cogX != 0 && cogY == 0){
      cogY = value;
      Serial.print("CogY: ");
      Serial.println(cogY);
    } else {
      Serial.print("Error");
    }
    //reset
  }
  
  }

  //send cogX cogY to firebase
  
  delay(1000);
}
