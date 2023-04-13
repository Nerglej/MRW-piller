#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>

#define BLE_SERVER_NAME "MRW Remote"

#define SERVICE_UUID "6f9f35df-adc2-44e1-8c02-1dcb67d42551"
#define CONTROL_CHARACTERISTIC_UUID "407adfd4-7909-4371-ab1f-362fdd9541ae"

BLECharacteristic *pControlCharacteristic;

// Index 0 is send, 1 is up, 2 is down, 3 is left, 4 is right.
int inputs[5] = { 33, 32, 27, 26, 25 };
std::__cxx11::string inputValues[5] = { "enter", "up", "down", "left", "right" };
int inputLength = 5;
int latestInput = 0;

void setupBLE() {
    Serial.println("Starter BLE server...");

    BLEDevice::init(BLE_SERVER_NAME);

    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);

    pControlCharacteristic = pService->createCharacteristic(
                                            CONTROL_CHARACTERISTIC_UUID,
                                            BLECharacteristic::PROPERTY_NOTIFY
                                        );
    pControlCharacteristic->setValue("");

    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();

    Serial.println("BLE serveren startede.");
}

void inputSetup() {
    for (int i = 0; i < inputLength; i++) {
        pinMode(inputs[i], INPUT_PULLUP);
    }
}

bool readButtonInput(uint8_t input) {
    if (digitalRead(input) == HIGH) {
        return false;
    }
    return true;
}

void sendInputValue(std::string s) {
    pControlCharacteristic->setValue(s);
    pControlCharacteristic->notify();
}

void inputRead() {
    if (latestInput != -1) {
        if (!readButtonInput(inputs[latestInput])) {
            latestInput = -1;
            sendInputValue("released");
        }
        return;
    }

    for (int i = 0; i < inputLength; i++) {
        if (readButtonInput(inputs[i])) {
            sendInputValue(inputValues[i]);
            latestInput = i;
            return;
        }
    }
}

void setup() {
    Serial.begin(115200);
    inputSetup();
    setupBLE();
}

void loop() {
    inputRead();
    delay(25);
}