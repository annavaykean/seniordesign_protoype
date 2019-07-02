
//pin definitions

const int trigPin = 9;
const int echoPin = 10;

//define variables

long duration;
int distance;

void setup() {

//set trig pin as out  
pinMode (trigPin, OUTPUT); 

//set echo pin as in
pinMode (echoPin, INPUT);

//start serial comm
Serial.begin(9600);

}

void loop() {
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

}
