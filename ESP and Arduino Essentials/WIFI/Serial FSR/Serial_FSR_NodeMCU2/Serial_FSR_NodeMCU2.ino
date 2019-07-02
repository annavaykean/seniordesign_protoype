// Sending Data Between Arduino and NodeMCU
// NodeMCU Code

// ************************* Headers and Libraries *************************

#include <avr/interrupt.h>
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);    // (RX, TX) to receive and transmit
                            // Connect 5->D5 & 6->D6
                            // s represents the transfer comm
int FR;             // Sensor data reading
long count = millis();


// ******************* Setting conditions for connection ******************* 

void setup() 
{
  s.begin(9600);        // Receive and Trasmit
  Serial.begin(9600);   // Serial Monitor

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
  if(FR>=100 && (count>=8884))
  {
    Serial.print("Leaning forward: ");
    Serial.print(FR);
    //Serial.print(analogRead(FL));
    Serial.println();
    s.write(FR);
  }
}


// ****************** Analog Readings coming from Arduino ****************** 

void loop() 
{
  s.write("s");
//  Serial.print("Analog reading = ");
//  Serial.println(fsrReading);

  if (s.available()>0)
  {
    if ( FR >= 100)
    {
      count ++;
    }
    else
    {
      count = 0;
    }
    FR = s.read();
  }
  
//  delay(1000);
}
