#include <Arduino.h>
#include <Stepper.h>

#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLEScan.h>

#include <Wifi.h>
#include <WifiCreds.h>

#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19

#define stepsPerRev 2038
#define stepSpeed 15

Stepper stepper = Stepper(stepsPerRev, IN1, IN3, IN2, IN4);

const int half = stepsPerRev / 2;
const int seventh = stepsPerRev / 7;

#define BLE_SERVER_NAME "MRW Remote"

static BLEUUID serviceUUID("6f9f35df-adc2-44e1-8c02-1dcb67d42551");
static BLEUUID inputCharacteristicUUID("407adfd4-7909-4371-ab1f-362fdd9541ae");

static BLEAddress *pServerAddress;

static BLERemoteCharacteristic* inputCharacteristic;

bool connectFlag = false;
bool connected = false;
bool scan = false;
BLEAdvertisedDevice *connectedDevice;

void inputCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    //TODO: Write callbacks to all events.
    char* pure = (char*)pData;
    if (strlen(pure) > length) {
        pure[length] = '\0';
    }

    Serial.print(pure);
    Serial.print(" ");
    // stepper.step(half);
}

class ClientCallbacks : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
        connected = true;
    }

    void onDisconnect(BLEClient* pclient) {
        connected = false;
        connectFlag = true;
        Serial.println("onDisconnect");
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
            connectFlag = true;
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
    pBLEScan->start(3, false);
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

void connectWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to Wifi...");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }

    Serial.print(" Got ip: ");
    Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);

    setupBLE();
    // connectWifi();

    stepper.setSpeed(stepSpeed);
}

void loop() {
    if (connectFlag && !connected) {
        connectBLE(connectedDevice->getAddress());
        connectFlag = false;
    }

    delay(1000);
}
