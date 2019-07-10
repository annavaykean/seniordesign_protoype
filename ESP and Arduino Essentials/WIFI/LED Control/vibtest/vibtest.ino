int t = 2;

void setup() {
 pinMode( 7 , OUTPUT);  // Must be a PWM pin

}
void loop() {

  analogWrite( 7 , 150 );  // 60% duty cycle
  delay(250);              // play for 0.5s

 analogWrite( 7 , 0 );    // 0% duty cycle (off)
  delay(400);
  
}
