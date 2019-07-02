// Sending Data Between Arduino and NodeMCU
// Arduino Code

// ************************* Headers and Libraries *************************

#include <SoftwareSerial.h>
SoftwareSerial s(5,6);    // (RX, TX) to receive and transmit
                          // Connect 5->D5 & 6->D6
                          // s represents the transfer comm
                          
//pin definitions

const int trigPin = 9;
const int echoPin = 10;

//define variables

long duration;
int distance;

// ******************* Setting conditions for connection ******************* 

void setup() 
{

  //set trig pin as out  
  pinMode (trigPin, OUTPUT); 
  
  //set echo pin as in
  pinMode (echoPin, INPUT);
  
  s.begin(115200);          // Receive and Trasmit
                            // The Higher the baud rate the more responsive
  Serial.begin(9600);       // Serial Monitor
}

void loop() 
{
    //clear trig pin
  
    digitalWrite (trigPin, LOW);
    delayMicroseconds (200);
    
    //set trig pin HIGH for 10us
    
    digitalWrite (trigPin, LOW);
    
    //set trig pin HIGH for 10us
    digitalWrite (trigPin, HIGH);
    delayMicroseconds (200);
    digitalWrite(trigPin, LOW);
    
    //read echoPin, return soundwave travel time in (us)
    duration = pulseIn (echoPin, HIGH);

    //calculate distance
    distance = duration*0.034/2;

    //prints distance on serial monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    s.write(distance);
    
  
  delay(1000);
}
