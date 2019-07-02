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

#define FIREBASE_HOST   "nodemcu-efc0a.firebaseio.com"
#define FIREBASE_AUTH   "qmlxD6fh7ZhaVC2pt3v1Qou6kFz8bVgdPXQMbfYa"
#define WIFI_SSID       "The Bomb Galaxy"
#define WIFI_PASSWORD   "phuong3648"
// Must connect to mobile hotpot on computer to use SSID and Password

// ******************* Setting conditions for connection ******************* 

void setup() 
{
  // *************** Connection between Arduino and Node ***************
  s.begin(9600);        // Receive and Trasmit
  Serial.begin(9600);   // Serial Monitor
  // *************** Connection between Arduino and Node ***************

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

//  Firebase.set("LED_STATUS", 0);
  Firebase.setInt("fsrReading", 0);
}

// ******************** Sending values to the Database ********************* 

//int n = 0;

void loop() 
{
  // *************** Sensor Values from Arduino and NodeMCU ***************
  s.write("s");
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);

  if (s.available()>0)
  {
    // We'll have a few threshholds, qualitatively determined
    if (fsrReading < 10) 
    { Serial.println(" - No pressure"); }  
         
    else if (fsrReading < 200) 
    { Serial.println(" - Light touch"); }  
         
    else if (fsrReading < 500) 
    { Serial.println(" - Light squeeze"); }  
        
    else if (fsrReading < 800) 
    { Serial.println(" - Medium squeeze"); } 
         
    else 
    { Serial.println(" - Big squeeze"); }
    
    // Show the Sensor readings
    fsrReading = s.read();
  }
  
  Firebase.setInt("Sensor Reading: ", fsrReading);
  
  delay(100);
  // *************** Sensor Values from Arduino and NodeMCU ***************


  // ******************** Testing other Sensor Example ********************
//  Serial.print("Sensor Reading: ");   // Display values on Arduino IDE
//  Serial.println("fsrReading");       // Display values on Arduino IDE
//  Firebase.setInt("Sensor Reading: ", fsrReading);
  // ******************** Testing other Sensor Example ********************


  // ******************** Firebase Connection Example ********************
//  // set value
//  Firebase.setInt("number", fsrReading);
//  // handle error
//  if ( Firebase.failed() ) 
//  {
//      Serial.print("setting /number failed:");
//      Serial.println(Firebase.error());  
//      return;
//  }
//  delay(1000);
//  
//  // update value
//  Firebase.setFloat("number", 43.0);
//  // handle error
//  if ( Firebase.failed() ) 
//  {
//      Serial.print("setting /number failed:");
//      Serial.println(Firebase.error());  
//      return;
//  }
//  delay(1000);
//
//  // get value 
//  Serial.print("Sensor Value: ");
//  Serial.println(Firebase.getFloat("number"));
//  delay(1000);
//
//  // remove value
//  Firebase.remove("number");
//  delay(1000);
//
//  // set string value
//  Firebase.setString("message", "Database Connected");
//  // handle error
//  if ( Firebase.failed() ) 
//  {
//      Serial.print("setting /message failed:");
//      Serial.println(Firebase.error());  
//      return;
//  }
//  delay(1000);
//  
//  // set bool value
//  Firebase.setBool("truth", false);
//  // handle error
//  if ( Firebase.failed() ) 
//  {
//      Serial.print("setting /truth failed:");
//      Serial.println(Firebase.error());  
//      return;
//  }
//  delay(1000);
//
//  // append a new value to /logs
//  String name = Firebase.pushInt("logs", n++);
//  // handle error
//  if ( Firebase.failed() ) 
//  {
//      Serial.print("pushing /logs failed:");
//      Serial.println(Firebase.error());  
//      return;
//  }
//  Serial.print("pushed: /logs/");
//  Serial.println(name);
//  delay(1000);
  // ******************** Firebase Connection Example ********************
  
}
