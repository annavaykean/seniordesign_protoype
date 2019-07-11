// Sending Data Between Arduino and NodeMCU
// NodeMCU Code

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
signed int value = 0;
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
    //read value from input buffer
    byte recieved = s.read();
    
    //process value according to flags
    if(recieved == 45)
    {
      //read a negative sign
      negative = true;
    }
    
    else if(recieved >=48 && recieved <= 57)
    {
      //read a numerical value
      value *= 10;
      value += recieved - 48;
    }
     
    else if(recieved == 59)
    {
      if(negative)
      {
        value *= -1;
        negative = false; //reset flag
      }
    }
     
    else if(recieved == 120 || recieved == 121)
    {
      //read an x or y
      if(recieved == 120) 
      {
        cogX = value;
        value = 0;
      }
      if(recieved == 121)
      {
        cogY = value;
        value = 0;
      }
    }
    

    if(cogX != 0 && cogY != 0)
    {
      //print out coords
      Serial.println();
      Serial.print("(");
      Serial.print(cogX);
      Serial.print(", ");
      Serial.print(cogY);
      Serial.println(")");
    

      while(!timeClient.update()) 
      {
        timeClient.forceUpdate();
      }
        // 2018-05-28T16:00:13Z
        formattedDate = timeClient.getFormattedDate();
//       Serial.println(formattedDate);
        
      signed int x = cogX;
      cogX = 0;
      signed int y = cogY;
      cogY = 0;
      //send cogX cogY to firebase
      Firebase.setInt("postureData/1212/" + formattedDate + "/cogX", x);
      Firebase.setInt("postureData/1212/" + formattedDate + "/cogY", y);
      Firebase.setString("postureData/1212/" + formattedDate + "/created_at", formattedDate);

    }
  
  // delay(1000);
  }
  
}
