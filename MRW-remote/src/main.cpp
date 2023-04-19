#include <esp_now.h>
#include <WiFi.h>

#include <mac.h>

// Index 0 is send, 1 is up, 2 is down, 3 is left, 4 is right.
int inputs[5] = { 33, 32, 27, 26, 25 };
String inputValues[5] = { "enter", "up", "down", "left", "right" };
int inputLength = 1;
int latestInput = 0;

esp_now_peer_info_t peerInfo;

typedef struct remote_control_message {
    String input;
} remote_control_message;

typedef struct motor_message {
    int pulse;
} motor_message;

motor_message incomingMotorData;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&incomingMotorData, incomingData, sizeof(incomingMotorData));

    Serial.print("Bytes received: ");
    Serial.println(len);
}

void ESPNOWSetup() {
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(OnDataSent);

    // Register peer
    memcpy(peerInfo.peer_addr, peer_address, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }

    // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(OnDataRecv);
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

void sendInputValue(String s) {
    // Send input
    remote_control_message control_msg;
    control_msg.input = s;

    esp_err_t result = esp_now_send(peer_address, (uint8_t *) &control_msg, sizeof(control_msg));

    if (result == ESP_OK) {
        Serial.println("Sent with success");
    }
    else {
        Serial.println("Error sending the data");
    }
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
            Serial.print("Clicked ");
            Serial.print(i);
            return;
        }
    }
}

void setup() {
    Serial.begin(115200);
    inputSetup();
    ESPNOWSetup();
}

void loop() {
    inputRead();
    delay(100);
}