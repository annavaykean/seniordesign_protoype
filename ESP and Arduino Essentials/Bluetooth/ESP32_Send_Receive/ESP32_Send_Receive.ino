#include "BluetoothSerial.h"
//===== Preferences Manager:  (hack so that I can return a preference data type)
//                            see: https://playground.arduino.cc/Code/Struct#Creation 
#include "name.h"

BluetoothSerial SerialBT;
void setup() {
  //add:     PreferenceAdd(preferenceName,preferenceLabel,defaultValue,regexValidation (javascript regex),description);
  preferenceAdd("wifiname",      "WiFi SSID",     "Wireless-N",         "^[A-Z|a-z|-]{5}$",                              "WiFi SSID");
  preferenceAdd("wifipwd",       "WiFi Password", "xyzzy3585",          "^[A-Z|a-z|0-9]{10}$",                           "WiFi Password");
  preferenceAdd("mqttip",        "MQTT IP",       "192.168.1.250",      "^[0-9|.{15}$",                                  "MQTT Broker IP");
  preferenceAdd("nodename",      "MQTT Node",     "Node01",             "^[A-Z|a-z|0-9]{5}$",                            "Node name for MQTT");
  Serial.begin(115200);
  setupBT();
}
String receivedSerialString   = "";
void loop() {
  processBT();
}  
//==  BT interface for preferences/nvs ======================================================
//
//
String receivedSerialBTString = "";
void setupBT() {
  SerialBT.begin("ESP32test"); //Bluetooth device name
  SerialBT.flush();
  Serial.println("The device started, now you can pair it and connect from another bluetooth");
}
void processBT() {
  char receivedBTChar;
  while (SerialBT.available()) {
    receivedBTChar = SerialBT.read();
    receivedSerialBTString = receivedSerialBTString + receivedBTChar;
    if (receivedBTChar == '\n') {
      // ===== hello
      if (receivedSerialBTString.startsWith("hello")) {
          SerialBT.print("hello\n");  
          receivedSerialBTString = "";
          continue;
      }
      // ===== reboot
      if (receivedSerialBTString.startsWith("reboot")) {
          SerialBT.print("rebooting      - reconnect\n");
          //SerialBT.flush();
          //delay(1000);
          esp_restart();  
      }
      // ===== preferences get and update
      if (receivedSerialBTString.startsWith("preferences")) {
          // ===== preferences get
          if(receivedSerialBTString.substring(12).startsWith("get") ) {
            Serial.print("preferences/get");Serial.print(" senting: "); Serial.println(preferenceGetCount());
                            //                                                                                                    1         1         1         1
                            //0         1         2         3         4         5         6         7         8         9         0         1         2         1
                            //01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
                            //                   name           label         value              regex                                     description
                            //preferences gotten wifiname       WiFi SSID     Wireless-N         /.*/                                      WiFi SSID\n");
            for (int i = 0; i < preferenceGetCount(); i++) {
              SerialBT.print("preferences gotten "
                             + (preferenceGetInfo(i).preferenceName   + "                                             ").substring(0,15)
                             + (preferenceGetInfo(i).preferenceLabel  + "                                             ").substring(0,14)
                             + (preferenceGetInfo(i).actualValue      + "                                             ").substring(0,19)
                             + (preferenceGetInfo(i).regexValidation  + "                                             ").substring(0,41)
                             + (preferenceGetInfo(i).description      + "                                             ").substring(0,23)
                             + "\n");   
            }
            receivedSerialBTString = "";
            continue;
          }
          // ===== preferences update
          if(receivedSerialBTString.substring(12).startsWith("update") ) {
              //                                                                                                    1         1         1         1
              //0         1         2         3         4         5         6         7         8         9         0         1         2         1
              //01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
              //                   name           value              
              //preferences update wifiname       Wireless-N         
              receivedSerialBTString = receivedSerialBTString + "                    ";  // trailing spaces so substring(19,33) is in range  
              String nameString = receivedSerialBTString.substring(19,33);
              String valueString = receivedSerialBTString.substring(34);
              nameString.trim();  //because trim() modifies in-place see: https://www.arduino.cc/reference/en/language/variables/data-types/string/functions/trim/
              valueString.trim(); //because trim() modifies in-place see: https://www.arduino.cc/reference/en/language/variables/data-types/string/functions/trim/
              preferenceUpdate(nameString, valueString);
              receivedSerialBTString = "";
              continue;
          }
          // ===== defaults
          if(receivedSerialBTString.substring(15).startsWith("defaults") ) {
            preferenceReset();
            receivedSerialBTString = "";
            continue;
          }
      }   
      Serial.println("Unknown: " + receivedSerialBTString); 
      receivedSerialBTString = "";
      continue;
    }
  }
}
// ===== Preferences Manager" Access to preference Name/Values in NVS 
//       implements: Add (to list and NVS); Reset: (all values in list are set to defaults and NVS too); update: (change list value and NVS value)
//                   getCount and getPreference
//
#include <FS.h>
#include <Preferences.h>
Preferences preferences;
preferenceInfo preferenceList[20];
int preferenceCount = 0;
//============================================================================================
//===== Preferences Manager:  return one of the array 
//
preferenceInfo preferenceGetInfo(int index) {
  return preferenceList[index];
}
//============================================================================================
//===== Preferences Manager:  return the count of preferences in the array 
//
int preferenceGetCount() {
  return preferenceCount;
}
//============================================================================================
//===== Preferences Manager:  Adds a preference to the list and to the NVS "preferences" 
//
void preferenceAdd(String preferenceName,String preferenceLabel,String defaultValue,String regexValidation,String description) { // Add an item and put its value (or default) in table
           // Serial.print(preferenceGetInfo[0].description);
            preferenceList[preferenceCount].preferenceName  = preferenceName;
            preferenceList[preferenceCount].preferenceLabel  = preferenceLabel;
            preferenceList[preferenceCount].defaultValue    = defaultValue;    
            preferenceList[preferenceCount].regexValidation = regexValidation;
            preferenceList[preferenceCount].description     = description;
            preferences.begin("preferences",false);
            preferenceList[preferenceCount].actualValue = preferences.getString(preferenceName.c_str(), defaultValue.c_str());
            preferences.end();
            preferenceCount++;
          }
