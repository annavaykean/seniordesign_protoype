#include <ESP8266WiFi.h>                                                
#include <FirebaseArduino.h>                                             

#define FIREBASE_HOST   "sd-proto.firebaseio.com"                         
#define FIREBASE_AUTH   "JHX8oWlHBy0OV2NSvrnziX9hJ297ypRT5OAxuitY"                    
#define WIFI_SSID       "The Bomb Galaxy"                                         
#define WIFI_PASSWORD   "phuong3648"                                 

#include <SoftwareSerial.h>
SoftwareSerial toUno(D6, D5); //Rx, Tx

String fireStatus = ""; 
int vibrator = D3;

void setup()
{
  toUno.begin(9600);
  Serial.begin(9600);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);      
  pinMode(led, OUTPUT);  

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
  Firebase.setString("settings/0000/vibration", "1");  
}

void loop()
{
  fireStatus = Firebase.getString("settings/0000/vibration");
  toUno.write("toUno");

  if (fireStatus == "ON" || fireStatus == "on" || fireStatus == "1") 
  {                                                  // compare the input of led status received from firebase
    Serial.println("Led Turned ON");                         
    digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
    digitalWrite(led, HIGH);                                                         // make external led ON
  } 
  else if (fireStatus == "OFF" || fireStatus == "off" || fireStatus == "0") 
  {                                                  // compare the input of led status received from firebase
    Serial.println("Led Turned OFF");
    digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
    digitalWrite(led, LOW);                                                         // make external led OFF
  }
  else 
  {
    Serial.println("Wrong Credential! Please send ON/OFF");
  }
  
  toUno.println("From the ESP8266");
  
  delay(1000);
}
