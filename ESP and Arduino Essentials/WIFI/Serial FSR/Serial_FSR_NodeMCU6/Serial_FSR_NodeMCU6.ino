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
  s.begin(9600);          // Receive and Trasmit
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
  if (s.available() > 6)
  { 
    cogX = 0;
    cogY = 0;
    negative = false;
    for(int i=0;i<2;i++) 
    {
      signed int value = 0;
      //reset cogX cogY vals
      negative = false;
     
      //get char
      byte recieved = s.read();
      
      //check for bad Ascii
//      if(recieved < 45 || recieved > 121)
//      {
//        Serial.print("Error : " + recieved);
//        
//      }else {
//        Serial.print("RECIEVED1: ");
//        if(recieved >= 48 && recieved <= 57){
//          Serial.println(recieved - 48);
//        }         
//        else if(recieved == 45) {
//          Serial.println('-');
//          }
//          else if(recieved == 59) {
//            Serial.println(';');
//            }
//            else if(recieved == 'x' or 'y'){
//              Serial.println(recieved == 'x'? 'x' : 'y');
//            }
//          else {
//        Serial.println(recieved);
//        }     
//        }
      
      //process char
      while(recieved != 'x' && recieved != 'y')
      {
        //check if value is negative
        if(recieved == 45)
        {
          negative = true;
        }
        //check for valid numerical value
        else if(recieved >= 48 && recieved <= 57)
        {
          value *= 10;
          value += recieved - 48;
        }
        //if null terminator ';', process negative flag
         else if(negative && recieved == 59)
        {
          value *= -1;
          negative = false;
        }
        else if(recieved == 59){
          //do nothing
        }
        else {
          Serial.println("Invalid");
        }
        recieved = s.read();
              
      //check for bad Ascii
//      if(recieved < 45  || recieved > 121)
//      {
//        Serial.println("Error: " + recieved);
//      } else {
//        Serial.print("RECIEVED2: ");
//        if(recieved >= 48 && recieved <= 57){
//          Serial.println(recieved - 48);
//        }
//        else if(recieved == 45) {
//          Serial.println('-');
//          }
//          else if(recieved == 59) {
//            Serial.println(';');
//            }
//            else if(recieved == 'x' or 'y'){
//              Serial.println(recieved == 'x'? 'x' : 'y');
//            }
//          else {
//        Serial.println(recieved);
//        }
//      }
      
      }
      //if 'x' recieved, save value to cogX variable
      if(recieved == 120)
      {
        //assign to cogX
        cogX = value;
        value = 0;
      }
      //if 'y' recieved, save value to cogY variable
      else if(recieved == 121)
      {
        //assign to cogY
        cogY = value;
        value = 0;
      }
      
    }
    // End of for loop

    
//    s.write("test");
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
      // The formattedDate comes with the following format:
      // 2018-05-28T16:00:13Z
      // We need to extract date and time
      
      formattedDate = timeClient.getFormattedDate();
      Serial.println(formattedDate);
      

  }
  // End of if statement for transfer data

  
  if(cogX != 0 && cogY != 0)
  {
    signed int x = cogX;
    signed int y = cogY;
    //send cogX cogY to firebase
    Firebase.setInt("postureData/1212/" + formattedDate + "/cogX", x);
    Firebase.setInt("postureData/1212/" + formattedDate + "/cogY", y);
    Firebase.setString("postureData/1212/" + formattedDate + "/created_at", formattedDate);
  }
  
  delay(1000);
}
