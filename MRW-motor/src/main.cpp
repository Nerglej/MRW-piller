#include <Arduino.h>
#include <Stepper.h>

#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLEScan.h>

#include <LiquidCrystal.h>

#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19

#define SOUND_PIN 23
#define FREQUENCY 300

#define stepsPerRev 2038
#define stepSpeed 15

Stepper stepper = Stepper(stepsPerRev, IN1, IN3, IN2, IN4);

const int seventh = stepsPerRev / 7;

#define LCD_RS 21
#define LCD_ENABLE 22
#define LCD_D4 27
#define LCD_D5 26
#define LCD_D6 25
#define LCD_D7 32
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

#define BLE_SERVER_NAME "MRW Remote"

static BLEUUID serviceUUID("6f9f35df-adc2-44e1-8c02-1dcb67d42551");
static BLEUUID inputCharacteristicUUID("407adfd4-7909-4371-ab1f-362fdd9541ae");

static BLEAddress *pServerAddress;

static BLERemoteCharacteristic* inputCharacteristic;

bool bleConnectFlag = false;
bool bleConnected = false;
bool bleScan = false;
BLEAdvertisedDevice *connectedDevice;

bool clicking = false;
bool turn = false;

void inputCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    //TODO: Write callbacks to all events.
    char* pure = (char*)pData;
    if (strlen(pure) > length) {
        pure[length] = '\0';
    }

    if (strcmp(pure, "released") == 0) {
        clicking = false;
    }
    else if (clicking == false) {
        if (strcmp(pure, "enter") == 0) {
            clicking = true;
            turn = true;
        }
    }
}

class ClientCallbacks : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
        bleConnected = true;
    }

    void onDisconnect(BLEClient* pclient) {
        bleConnected = false;
        bleConnectFlag = true;
    }
};

class AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        if (!advertisedDevice.haveServiceUUID()) {
            return;
        }

        Serial.print("BLE Advertised Device with service UUID has been found: ");
        Serial.println(advertisedDevice.toString().c_str());
        
        if (advertisedDevice.isAdvertisingService(serviceUUID)) {
            BLEDevice::getScan()->stop();
            connectedDevice = new BLEAdvertisedDevice(advertisedDevice);
            bleConnectFlag = true;
            Serial.println("Found valid BLE server.");
        }
    }
};

void setupBLE() {
    BLEDevice::init("");

    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(3, true);
}

bool connectBLE(BLEAddress pAddress) {
    BLEClient* pClient = BLEDevice::createClient();

    pClient->setClientCallbacks(new ClientCallbacks());

    pClient->connect(pAddress);
    Serial.println("Connected to server.");

    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
        Serial.print("Failed to find service with the given UUID: ");
        Serial.println(serviceUUID.toString().c_str());
        pClient->disconnect();

        return false;
    }

    inputCharacteristic = pRemoteService->getCharacteristic(inputCharacteristicUUID);

    if (inputCharacteristic == nullptr) {
        Serial.print("Failed to find characteristic with the given UUID: ");
        Serial.println(inputCharacteristicUUID.toString().c_str());
        pClient->disconnect();

        return false;
    }

    inputCharacteristic->registerForNotify(inputCallback);

    Serial.println("Successfully got all characteristics.");
    return true;
}

void setupLCD() {
    Serial.print("Setting up LCD...");

    lcd.begin(16, 2);
    lcd.setCursor(3, 0);
    lcd.print("MRW-piller");
    lcd.blink();
}

void setup() {
    Serial.begin(115200);

    setupLCD();
    setupBLE();

    pinMode(SOUND_PIN, OUTPUT);

    stepper.setSpeed(stepSpeed);
}

void loop() {
    if (turn) {
        lcd.setCursor(1, 1);
        lcd.write("Pilling you...");

        tone(SOUND_PIN, FREQUENCY, 0);
        
        stepper.step(seventh);

        noTone(SOUND_PIN);

        lcd.setCursor(1, 1);
        lcd.write("              ");
        turn = false;
    }

    if (bleConnectFlag && !bleConnected) {
        connectBLE(connectedDevice->getAddress());
        bleConnectFlag = false;
    }

    delay(100);
}
