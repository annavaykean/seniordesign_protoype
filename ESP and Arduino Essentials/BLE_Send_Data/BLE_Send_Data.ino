// Libraries to include for BT device
// MAC Address CC:50:E3:AF:0C:A2
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
int txValue = 0;


// Univesally Unique Identifier
#define SERVICE_UUID            "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX  "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


// Connection of the Device
class MyServerCallbacks:  public BLEServerCallbacks
{
  void onConnect (BLEServer* pServer)
  {
    deviceConnected = true;
  };

  void onDisconnect (BLEServer* pServer)
  {
    deviceConnected = false;
  }
};


//boolean stop_it = false;


// Declaring the Device and Connection
// Device Server and Characteristic
void setup() 
{
  Serial.begin(9600);

  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks( new MyServerCallbacks() );

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                CHARACTERISTIC_UUID_TX,
                BLECharacteristic::PROPERTY_NOTIFY
                );

  // BLE2902 needed to notify
  pCharacteristic->addDescriptor( new BLE2902() );

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection to notify...");
}


//boolean stop_main_loop()
//{
//  if (stop_it == false)
//  {
//    if (deviceConnected)
//    {
//      txValue = random(0, 9);
//  
//      // Conversion of txValue
//      char txString[8];
//      // Convert float to string
//      dtostrf(txValue, 1, 2, txString);
//  
//      // Setting the value to the characteristic
//      pCharacteristic->setValue(txString);
//  
//      // Notifying the connected client
//      pCharacteristic->notify();
//      Serial.println("Sent Value: " + String(txString));
//      delay(500); 
//    }
//  }
//
//  stop_it = true;
//  return stop_it;
//}


// Neverending Loop of Values
void loop() 
{

//  stop_it = stop_main_loop();

  
  if (deviceConnected)
  {
    txValue = random(0, 9);

    // Conversion of txValue
    char txString[8];
    // Convert float to string
    dtostrf(txValue, 1, 2, txString);

    // Setting the value to the characteristic
    pCharacteristic->setValue(txString);

    // Notifying the connected client
    pCharacteristic->notify();
    Serial.println("Sent Value: " + String(txString));
    delay(500); 
  }
}
