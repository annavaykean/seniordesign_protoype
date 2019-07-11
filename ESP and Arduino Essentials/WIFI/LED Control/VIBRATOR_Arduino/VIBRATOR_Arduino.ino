// Arduino Code for Receiving end

#include <SoftwareSerial.h>
SoftwareSerial s(5,6);

int VIBRATOR = 11;   // Digital pin for VIBRATOR to trigger

void setup() 
{
  Serial.begin(9600);
  s.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);         // Turn on LED
  pinMode(VIBRATOR, OUTPUT);            // Activate the Vibration Module
}

void loop()
{
  //Serial.println("LOOP");
  if (s.available())
  {
    Serial.println("START");
//    char inChar = s.read();
//    Serial.print(inChar);
        byte val = s.read();
        int turnOnVibrator = val - 48;
        Serial.println(val);
    
    if (turnOnVibrator == 1)
    {
      Serial.println("LED Turned ON");
      digitalWrite(LED_BUILTIN, LOW);     // make bultin led ON
//      digitalWrite(LED, HIGH);            // make external led ON
      analogWrite(VIBRATOR, 150);         // 60% duty cycle (ON)
//      delay(250);                         // play for 0.5s
    }
  
    else if (turnOnVibrator == 0)
    {
      Serial.println("LED Turned OFF");
      digitalWrite(LED_BUILTIN, HIGH);    // make bultin led OFF
//      digitalWrite(LED, LOW);             // make external led OFF
      analogWrite(VIBRATOR, 0);         // 0% duty cycle (OFF)
//      delay(400);                       // play for 0.5s
    }
  
    else
    {
      Serial.println("Wrong Credential! Please send ON/OFF");
    }

  }
  
}
