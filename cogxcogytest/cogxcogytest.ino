#include <avr/interrupt.h>


//****************************

long count = millis();

//fetch from database for vibrate module
int t = 1;

//define variables

long duration;
int distance;
//large sensors

int FR = analogRead(A0);
int FL = analogRead(A1);
int BR = analogRead(A2);
int BL = analogRead(A3);

//small sensors

int SF = analogRead(A4);
int SB = analogRead(A5);
int SL = analogRead(A6);
int SR = analogRead(A7);

// x & y points
//measured values on chair if center is assumed to be zero--> will change once sensors permanently mounted

 int x1= 5;
 int x2= -5;
 int x3= -5;
 int x4= 5;
 int x5= 0; 
 int x6= -5;
 int x7= 0; 
 int x8= 5;

int y1= 5;
int y2= 5;
int y3= -5;
int y4= -5;
int y5= 5;
int y6= 0; 
int y7= -5;
int y8= 0; 

int cogx  = (((x1*(FR))+(x2*(FL))+(x3*(BL))+(x4*(BR)))/((FR)+(FL)+(BL)+(BR)));
int cogy = ((y1*(FR)+y2*(FL)+y3*(BL)+y4*(BR))/((FR)+(FL)+(BL)+(BR)));

int cogxnumer = ((x1*(FR))+(x2*(FL))+(x3*(BL))+(x4*(BR)));
int cogxdenom = ((FR)+(FL)+(BL)+(BR));


/*Sensor Scheme
 1-FR,+x +y
 2-FL,-x +y
 3-BL,-x -y
 4-BR,+x -y
 5-SF,0 +y
 6-SL,-x 0
 7-SB,0 -y
 8-SR,+x 0
*/

//****************************

void setup() {
  // put your setup code here, to run once:

//enable vibrate module on pin 7

 pinMode( 7 , OUTPUT);  // Must be a PWM pin

//initialize timer 1

noInterrupts();
TCCR1A = 0;
TCCR1B = 0;
TCNT1 = 0;

OCR1A = 62500;
TCCR1B |= (1<<WGM12); //CTC MODE
TCCR1B |= (1<<CS12);  //256 prescaler
TIMSK1 |= (1<<OCIE1A);  //enable timer compare interrupt

interrupts(); //enable all interrupts


}

//timer compare interrupt service routine
ISR(TIMER1_COMPA_vect){
 
 //leaning forward 
  if(((FR)>=55) && ((FL)>=55) && (count>=8884)){
    Serial.print("Leaning Forward: ");
    Serial.print((FR));
    Serial.print("\t");
    Serial.print((FL));
    Serial.println();
   
    Serial.print("cogx: ");
    Serial.print(cogx);
    Serial.print("\t");
    Serial.print("cogy: ");
    Serial.print(cogy);
    Serial.println();
//s.write cog values and disable other serial print outputs 
  }
  //leaning backward
  else if(((BR)>=55) && ((BL)>=55)&& (count>=8884)){
    Serial.print("Leaning Backward: ");
    Serial.print((BR));
    Serial.print("\t");
    Serial.print((BL));
    Serial.println();

    Serial.print("cogx: ");
    Serial.print(cogx);
    Serial.print("\t");
    Serial.print("cogy: ");
    Serial.print(cogy);
    Serial.println();
    }
    //leaning left
      else if(((FL)>=55) && ((BL)>=55)&& (count>=8884)){
    Serial.print("Leaning Left: ");
    Serial.print((FL));
    Serial.print("\t");
    Serial.print((BL));
    Serial.println();
    
    Serial.print("cogx: ");
    Serial.print(cogx);
    Serial.print("\t");
    Serial.print("cogy: ");
    Serial.print(cogy);
    Serial.println();
        }
    //leaning right
    else if(((BR)>=55) && ((FR)>=55) &&(count>=8884)){
    Serial.print("Leaning Right: ");
    Serial.print((FR));
    Serial.print("\t");
    Serial.print((BR));
    Serial.println();

    Serial.print("cogx: ");
    Serial.print(cogx);
    Serial.print("\t");
    Serial.print("cogy: ");
    Serial.print(cogy);
    Serial.println();
    }
  }

 
    void loop() {
  
Serial.print(cogxnumer);
Serial.print("\t");
Serial.print(cogxdenom);
Serial.println();

Serial.print(x4*(BR));
Serial.print("\t");
Serial.println();
delay(1000);

  if((((FR)>=55)|((FL)>=55))|((BR)>=55)|((BL)>=55)){
    count ++;
  }
    else{
      count = 0;
    }
    
//code below is for vibrate modules
   /* if(t==1){
  analogWrite( 7 , 150 );  // 60% duty cycle
  delay(250);              // play for 0.5s

 analogWrite( 7 , 0 );    // 0% duty cycle (off)
  delay(400);
  
  analogWrite( 7 , 150 );  // 60% duty cycle
  delay(250);     
  
  analogWrite( 7 , 0 );    // 0% duty cycle (off)
  delay(4000);             // wait for 4s
  */
}
