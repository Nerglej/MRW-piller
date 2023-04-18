#include <Arduino.h>
#include <Stepper.h>

#include <esp_now.h>
#include <WiFi.h>
#include <mac.h>

#include <LiquidCrystal.h>

#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19

#define SOUND_PIN 23
#define FREQUENCY 2093

#define stepsPerRev 2038
#define stepSpeed 15

#define LCD_RS 21
#define LCD_ENABLE 22
#define LCD_D4 27
#define LCD_D5 26
#define LCD_D6 25
#define LCD_D7 32

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

Stepper stepper = Stepper(stepsPerRev, IN1, IN3, IN2, IN4);
const int seventh = stepsPerRev / 7;

esp_now_peer_info_t peerInfo;

typedef struct remote_control_message {
    String input;
} remote_control_message;

typedef struct motor_message {
    int pulse;
} motor_message;

remote_control_message remote_message;

bool clicking = false;
bool turn = false;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&remote_message, incomingData, sizeof(remote_message));

    if (remote_message.input == "released") {
        clicking = false;
    }
    else if (!clicking) {
        if (remote_message.input == "enter") {
            turn = true;
            clicking = true;
        }
    }
}

void LCDSetup() {
    Serial.print("Setting up LCD...");

    lcd.begin(16, 2);
    lcd.setCursor(3, 0);
    lcd.print("MRW-piller");
    lcd.blink();
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

void setup() {
    Serial.begin(115200);

    // Opsætning af LCD
    LCDSetup();
    // Opsætning af lyd-output
    pinMode(SOUND_PIN, OUTPUT);
    // Opsætning af ESPNOW
    ESPNOWSetup();

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

    delay(100);
}
