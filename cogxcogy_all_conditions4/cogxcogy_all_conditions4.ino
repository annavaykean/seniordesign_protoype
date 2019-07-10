#include <avr/interrupt.h>

#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial s(3,4);


//****************************

long count = millis();

//define variables

volatile int FR ;
volatile int FL ;
volatile int BR ;
volatile int BL ;

//small sensors

volatile int SF ;
volatile int SB ;
volatile int SL ;
volatile int SR ;

// x & y points
//measured values on chair if center is assumed to be zero--> will change once sensors permanently mounted



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

  s.begin(9600);
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

//pinMode(3,OUTPUT);
//pinMode(11,OUTPUT);
  }


 
//timer compare interrupt service routine
ISR(TIMER1_COMPA_vect){

 signed int x1= 5;
 signed int x2= -5.5;
 signed int x3= -2;
 signed int x4= 1.5;
 signed int x5= 0; 
 signed int x6= 0;
 signed int x7= 0; 
 signed int x8= 0;

signed int y1= 2.5;
signed int y2= 2.5;
signed int y3= -2.5;
signed int y4= -2.5;
signed int y5= 0;
signed int y6= 0; 
signed int y7= 0;
signed int y8= 0; 


  
    char toSendX[6];
    char toSendY[6];
    signed int temp;
    signed int number;
    signed int x;
    signed int y;
    
signed int cogx = ((x1*(FR)+x2*(FL)+x3*(BL)+x4*(BR))/(4));
signed int cogy = ((y1*(FR)+y2*(FL)+y3*(BL)+y4*(BR))/(4));



if (s.available())
  {
//    char inChar = s.read();
//    Serial.print(inChar);
        byte val = s.read();
       volatile int turnOnLED = val - 48;
    
    if (turnOnLED == 1)
    {
      Serial.println("LED Turned ON");
      //digitalWrite(LED_BUILTIN, LOW);     // make bultin led ON
      
    
     pinMode(11, OUTPUT);            // make external led ON
    }
  
    else if (turnOnLED == 0)
    {
      Serial.println("LED Turned OFF");
      //digitalWrite(LED_BUILTIN, HIGH);    // make bultin led OFF
      pinMode(11, INPUT);             // make external led OFF
    }
  
    else
    {
      Serial.println("Wrong Credential! Please send ON/OFF");
    }
   
  }

 //leaning forward 
  if(((FL)>=100)&&((FR)>=100)&&(count>=8854)){ 
    
  
   x = cogx;
   y = cogy;

digitalWrite(11,HIGH);
  
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
     // Serial.println();
 //     Serial.println(toSend);
     
    Serial.print("X: ");
  for(int i=0;i<=5;i++){
        //  delay(20000);
           Serial.print(toSendX[i]);
           //s.write(toSendX[i]);

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
     // Serial.println();
 //     Serial.println(toSend);
  
     Serial.print("Y: ");
  for(int i=0;i<=5;i++){
           Serial.print(toSendY[i]);
          
          // s.write(toSendY[i]);
    }
    Serial.println();
         //      delay(20000);

        count = 0;
  }
  
//
//   
//  //leaning backward
//
//  
//  else if(((BR)>=100) && ((BL)>=100)&& (count>=8854)){
// 
//   x = cogx;
//   y = cogy;
//
//  digitalWrite(3,LOW);
//  digitalWrite(4,LOW);
//
//  digitalWrite(3,HIGH);
//  digitalWrite(4,HIGH);
//  delay(1000);
//  digitalWrite(3,LOW);
//  digitalWrite(4,LOW);
//
//if(x<0){
//  toSendX[0] = '-';
//  toSendX[4] = ';';
//  toSendX[5] = 'x';
//  x *= -1;
//}
//
// else{
//    toSendX[0] = '0';
//    toSendX[4] = ';';
//    toSendX[5] = 'x';
//  }
//    for( int i=3 ; i>=1;i--){
//      toSendX[i]=(char)((x%10) + 48);
//      x = x/10;
//    }
//    
//s.flush();    //clear buffer
//     // Serial.println();
// //     Serial.println(toSend);
//     
//    Serial.print("X: ");
//  for(int i=0;i<=5;i++){
//          
//           Serial.print(toSendX[i]);
//           s.write(toSendX[i]);
//
//              }
//   Serial.println();
//
//
//
//
//
//if(y<0){
//  toSendY[0] = '-';
//  toSendY[4] = ';';
//  toSendY[5] = 'y';
//  y *= -1;
//}
//
// else{
//    toSendY[0] = '0';
//    toSendY[4] = ';';
//    toSendY[5] = 'y';
//  }
//    for( int i=3 ; i>=1;i--){
//      toSendY[i]=(char)((y%10) + 48);
//      y = y/10;
//    }
//    
//s.flush();    //clear buffer
//     // Serial.println();
// //     Serial.println(toSend);
//  
//     Serial.print("Y: ");
//  for(int i=0;i<=5;i++){
//           Serial.print(toSendY[i]);
//           s.write(toSendY[i]);
//    }
//    Serial.println();
//
//    count = 0;
//    }
//    
//    //leaning left
//      else if(((FL)>=100) && ((BL)>=100)&& (count>=8854)){
//  
//   
//   x = cogx;
//   y = cogy;
//
//  digitalWrite(3,LOW);
//  digitalWrite(4,LOW);
//
//  digitalWrite(3,HIGH);
//  digitalWrite(4,HIGH);
//  delay(100);
//  digitalWrite(3,LOW);
//  digitalWrite(4,LOW);
//  
//
//if(x<0){
//  toSendX[0] = '-';
//  toSendX[4] = ';';
//  toSendX[5] = 'x';
//  x *= -1;
//}
//
// else{
//    toSendX[0] = '0';
//    toSendX[4] = ';';
//    toSendX[5] = 'x';
//  }
//    for( int i=3 ; i>=1;i--){
//      toSendX[i]=(char)((x%10) + 48);
//      x = x/10;
//    }
//    
//s.flush();    //clear buffer
//     // Serial.println();
// //     Serial.println(toSend);
//     
//    Serial.print("X: ");
//  for(int i=0;i<=5;i++){
//        //  delay(20000);
//           Serial.print(toSendX[i]);
//           s.write(toSendX[i]);
//
//              }
//   Serial.println();
//
//
//
//
//
//if(y<0){
//  toSendY[0] = '-';
//  toSendY[4] = ';';
//  toSendY[5] = 'y';
//  y *= -1;
//}
//
// else{
//    toSendY[0] = '0';
//    toSendY[4] = ';';
//    toSendY[5] = 'y';
//  }
//    for( int i=3 ; i>=1;i--){
//      toSendY[i]=(char)((y%10) + 48);
//      y = y/10;
//    }
//    
//s.flush();    //clear buffer
//     // Serial.println();
// //     Serial.println(toSend);
//  
//     Serial.print("Y: ");
//  for(int i=0;i<=5;i++){
//           Serial.print(toSendY[i]);
//          
//           s.write(toSendY[i]);
//    }
//    Serial.println();
//         //      delay(20000);
//
//        count = 0;
//  }
//        
//    //leaning right
//    
//    else if(((BR)>=100) && ((FR)>=100) &&(count>=8854)){
//
//  
//   x = cogx;
//   y = cogy;
//
//  digitalWrite(3,LOW);
//  digitalWrite(4,LOW);
//
//  digitalWrite(3,HIGH);
//  digitalWrite(4,HIGH);
//  delay(100);
//  digitalWrite(3,LOW);
//  digitalWrite(4,LOW);
//
//if(x<0){
//  toSendX[0] = '-';
//  toSendX[4] = ';';
//  toSendX[5] = 'x';
//  x *= -1;
//}
//
// else{
//    toSendX[0] = '0';
//    toSendX[4] = ';';
//    toSendX[5] = 'x';
//  }
//    for( int i=3 ; i>=1;i--){
//      toSendX[i]=(char)((x%10) + 48);
//      x = x/10;
//    }
//    
//s.flush();    //clear buffer
//     // Serial.println();
// //     Serial.println(toSend);
//     
//    Serial.print("X: ");
//  for(int i=0;i<=5;i++){
//        //  delay(20000);
//           Serial.print(toSendX[i]);
//           s.write(toSendX[i]);
//
//              }
//   Serial.println();
//
//
//
//
//
//if(y<0){
//  toSendY[0] = '-';
//  toSendY[4] = ';';
//  toSendY[5] = 'y';
//  y *= -1;
//}
//
// else{
//    toSendY[0] = '0';
//    toSendY[4] = ';';
//    toSendY[5] = 'y';
//  }
//    for( int i=3 ; i>=1;i--){
//      toSendY[i]=(char)((y%10) + 48);
//      y = y/10;
//    }
//    
//s.flush();    //clear buffer
//     // Serial.println();
// //     Serial.println(toSend);
//  
//     Serial.print("Y: ");
//  for(int i=0;i<=5;i++){
//           Serial.print(toSendY[i]);
//          
//           s.write(toSendY[i]);
//    }
//    Serial.println();
//         
//        count = 0;
//  }
  
  
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

 pinMode(3,OUTPUT);
pinMode(11,OUTPUT);




//    s.read(turnOnLED);
  
//     End of transfer data statement
  
//  s.print(Serial.readString());
  







if((((FR)>=100)|((FL)>=100)|((BR)>=100)|((BL)>=100))){  //CHANGE TO SMALL SENSORS ?
    count ++;
  
   
  }
    else{
      count = 0;
      

 }

}
