#include <ESP8266WiFi.h>                                                
#include <FirebaseArduino.h>                                             

#define FIREBASE_HOST   "sd-proto.firebaseio.com"                         
#define FIREBASE_AUTH   "JHX8oWlHBy0OV2NSvrnziX9hJ297ypRT5OAxuitY"                    
#define WIFI_SSID       "The Bomb Galaxy"                                         
#define WIFI_PASSWORD   "phuong3648"                                 

#include <SoftwareSerial.h>
SoftwareSerial s(D6, D5); //RX, TX

String fireStatus = ""; 
int VIBRATOR = D3;

void setup()
{
  s.begin(9600);
  Serial.begin(9600);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);      
  pinMode(VIBRATOR, OUTPUT);  

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
//  Firebase.setString("settings/1212/vibration", "1");  
}

void loop()
{
  fireStatus = Firebase.getString("settings/1212/vibration");

  if (fireStatus == "1")
  {
    // compare the input of led status received from firebase
    Serial.print("VIBRATOR Turned ON\t");       
    Serial.println("Value: " + fireStatus);                  
    digitalWrite(LED_BUILTIN, LOW);           // make bultin led ON
    digitalWrite(VIBRATOR, HIGH);             // make external led ON
    s.write("1");
  } 
  
  else if (fireStatus == "0")
  {
    // compare the input of led status received from firebase
    Serial.print("VIBRATOR Turned OFF\t");
    Serial.println("Value: " + fireStatus);
    digitalWrite(LED_BUILTIN, HIGH);          // make bultin led OFF
    digitalWrite(VIBRATOR, LOW);              // make external led OFF
    s.write("0");
  }
  
  else 
  {
    Serial.println("Wrong Credential! Please send ON/OFF");
  }
  delay(1000);
}
