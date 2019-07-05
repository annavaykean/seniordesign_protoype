// Sending Data Between Arduino and NodeMCU
// Firebase Code

// ************************* Headers and Libraries *************************

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <String.h>
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);    // (RX, TX) to receive and transmit
                            // Connect 5->D5 & 6->D6
                            // s represents the transfer comm
signed int cogX;            
signed int cogY;
boolean negative = false;

// *********************** Connection to the Network *********************** 

#define FIREBASE_HOST   "sd-proto.firebaseio.com"
#define FIREBASE_AUTH   "JHX8oWlHBy0OV2NSvrnziX9hJ297ypRT5OAxuitY"
#define WIFI_SSID       "The Bomb Galaxy"
#define WIFI_PASSWORD   "phuong3648"
// Must connect to mobile hotpot on computer to use SSID and Password


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

// ******************* Setting conditions for connection ******************* 

void setup() 
{
  s.begin(115200);          // Receive and Trasmit
                            // The Higher the baud rate the more responsive
  Serial.begin(9600);       // Serial Monitor

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);


  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(-14400);
}

// ****************** Analog Readings coming from Arduino ****************** 

void loop() 
{
  if (s.available() > 0)
  { 
    cogX = 0;
    cogY = 0;
    negative = false;
    for(int i=0;i<2;i++) 
    {
    signed int value = 0;
    Serial.println("START");
    //reset cogX cogY vals
    negative = false;
   
    //get char
    byte recieved = s.read();
    Serial.print("RECIEVED: ");
    Serial.println(recieved);
    while(recieved != 59)
    {
    if(recieved == 45)
    {
   //   Serial.println("NEGATIVE VALUE DETECTED");
      negative = true;
    }
    else if(recieved >= 48 && recieved <= 57)
    {
   //   Serial.println("PROCESSING INTEGER...");
      value *= 10;
      value += recieved - 48;
    }
    recieved = s.read();
    Serial.print("RECIEVED: ");
    Serial.println(recieved);
    }
    if(negative && recieved == 59)
    {
      value *= -1;
      negative = false;
    }

    if(cogX == 0){
      cogX = value;
      Serial.print("CogX: ");
      Serial.println(cogX);
    } else if(cogX != 0 && cogY == 0){
      cogY = value;
      Serial.print("CogY: ");
      Serial.println(cogY);
    } else {
      Serial.print("Error");
    }
    //reset
  }

  while(!timeClient.update()) 
    {
      timeClient.forceUpdate();
    }
      // The formattedDate comes with the following format:
      // 2018-05-28T16:00:13Z
      // We need to extract date and time
      formattedDate = timeClient.getFormattedDate();
      Serial.println(formattedDate);
  }
  
  if(cogX != 0 && cogY != 0)
  {
    //send cogX cogY to firebase
    Firebase.setInt("postureData/1212/" + formattedDate + "/cogX", cogX);
    Firebase.setInt("postureData/1212/" + formattedDate + "/cogY", cogY);
    Firebase.setString("postureData/1212/" + formattedDate + "/created_at", formattedDate);
  }
  delay(1000);
}
