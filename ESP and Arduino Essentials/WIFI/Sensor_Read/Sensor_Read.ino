float cf = 3.5; // calibration factor

int ffs1 = A0;  // FlexiForce sensor pin A0

int ffsdata = 0;
float Vout;

void setup() 
{
  Serial.begin(9600);
  pinMode(ffs1, INPUT);
}

void loop() 
{
  ffsdata = analogRead(ffs1);
  Vout = (ffsdata * 5.0) / 1023.0;
  Vout = Vout * cf;
  Serial.print("Flexi Force Sensor: ");
  Serial.print(Vout,3);
  Serial.println("");
  delay(1000);
}
