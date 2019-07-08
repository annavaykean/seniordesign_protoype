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
	byte recieved;
    for(int i=0;i<2;i++) 
    {
	cogX = 0;
	cogY = 0;
	negative = false;
	signed int value = 0;
	for(int j = 0;j<7;j++){
		
      //get initial +/- char
	  recieved = null;
      recieved = s.read();
      Serial.print("RECIEVED: ");
      Serial.println(recieved);
      
      //check for bad Ascii
      if(recieved == 255){
			Serial.print("Error1");
			break;
		}
	  else if(recieved == '-')
		{
		  negative = true;
		}
		//check for valid numerical value
		else if(recieved >= '0' && recieved <= '9')
		{
		  value *= 10;
		  value += recieved - '0';
		}
		//if null terminator ';', process negative flag
		 else if(negative && recieved == ';')
		{
		  value *= -1;
		  negative = false;
		}
	}
	recieved = s.read();	 
	//if 'x' recieved, save value to cogX variable
      if(recieved == 'x'){
        //assign to cogX
        cogX = value;
      }
      //if 'y' recieved, save value to cogY variable
      else if(recieved == 'y'){
        //assign to cogY
        cogY = value;
      }
    }
    //print out coords to console
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
      // Format: 2018-05-28T16:00:13Z
      formattedDate = timeClient.getFormattedDate();
      Serial.println(formattedDate);
      
  if(cogX != 0 && cogY != 0)
  {
    //send cogX cogY to firebase
    Firebase.setInt("postureData/1212/" + formattedDate + "/cogX", cogX);
    Firebase.setInt("postureData/1212/" + formattedDate + "/cogY", cogY);
    Firebase.setString("postureData/1212/" + formattedDate + "/created_at", formattedDate);
  }
  
  delay(1000);
  
  }
  

}
