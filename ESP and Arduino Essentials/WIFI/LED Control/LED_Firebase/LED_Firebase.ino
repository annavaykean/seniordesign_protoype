#include <ESP8266WiFi.h>                                                
#include <FirebaseArduino.h>                                             

#define FIREBASE_HOST   "nodemcu-efc0a.firebaseio.com"                         
#define FIREBASE_AUTH   "qmlxD6fh7ZhaVC2pt3v1Qou6kFz8bVgdPXQMbfYa"                    
#define WIFI_SSID       "The Bomb Galaxy"                                         
#define WIFI_PASSWORD   "phuong3648"                                 

#include <SoftwareSerial.h>
SoftwareSerial toUno(D6, D5); //Rx, Tx

String fireStatus = ""; 
int led = D3;

void setup()
{
  toUno.begin(9600);
  Serial.begin(9600);
//  delay(1000);
//  pinMode(LED_BUILTIN, OUTPUT);      
//  pinMode(led, OUTPUT);  

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting to ");
  Serial.print(WIFI_SSID);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());    
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("settings/1212/vibration", "1");  
}

void loop()
{
  fireStatus = Firebase.getString("settings/1212/vibration");
//  toUno.write("toUno");

  if (fireStatus == "1")
  {
    // compare the input of led status received from firebase
    Serial.println("Led Turned ON");                         
//    digitalWrite(LED_BUILTIN, LOW);           // make bultin led ON
//    digitalWrite(led, HIGH);                  // make external led ON
//    toUno.write(fireStatus);
  } 
  else if (fireStatus == "0")
  {
    // compare the input of led status received from firebase
    Serial.println("Led Turned OFF");
//    digitalWrite(LED_BUILTIN, HIGH);          // make bultin led OFF
//    digitalWrite(led, LOW);                   // make external led OFF
//    toUno.write(fireStatus);
  }
  else 
  {
    Serial.println("Wrong Credential! Please send ON/OFF");
  }
  
//  toUno.println("From the ESP8266");
  toUno.write(fireStatus);
  
  delay(1000);
}
