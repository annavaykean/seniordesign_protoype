// Sending Data Between Arduino and NodeMCU to Firebase
// NodeMCU to Firebase Code

// ************************* Headers and Libraries *************************

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

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
  }
  // Firebase will read as label
  Firebase.setInt("postureData/1212", fsrReading);
  
  delay(1000);
}
