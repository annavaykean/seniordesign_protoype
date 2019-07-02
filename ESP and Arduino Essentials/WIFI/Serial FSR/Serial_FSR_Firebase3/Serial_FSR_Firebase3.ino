// Sending Data Between Arduino and NodeMCU to Firebase
// NodeMCU to Firebase Code

// ************************* Headers and Libraries *************************

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
//#include <String.h>

#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);    // (RX, TX) to receive and transmit
                            // Connect 5->D5 & 6->D6
                            // s represents the transfer comm
int fsrReading;             // Sensor data reading

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
  // *************** Connection between Arduino and NodeMCU ***************
  s.begin(115200);        // Receive and Trasmit
                          // The Higher the baud rate the more responsive
  Serial.begin(9600);     // Serial Monitor
  // *************** Connection between Arduino and NodeMCU ***************

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
  timeClient.setTimeOffset(3600);
}

// ******************** Sending values to the Database ********************* 

void loop() 
{
  // *************** Sensor Values from Arduino and NodeMCU ***************
  s.write("s");

  if (s.available()>0)
  {  
    // Show the Sensor readings
    Serial.print("Sensor Reading: ");
    Serial.println(fsrReading);
    // Setting the s.read inside does NOT reset values
    fsrReading = s.read();


    while(!timeClient.update()) 
      {
        timeClient.forceUpdate();
      }
      // The formattedDate comes with the following format:
      // 2018-05-28T16:00:13Z
      // We need to extract date and time
      formattedDate = timeClient.getFormattedDate();
      Serial.println(formattedDate);
    
      // Extract date
//      int splitT = formattedDate.indexOf("T");
//      dayStamp = formattedDate.substring(0, splitT);
    //  Serial.print("DATE: ");
    //  Serial.println(dayStamp);
    //  // Extract time
    //  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
    //  Serial.print("HOUR: ");
    //  Serial.println(timeStamp);
  }
  if(fsrReading != 0) {
  Firebase.setInt("postureData/1212/" + formattedDate + "/cogX", fsrReading);
  Firebase.setInt("postureData/1212/" + formattedDate + "/cogY", 0);
  Firebase.setString("postureData/1212/" + formattedDate + "/created_at", formattedDate);
  }
  
  delay(1000);
}
