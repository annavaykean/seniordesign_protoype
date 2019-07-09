// Arduino Code for Receiving end

#include <SoftwareSerial.h>
SoftwareSerial s(5,6);

int LED = 11;   // Digital pin for LED trigger

void setup() 
{
  Serial.begin(9600);
  s.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(LED, OUTPUT);
}

void loop()
{
  if (s.available())
  {
//    char inChar = s.read();
//    Serial.print(inChar);
        byte val = s.read();
        int turnOnLED = val - 48;
    
    if (turnOnLED == 1)
    {
      Serial.println("LED Turned ON");
      digitalWrite(LED_BUILTIN, LOW);     // make bultin led ON
      digitalWrite(LED, HIGH);            // make external led ON
    }
  
    else if (turnOnLED == 0)
    {
      Serial.println("LED Turned OFF");
      digitalWrite(LED_BUILTIN, HIGH);    // make bultin led OFF
      digitalWrite(LED, LOW);             // make external led OFF
    }
  
    else
    {
      Serial.println("Wrong Credential! Please send ON/OFF");
    }


//    s.read(turnOnLED);
  }
  // End of transfer data statement
  
//  s.print(Serial.readString());
  
}
