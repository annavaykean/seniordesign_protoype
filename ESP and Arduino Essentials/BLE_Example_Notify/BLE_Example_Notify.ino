// source video https://www.youtube.com/watch?v=oCMOYS71NIU

#include "BLE.h"
#include "BLEUtils.h"
#include "BLEServer.h"
#include <esp_log.h>
#include <string>
#include <sys/time.h>
#include <sstream>
#include "Task.h"
#include "BLE2902.h"

#include "sdkconfig.h"

static char LOG_TAG[] = "SampleNotify";

// See the following for generating UUIDs:
// hhtps://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5493e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;

class MyNotifyingTask: public Task
{
  void run (void *data)
  {
    uint8_t value = 0;
    while(1)
    {
      delay(2000);  // 2 seconds
      ESP_LOGD( LOG_TAG, "*** NOTIFY: %d ***", value);
      pCharacteristic->setValue(&value, 1);
      pCharacteristic->notify();
      value++;
    } // while 1
  }   // run
};    // MyNotifyTask

MyNotifyTask * pMyNotifyTask;


class MyServerCallbacks: public BLEServerCallbacks
{
  void onConnect    (BLEServer* pServer)
  {
    pMyNotifyTask->start();
  };

  void onDisconnect (BLEServer* pServer)
  {
    pMyNotifyTask->stop();
  }
};


static void run()
{
  pMyNotifyTask = new MyNotifyTask();
  pMyNotifyTask->setStackSize(8000);

  BLE::initServer("ESP Device");
  BLEServer *pServer = new BLEServer();
  pServer->setCallbacks( new MyServerCallbacks() );

  BLEServer *pService = pServer->createService( BLEUUID(SERVICE_UUID) );

  pCharacteristic = pService->createCharacteristic(
    BLEUUID(CHARACTERISTIC_UUID),
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
    );

  pCharacteristic->addDescriptor( new BLE2902() );

  // Start the device
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
}


void SampleNotify(void)
{
  
}
