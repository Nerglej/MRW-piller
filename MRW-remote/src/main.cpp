#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>

#define BLE_SERVER_NAME "MRW Remote"

#define SERVICE_UUID "6f9f35df-adc2-44e1-8c02-1dcb67d42551"
#define CHARACTERISTIC_UUID "a2779120-40d8-4920-8347-57655f37f23a"

BLECharacteristic inputCharacteristic();
BLEDescriptor inputDescriptor(BLEUUID((uint16_t)0x2902));

void bleSetup() {
    Serial.println("Starting BLE server...");

    BLEDevice::init(BLE_SERVER_NAME);

    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                            CHARACTERISTIC_UUID,
                                            BLECharacteristic::PROPERTY_READ
                                        );

    pCharacteristic->setValue("start");

    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();

    Serial.println("Successfully started BLE server!");
}

void setup() {
    Serial.begin(115200);
    bleSetup();
}

void loop() {
  
}