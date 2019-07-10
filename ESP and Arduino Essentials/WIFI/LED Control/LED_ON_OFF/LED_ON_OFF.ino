#include <ESP8266WiFi.h>          // esp8266 library
#include <FirebaseArduino.h>      // firebase library

#define FIREBASE_HOST   "sd-proto.firebaseio.com"                  // the project name address from firebase id
#define FIREBASE_AUTH   "JHX8oWlHBy0OV2NSvrnziX9hJ297ypRT5OAxuitY"      // the secret key generated from firebase
#define WIFI_SSID       "The Bomb Galaxy"                               // input your home or public wifi name 
#define WIFI_PASSWORD   "phuong3648"                                    //password of wifi ssid

String fireStatus = "";       // led status received from firebase
int led = D3;                 // for external led

void setup() 
{
  Serial.begin(9600);
  delay(1000);

  // Output of the LED
  pinMode(LED_BUILTIN, OUTPUT);      
  pinMode(led, OUTPUT);                 

  // WiFi configuration
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
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
  Serial.println(WiFi.localIP());                       //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);         // connect to firebase
  Firebase.setString("settings/1212/vibration", "1");   //send initial string of led status
}

void loop() 
{
  fireStatus = Firebase.getString("settings/1212/vibration");   // get ld status input from firebase
  
  if (fireStatus == "ON" || fireStatus == "on" || fireStatus == "1") 
  {                                                          
    // compare the input of led status received from firebase
    Serial.println("Led Turned ON");                         
    digitalWrite(LED_BUILTIN, LOW);     // make bultin led ON
    digitalWrite(led, HIGH);            // make external led ON
  } 
  else if (fireStatus == "OFF" || fireStatus == "off" || fireStatus == "0") 
  {                                                  
    // compare the input of led status received from firebase
    Serial.println("Led Turned OFF");
    digitalWrite(LED_BUILTIN, HIGH);    // make bultin led OFF
    digitalWrite(led, LOW);             // make external led OFF
  }
  else 
  {
    Serial.println("Wrong Credential! Please send ON/OFF");
  }

  delay(1000);
}
