// Sending Data Between Arduino and NodeMCU
// Arduino Code

// ************************* Headers and Libraries *************************

#include <avr/interrupt.h>
#include <SoftwareSerial.h>
SoftwareSerial s(5,6);    // (RX, TX) to receive and transmit
                          // Connect 5->D5 & 6->D6
                          // s represents the transfer comm
long count = millis();

//fetch from database for vibrate module
int t = 1;

//define variables

long duration;
int distance;
//large sensors

int FR = A0;
int FL = A1;
int BR = A2;
int BL = A3;

//small sensors

int SF = A4;
int SB = A5;
int SL = A6;
int SR = A7;

// x & y points
//measured values on chair if center is assumed to be zero--> will change once sensors permanently mounted

 int x1= -5;
 int x2= 5.5;
 int x3= -2;
 int x4= 1.5;
 int x5= 0; 
 int x6= 0;
 int x7= 0; 
 int x8= 0;

int y1= 2;
int y2= 2;
int y3= -1.5;
int y4= -1.5;
int y5= 0;
int y6= 0; 
int y7= 0;
int y8= 0; 

int cogX; 
int cogY;

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
  cogX = ((x2*analogRead(FR)+x1*analogRead(FL)+x3*analogRead(BL)+x4*analogRead(BR))/(4));
  
  if(analogRead(FR)>=50 && (count>=8884))
  {
    Serial.print("Leaning forward: ");
    Serial.print(analogRead(FR));
    Serial.print("\t");
    //Serial.print(analogRead(FL));
    Serial.print("\t");

    Serial.print(cogX);
    Serial.print("\t");
    Serial.println(); 
    
    s.write(analogRead(FR));
//    s.write(cogX);
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
