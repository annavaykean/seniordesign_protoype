#include <Ticker.h>

Ticker testTick;

volatile int WatchDogCount = 0;

void ISRWatchDog()
{
  WatchDogCount++;
  Serial.print("ISRWatchDog function executed...>>");
  Serial.printf("Counter's Value increased to ");
  Serial.println(WatchDogCount);
}

void setup() 
{
  Serial.begin(115200);
  testTick.attach(1,ISRWatchDog);
}

void loop() 
{
  Serial.println("\n\nVoid LOOP is executing");
  delay(5000);
}
