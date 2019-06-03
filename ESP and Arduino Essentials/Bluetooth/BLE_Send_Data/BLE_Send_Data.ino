// Libraries to include for BT device
// MAC Address CC:50:E3:AF:0C:A2 for green dot
// MAC Address CC:50:E3:AB:41:32 for purple dot
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
int txValue = 0;

// Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE" 
// Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

// Univesally Unique Identifier
#define SERVICE_UUID            "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
//#define CHARACTERISTIC_UUID_RX  "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
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

  // Create a BLE Characteristic to transmit
  pCharacteristic = pService->createCharacteristic(
                CHARACTERISTIC_UUID_TX,
                BLECharacteristic::PROPERTY_NOTIFY 

                //CHARACTERISTIC_UUID_RX,
                //BLECharacteristic::PROPERTY_WRITE
                );

  // BLE2902 needed to notify
  pCharacteristic->addDescriptor( new BLE2902() );

//  // Create a BLE Characteristic to receive
//  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
//                                         CHARACTERISTIC_UUID_RX,
//                                         BLECharacteristic::PROPERTY_WRITE
//                                       );
//
//  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection to notify...");
}


// Neverending Loop of Values
void loop() 
{
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
