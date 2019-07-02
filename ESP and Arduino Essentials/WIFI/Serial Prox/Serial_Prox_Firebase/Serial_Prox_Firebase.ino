#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "nodemcu-efc0a.firebaseio.com"
#define FIREBASE_AUTH "qmlxD6fh7ZhaVC2pt3v1Qou6kFz8bVgdPXQMbfYa"
#define WIFI_SSID     "SuperSaiyan"
#define WIFI_PASSWORD "SuperGoku"

#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);    // (RX, TX) to receive and transmit
                            // Connect 5->D5 & 6->D6
                            // s represents the transfer comm
int distanceReading;        // Sensor data reading


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
}

void loop() 
{
  s.write("s");

  if (s.available())
  {
    // Show the Sensor readings
    // Serial Monitor inside loop read values along with Arduino
    Serial.print("Distance Reading: ");
    Serial.println(distanceReading);
    // Setting the s.read inside does NOT reset values
    distanceReading = s.read();
  }

  // Send Distance Values to Firebase
  Firebase.setInt("distance",distanceReading);
  delay(1000);
}
