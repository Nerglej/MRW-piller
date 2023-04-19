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

motor_message incomingMotorData;

// Callback når data bliver sendt. Udelukkende for fejlfinding.
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nStatus på seneste pakke:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Succes" : "Fejl på afsendelse");
}

// Opsætning af ESP-NOW
void ESPNOWSetup() {
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Kunne ikke initializere ESP-NOW");
        return;
    }

    esp_now_register_send_cb(OnDataSent);

    // Registrer den forbundede peer
    memcpy(peerInfo.peer_addr, peer_address, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    
    // Tilføj peer   
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Kunne ikke tilføje peer");
        return;
    }
}

// Opsætning af knap-input.
void inputSetup() {
    for (int i = 0; i < inputLength; i++) {
        pinMode(inputs[i], INPUT_PULLUP);
    }
}

// Læs en knap og se om den er klikket ned.
bool readButtonInput(uint8_t input) {
    if (digitalRead(input) == HIGH) {
        return false;
    }
    return true;
}

// Sender en besked til motoren om at en knap er blevet klikket ned.
void sendInputValue(String s) {
    // Et objekt der skal sendes
    remote_control_message control_msg;
    // Sætter objektets input til den string der skal sendes.
    control_msg.input = s;

    // Sender beskeden til motoren.
    esp_now_send(peer_address, (uint8_t *) &control_msg, sizeof(control_msg));
}

// Læser et input. Sørger for at den kun sender beskeden én gang per klik.
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

// Kører én gang når ESP'en starter.
void setup() {
    Serial.begin(115200);

    // Opsætning af input
    inputSetup();

    // Opsætning af ESP-NOW
    ESPNOWSetup();
}

// Kører hele tiden
void loop() {
    // Behandler inputtet.
    inputRead();
    delay(100);
}