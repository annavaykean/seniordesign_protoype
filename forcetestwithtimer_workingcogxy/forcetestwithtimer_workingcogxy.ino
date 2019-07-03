#include <avr/interrupt.h>

#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial s(5,6);


//****************************

long count = millis();

//define variables


int FR ;
int FL ;
int BR ;
int BL ;

//small sensors

int SF ;
int SB ;
int SL ;
int SR ;

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


int cogy = ((y1*(FR)+y2*(FL)+y3*(BL)+y4*(BR))/(y1+y2+y3+y4));

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

  s.begin(115200);
  // put your setup code here, to run once:

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

 

Serial.begin(9600);


}

//timer compare interrupt service routine
ISR(TIMER1_COMPA_vect){
  

  

char x[]  = {'-', '1', '0','\0'};

signed int cogy = ((y1*(FR)+y2*(FL)+y3*(BL)+y4*(BR))/(4));

 //leaning forward 
  if(((FL)>=100)&&((FR)>=100)&&(count>=8854)){ //mod to work with one sensor put back FR for chair testing
    //Serial.print("Leaning Forward: ");
    //Serial.print((FR));
   // Serial.print("\t");
   // Serial.println((FL));
   // Serial.println(toSend);
    //Serial.print(cogy);
    //s.write(toSend);
   // s.write(cogy);
    
   }
  //leaning backward
  else if(((BR)>=150) && ((BL)>=150)&& (count>=8885)){
    Serial.print("Leaning Backward: ");
    Serial.print((BR));
    Serial.print("\t");
    Serial.print((BL));
    Serial.println();

 


    }
    //leaning left
      else if(((FL)>=150) && ((BL)>=150)&& (count>=8885)){
    Serial.print("Leaning Left: ");
    Serial.print((FL));
    Serial.print("\t");
    Serial.print((BL));
    Serial.println();

        }
    //leaning right
    else if(((BR)>=150) && ((FR)>=150) &&(count>=8885)){
    Serial.print("Leaning Right: ");
    Serial.print((FR));
    Serial.print("\t");
    Serial.print((BR));
    Serial.println();
  
      }
  }

  
void loop() {
 FR = analogRead(A0);
 FL = analogRead(A1);
 BR = analogRead(A2);
 BL = analogRead(A3);

//small sensors

 SF = analogRead(A4);
 SB = analogRead(A5);
 SL = analogRead(A6);
 SR = analogRead(A7);

//this code is for sending negative values as characters -->needs to be written as a function
//*********************************************************************************************************************************************************************************
signed int cogx = -136;
//((x1*(FR)+x2*(FL)+x3*(BL)+x4*(BR))/(4));

char toSend[5];
signed int temp = cogx;

if(cogx<0){
//Serial.print("HIT");
  toSend[0] = '-';
  toSend[4] = ';';
  temp *= -1;
}
 else{
    toSend[0] = '0';

  }
    for( int i=3 ; i>=1;i--){
      //Serial.print("HIT");
      toSend[i]=(char)((temp%10) + 48);
      temp = temp/10;
      //Serial.print("At index " + i);
    }
    for(int i=0;i<5;i++){
            Serial.println(toSend[i]);
            s.write(toSend[i]);
    }
s.flush();
     // Serial.println();

 //     Serial.println(toSend);
 //     s.write(toSend);
      delay(5000);

     if((((FR)>=100)|((FL)>=50)|((BR)>=100)|((BL)>=100))){  //CHANGE TO SMALL SENSORS ?
    count ++;

   
  }
    else{
      count = 0;
  
    }
//*******************************************************************************************************************************************************************************8

 /*   
  Serial.print(analogRead(FL));
    Serial.print("\t");
    Serial.print(analogRead(BL));
    Serial.print("\t");
    Serial.print(analogRead(FR));
    Serial.print("\t");
    Serial.print(analogRead(BR));
   Serial.println();

   Serial.print(cogx);
Serial.print("\t");
Serial.print(cogy);
Serial.println();
   */

}
