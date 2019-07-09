// Arduino Code for Receiving end

#include <SoftwareSerial.h>
SoftwareSerial fromESP(5,6);

String turnOnLED = "";
int LED = 11;   // Digital pin for LED trigger

void setup() 
{
  Serial.begin(115200);
  fromESP.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(LED, OUTPUT);
}

void loop()
{
  // Loop to turn on LED from Arduino Blink Example
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(1000);
//  digitalWrite(LED_BUILTIN, LOW);
//  delay(1000);

  if (fromESP.available())
  {
    if (turnOnLED == "1")
    {
      Serial.println("LED Turned ON");
      digitalWrite(LED_BUILTIN, LOW);     // make bultin led ON
      digitalWrite(LED, HIGH);            // make external led ON
    }
  
    else if (turnOnLED == "0")
    {
      Serial.println("LED Turned OFF");
      digitalWrite(LED_BUILTIN, HIGH);    // make bultin led OFF
      digitalWrite(LED, LOW);             // make external led OFF
    }
  
    else
    {
      Serial.println("Wrong Credential! Please send ON/OFF");
    }
    
  }
//  End of transfer data statement
  
  fromESP.print(Serial.readString());
  
  
  
  
//  if (fromESP.available())
//  {
//    char inChar = fromESP.read();
//    Serial.print(inChar);
//  }

//  delay(1000);

}
