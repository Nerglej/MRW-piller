#include <Arduino.h>
#include <ArduinoBle

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define BLE_SERVER_NAME "MRW Remote 1"

#define SERVICE_UUID "6f9f35df-adc2-44e1-8c02-1dcb67d42551"
#define CHARACTERISTIC_UUID "a2779120-40d8-4920-8347-57655f37f23a"

void bleSetup() {
    Serial.println("Starting BLE server...");
    BLEDevice::init(BLE_SERVER_NAME);

    BLEServer *pServer = BLEDevice::createServer();
}

void setup() {
    Serial.begin(115200);
    bleSetup();
}

void loop() {
  
}