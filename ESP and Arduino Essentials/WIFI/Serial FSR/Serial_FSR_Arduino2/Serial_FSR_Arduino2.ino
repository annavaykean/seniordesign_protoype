// Sending Data Between Arduino and NodeMCU
// Arduino Code

// ************************* Headers and Libraries *************************

#include <avr/interrupt.h>
#include <SoftwareSerial.h>
SoftwareSerial s(5,6);    // (RX, TX) to receive and transmit
                          // Connect 5->D5 & 6->D6
                          // s represents the transfer comm
long count = millis();

int FR = A0;

// *********************** Pin connection of Sensors ***********************

//int fsrPin = 0;     // the FSR and 10K pulldown are connected to a0
//int fsrReading;     // the analog reading from the FSR resistor divider


// ******************* Setting conditions for connection ******************* 

void setup() 
{
  s.begin(115200);          // Receive and Trasmit
                            // The Higher the baud rate the more responsive
  Serial.begin(9600);       // Serial Monitor

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 62500;
  TCCR1B |= (1<<WGM12);
  TCCR1B |= (1<<CS12);
  TIMSK1 |= (1<<OCIE1A);

  interrupts(); 
}


ISR(TIMER1_COMPA_vect)
{
  if(analogRead(FR)>=50 && (count>=8884))
  {
    Serial.print("Leaning forward: ");
    Serial.print(analogRead(FR));
    //Serial.print(analogRead(FL));
    Serial.println();
    s.write(analogRead(FR));
  }
}
    

// ****************** Analog Readings from the FSR Sensor ****************** 

void loop() 
{
//  fsrReading = analogRead(FR);  
 
//  Serial.print("Analog reading = ");
//  Serial.print(fsrReading);     // the raw analog reading
  
  // Simple method
  if (s.available()>0)
  {
    if ( analogRead(FR) >= 50)
    {
      count ++;
    }
    else
    {
      count = 0;
    }

    // Writes sensor values
    //s.write( analogRead(FR) );
  }
  
//  delay(1000);
}
