// Sending Data Between Arduino and NodeMCU
// NodeMCU Code

// ************************* Headers and Libraries *************************

#include <ESP8266WiFi.h>                                                
#include <FirebaseArduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>                                             

// *********************** Connection to the Network *********************** 

#define FIREBASE_HOST   "sd-proto.firebaseio.com"                         
#define FIREBASE_AUTH   "JHX8oWlHBy0OV2NSvrnziX9hJ297ypRT5OAxuitY"                    
#define WIFI_SSID       "Rudy"                                         
#define WIFI_PASSWORD   "pleasework"                                 

#include <SoftwareSerial.h>
SoftwareSerial s(D6, D5); //Rx, Tx

// *************************** Center of Gravity *************************** 

signed int cogX;            
signed int cogY;
signed int value = 0;
boolean negative = false;
// ********************* Vibration Avtication Commands ********************* 

String fireStatus = ""; 
//int VIBRATOR = D3;

// *************************** Timestamp for FSR *************************** 

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;


// ****************** Setting the Interrupt Statement ******************

#define my_delay 3      // Interrupt time in seconds
                        // Every 10 sec multiple (n x 10)
#define timer0_preload 80000000
                        // Arbitraty value, this is set to 80MHz
                        // to achieve 1 sec delay/timer time out
volatile int vib;
volatile int count;

void inline interruptVibrator();

//void inline interruptVibrator (void)
//{
//  if (count == 30000)
//  {
//    // Interrupt functions available on pin D0-D8
//    // Attached Interrupt does not work on D0/D18
//    vib = (vib == 1)? 0 : 1;
//    digitalWrite(LED_BUILTIN, vib);
//    timer0_write(ESP.getCycleCount() + timer0_preload * my_delay);
//    s.write('@');
//    Firebase.setString("settings/1212/getUp", "1");
//  
//    count = 0;
//  }
//
//}

// ******************* Setting conditions for connection ******************* 

void setup()
{
  s.begin(57600);
  // Uno can only work at baud rate 57600
  // Baud Rate at 115200 has less latency
  // 115200 does not activate vibrator on Uno
  Serial.begin(9600);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);      
//  pinMode(VIBRATOR, OUTPUT);

 
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

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(-14400);
  // Offset the timestamp by due to incorrect time reading
  // -14400 is by seconds, meaning 5 hours in seconds

   // Interrupt
  noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(interruptVibrator);
  timer0_write(ESP.getCycleCount() + timer0_preload * my_delay);
  interrupts();

}

void inline interruptVibrator(void)
{
  if (count == 30000)
  {
    // Interrupt functions available on pin D0-D8
    // Attached Interrupt does not work on D0/D18
    vib = (vib == 1)? 0 : 1;
    digitalWrite(LED_BUILTIN, vib);
    timer0_write(ESP.getCycleCount() + timer0_preload * my_delay);
    s.write('@');
    Firebase.setString("settings/1212/getUp", "1");
  
    count = 0;
  }

}


// ****************** Analog Readings coming from Arduino ******************

void loop()
{
  count = 0;
  // ****************** FSR Reading Data ******************
  if (s.available() > 0)
  { 
    count++;
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
    
    else if(recieved == '*')
    {
      //bad data sent, dismiss current value
      value = 0;
      cogX = 0;
      cogY = 0;
      Serial.println("User got up");
    }
    
    // Gather the data from Sensor to print values
    if((cogX != 0 && cogY != 0) && (cogX < 500 && cogX > -500 && cogY < 500 && cogY > -500))
    {
      //print out coords
      Serial.println();
      Serial.print("(");
      Serial.print(cogX);
      Serial.print(", ");
      Serial.print(cogY);
      Serial.println(")");
    
      // Timestamp of the sensor data
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
//   delay(1000);


//    // ****************** Vibration Module Activation ******************
//    // ******************* Inside the FSR Statement *******************
//    fireStatus = Firebase.getString("settings/1212/vibration");
//    
//    if (fireStatus == "1")
//    {
//      // compare the input of led status received from firebase
//      Serial.print("VIBRATOR Turned ON\t");       
//      Serial.println("Value: " + fireStatus);                  
//      digitalWrite(LED_BUILTIN, LOW);           // make bultin led ON
////      digitalWrite(VIBRATOR, HIGH);             // make external led ON
//      s.write("1");
//    } 
//    
//    else if (fireStatus == "0")
//    {
//      // compare the input of led status received from firebase
//      Serial.print("VIBRATOR Turned OFF\t");
//      Serial.println("Value: " + fireStatus);
//      digitalWrite(LED_BUILTIN, HIGH);          // make bultin led OFF
////      digitalWrite(VIBRATOR, LOW);              // make external led OFF
//      s.write("0");
//    }
//    
//    else 
//    {
//      Serial.println("Wrong Credential! Please send ON/OFF");
//    }
//    delay(500);
    
  }
  // The end of the s.available


  // ****************** Vibration Module Activation ******************
  // ******************* Outside the FSR Statement *******************
  fireStatus = Firebase.getString("settings/1212/vibration");
    
  if (fireStatus == "1")
  {
    // compare the input of led status received from firebase
    Serial.print("VIBRATOR Turned ON\t");       
    Serial.println("Value: " + fireStatus);                  
    digitalWrite(LED_BUILTIN, LOW);           // make bultin led ON
//    digitalWrite(VIBRATOR, HIGH);             // make external led ON
    s.write("1");
  } 
    
  else if (fireStatus == "0")
  {
    // compare the input of led status received from firebase
    Serial.print("VIBRATOR Turned OFF\t");
    Serial.println("Value: " + fireStatus);
    digitalWrite(LED_BUILTIN, HIGH);          // make bultin led OFF
//    digitalWrite(VIBRATOR, LOW);              // make external led OFF
    s.write("0");
  }
    
  else 
  {
    Serial.println("Wrong Credential! Please send ON/OFF");
  }
   
  delay(500);
  

  // Conclusion of the code print statement into serial communication
  // Once sensor code starts sending data, vibration statement starts
  // Proper baud rate is important to match the readings from Arduino
  
}