//============================================================================================
//===== Preference Manager: Sets all preferences in the list to their default (does not clear/change others in NVS but not in list)
//
void preferenceReset() {                                               //clear all and set the list of values to their default
              preferences.begin("preferences",false);
              for (int i = 0; i < preferenceCount; i++) {
                preferences.putString(preferenceList[i].preferenceName.c_str(), preferenceList[i].defaultValue.c_str());
                preferenceList[i].actualValue = preferences.getString(preferenceList[i].preferenceName.c_str(), preferenceList[i].defaultValue.c_str());
              }
              preferences.end(); 
              return;
}
//============================================================================================
//===== Preference Manager: Changes the preference value in NVS and sets the actualValue in the list
//
void preferenceUpdate(String preferenceName,String preferenceValue) {  // Change an items value and update the table 
          preferences.begin("preferences",false);
          for (int i = 0; i < preferenceCount; i++) {
              //Serial.println("compare:" + preferenceList[i].preferenceName + "/" + preferenceName + "/" +  preferenceValue);
              if (preferenceList[i].preferenceName == preferenceName) {
                //Serial.print("Changed:  "); Serial.print(preferenceName.c_str()); Serial.print("/"); Serial.println(preferenceValue.c_str());
                preferenceList[i].actualValue = preferenceValue;
                preferences.putString(preferenceName.c_str(),preferenceValue.c_str()); 
                //Serial.println("Verify:" +  preferences.getString(preferenceList[i].preferenceName.c_str(), "999"));
              }
          }
          preferences.end();
          return;
}
