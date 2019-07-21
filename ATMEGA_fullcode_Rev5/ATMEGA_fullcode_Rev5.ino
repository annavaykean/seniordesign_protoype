#include <avr/interrupt.h>
#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial s(51,52);  //these are isp pins that work on the pcb int 1 & 2

//****************************

long count = millis();  //counter for 10 second data transmit interval
long getUp = millis();  //counter for 30 second get up notification
//define variables
//Large Sensors
volatile int FR ;   //A0
volatile int FL ;   //A1
volatile int BR ;   //A2
volatile int BL ;   //A3

//Small Sensors
volatile int SF ;   //A4
volatile int SB ;   //A5
volatile int SL ;   //A6
volatile int SR ;   //A7

volatile unsigned int vibeSet;  //check variable for vibrate on/off
volatile int userUp = 0;  //userUp flag

/* x & y points 
measured values on chair if center is assumed to be zero--> will change once sensors permanently mounted
Sensor Scheme
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
  
 s.begin(57600); //baud rate cannot go above 57600 b/c pwm will not work for vibrate module this is for serial channels not serial comm

//initialize timer 1

noInterrupts();

TCCR1A = 0;
TCCR1B = 0;
TCNT1 = 0;

OCR1A = 62500;
TCCR1B |= (1<<WGM12); //CTC MODE
TCCR1B |= (1<<CS12);  //256 prescaler
TIMSK1 |= (1<<OCIE1A);  //enable timer compare interrupt

//enable timer 3
TCCR3A = 0;
TCCR3B = 0;
TCNT3 = 0;


OCR3A = 62500;
TCCR3B |= (1<<WGM32); //CTC MODE
TCCR3B |= (1<<CS32);  //256 prescaler
TIMSK3 |= (1<<OCIE3A);  //enable timer compare interrupt

interrupts(); //enable all interrupts
  
Serial.begin(9600);

  }
 
//timer compare interrupt service routine

ISR(TIMER1_COMPA_vect){

volatile signed int x1= 5;
volatile signed int x2= -5.5;
volatile signed int x3= -2;
volatile signed int x4= 1.5;
volatile signed int x6= -6.5; 
volatile signed int x8= 6.5;

volatile signed int y1= 2.5;
volatile signed int y2= 2.5;
volatile signed int y3= -2.5;
volatile signed int y4= -2.5;
volatile signed int y5= 6;
volatile signed int y7= -6; 

char toSendX[6];
char toSendY[6];
signed int temp;
signed int number;
signed int x;
signed int y;

signed int cogx = ((x1*(FR)+x2*(FL)+x3*(BL)+x4*(BR)+x6*(SL)+x8*(SR))/(6));
signed int cogy = ((y1*(FR)+y2*(FL)+y3*(BL)+y4*(BR)+y5*(SF)+y7*(SB))/(6));



//leaning forward
if(((FL)>=100)&&((FR)>=100)&&(count>=8854)){  //ADC check condition
     
 
//Serial.println(FR);
//Serial.println(FL);
//Serial.println(BR);
//Serial.println(BL);
//Serial.println(vibeSet);
Serial.print(count);
Serial.println(getUp);

   userUp = 0; 
   x = cogx;
   y = cogy;
   
   
    if(x<0){
  toSendX[0] = '-';
  toSendX[4] = ';';
  toSendX[5] = 'x';
  x *= -1;
           }

     else{
    toSendX[0] = '0';
    toSendX[4] = ';';
    toSendX[5] = 'x';
          }
    for( int i=3 ; i>=1;i--){
      toSendX[i]=(char)((x%10) + 48);
      x = x/10;
                             }
    
s.flush();    //clear buffer
         Serial.print("X: ");
  
    for(int i=0;i<=5;i++){
      Serial.print(toSendX[i]);
      s.write(toSendX[i]);
                          }
   Serial.println();

    if(y<0){
      toSendY[0] = '-';
      toSendY[4] = ';';
      toSendY[5] = 'y';
      y *= -1;
            }

    else{
      toSendY[0] = '0';
      toSendY[4] = ';';
      toSendY[5] = 'y';
        }
        
    for( int i=3 ; i>=1;i--){
      toSendY[i]=(char)((y%10) + 48);
      y = y/10;
                            }
    
s.flush();    //clear buffer
          Serial.print("Y: ");
  
    for(int i=0;i<=5;i++){
      Serial.print(toSendY[i]);
          s.write(toSendY[i]);
                         }
    Serial.println();
        count = 0;
}
  

//  //leaning backward
 else if(((BR)>=100) && ((BL)>=100)&& (count>=8854)){
     
 
//Serial.println(FR);
//Serial.println(FL);
//Serial.println(BR);
//Serial.println(BL);
//Serial.println(vibeSet);
Serial.print(count);
Serial.println(getUp);

   userUp = 0; 
   x = cogx;
   y = cogy;
   
   
    if(x<0){
  toSendX[0] = '-';
  toSendX[4] = ';';
  toSendX[5] = 'x';
  x *= -1;
           }

     else{
    toSendX[0] = '0';
    toSendX[4] = ';';
    toSendX[5] = 'x';
          }
    for( int i=3 ; i>=1;i--){
      toSendX[i]=(char)((x%10) + 48);
      x = x/10;
                             }
    
s.flush();    //clear buffer
         Serial.print("X: ");
  
    for(int i=0;i<=5;i++){
      Serial.print(toSendX[i]);
      s.write(toSendX[i]);
                          }
   Serial.println();

    if(y<0){
      toSendY[0] = '-';
      toSendY[4] = ';';
      toSendY[5] = 'y';
      y *= -1;
            }

    else{
      toSendY[0] = '0';
      toSendY[4] = ';';
      toSendY[5] = 'y';
        }
        
    for( int i=3 ; i>=1;i--){
      toSendY[i]=(char)((y%10) + 48);
      y = y/10;
                            }
    
s.flush();    //clear buffer
          Serial.print("Y: ");
  
    for(int i=0;i<=5;i++){
      Serial.print(toSendY[i]);
          s.write(toSendY[i]);
                         }
    Serial.println();
        count = 0;
 }

//leaning left

 else if(((FL)>=100) && ((BL)>=100)&& (count>=8854)){
  //Serial.println(FR);
  //Serial.println(FL);
  //Serial.println(BR);
  //Serial.println(BL);
  //Serial.println(vibeSet);
  Serial.println(count);
  Serial.println(getUp);
  userUp = 0; 
  x = cogx;
  y = cogy;
  
    if(vibeSet == 1){
      analogWrite(10,140);
                    }
                    
    else if(vibeSet == 0){
      analogWrite(10,0);
                         }
 
    if(x<0){
      toSendX[0] = '-';
      toSendX[4] = ';';
      toSendX[5] = 'x';
      x *= -1;
           }
           
    else{
      toSendX[0] = '0';
      toSendX[4] = ';';
      toSendX[5] = 'x';
        }
        
    for( int i=3 ; i>=1;i--){
      toSendX[i]=(char)((x%10) + 48);
      x = x/10;
                            }
s.flush();    //clear buffer
  
  Serial.print("X: ");
  
    for(int i=0;i<=5;i++){
        Serial.print(toSendX[i]);
        s.write(toSendX[i]);
                       }
    Serial.println();
  
    if(y<0){
      toSendY[0] = '-';
      toSendY[4] = ';';
      toSendY[5] = 'y';
      y *= -1;
            }

    else{
      toSendY[0] = '0';
      toSendY[4] = ';';
      toSendY[5] = 'y';
        }
        
    for( int i=3 ; i>=1;i--){
      toSendY[i]=(char)((y%10) + 48);
      y = y/10;
                            }
s.flush();    //clear buffer
  
  Serial.print("Y: ");
    
    for(int i=0;i<=5;i++){
      Serial.print(toSendY[i]);
      s.write(toSendY[i]);
                          }
    Serial.println();
    count = 0;
 }
  
//leaning right  
else if(((BR)>=100) && ((FR)>=100) &&(count>=8854)){
  //Serial.println(FR);
  //Serial.println(FL);
  //Serial.println(BR);
  //Serial.println(BL);
  //Serial.println(vibeSet);
  Serial.println(count);
  Serial.println(getUp);
    userUp = 0; 
    x = cogx;
    y = cogy;
    
     if(vibeSet == 1){
      analogWrite(11,140);
                      }
                      
     else if(vibeSet == 0){
      analogWrite(11,0);
                          }
 
     if(x<0){
      toSendX[0] = '-';
      toSendX[4] = ';';
      toSendX[5] = 'x';
      x *= -1;
             } 

    else{
      toSendX[0] = '0';
      toSendX[4] = ';';
      toSendX[5] = 'x';
        }
    
    for( int i=3 ; i>=1;i--){
      toSendX[i]=(char)((x%10) + 48);
      x = x/10;
                            }
    
s.flush();    //clear buffer
  
    Serial.print("X: ");
    
    for(int i=0;i<=5;i++){
      Serial.print(toSendX[i]);
      s.write(toSendX[i]);
                         }
                         
   Serial.println();

    if(y<0){
      toSendY[0] = '-';
      toSendY[4] = ';';
      toSendY[5] = 'y';
      y *= -1;
            }

    else{
      toSendY[0] = '0';
      toSendY[4] = ';';
      toSendY[5] = 'y';
         }
         
    for( int i=3 ; i>=1;i--){
      toSendY[i]=(char)((y%10) + 48);
      y = y/10;
                            }
    
s.flush();    //clear buffer
  
     Serial.print("Y: ");
  
    for(int i=0;i<=5;i++){
      Serial.print(toSendY[i]);
      s.write(toSendY[i]);
                         }
    Serial.println();
    count = 0;
}  

else if((((FR)<20)&&((FL)<20)&&((BR)<20)&&((BL)<20))){
  userUp = 1;
  s.write('*');
  userUp = 0;   
  s.flush();
}

}

    
 ISR(TIMER3_COMPA_vect){
  
  if((((FR)>=20)|((FL)>=20)|((BR)>=20)|((BL)>=20))&& (getUp >= 30000)){
    Serial.println(getUp);
    s.write('@');
    Serial.print("sent @");
    getUp = 0;
    count = 0;
    Serial.print(vibeSet);
    
    if(vibeSet == 1){
      analogWrite(10,150);
      analogWrite(11,150);
      s.flush();
                    }
                    
     else if(vibeSet == 0){       
      analogWrite(10,0);
      analogWrite(11,0);
                          }
            
  }
  
 }
 


 void loop() {
  //large sensors
  FR = analogRead(A0);
  FL = analogRead(A1);
  BR = analogRead(A2);
  BL = analogRead(A3);

  //small sensors
  SF = analogRead(A4);
  SB = analogRead(A5);
  SL = analogRead(A6);
  SR = analogRead(A7);

  
/*USE TO SEE IF SENSORS ARE OUTPUTTING AS EXPECTED

Serial.println(FR);
Serial.println(FL);
Serial.println(BL);
Serial.println(BR);
Serial.println(SF);
Serial.println(SB);

*/

  if (s.available()>0){    //check if serial is available
    byte val = s.read();    //read value from esp, store in val
    Serial.println("RECIEVE");  //confirm code is running
    volatile int turnOnLED = val - 48;   //offset by 48 to recieve 1 or 0
  
  if (turnOnLED == 1){
    Serial.println("LED Turned ON");
    pinMode(5, OUTPUT);            // make external led ON
    pinMode(6,OUTPUT);
    analogWrite(10,0);   //left vibrate
    analogWrite(11,0);   //right vibrate
    vibeSet = 1;
    
    s.flush();
                    }
  else if(turnOnLED == 0){
    Serial.println("LED Turned OFF");
    pinMode(10,INPUT);             // disable vibrate
    pinMode(11,INPUT);              //disable vibrate
    vibeSet = 0;
    s.flush();
                          }
  
   else{
    Serial.println("Wrong Credential! Please send ON/OFF");
       }

}

  if((((FR)>=20)|((FL)>=20)|((BR)>=20)|((BL)>=20))){  //CHANGE back to 100 from 10
    count ++;
    getUp++; 
                                                 }
  else{
    count = 0;
    getUp = 0;
  }

 }
