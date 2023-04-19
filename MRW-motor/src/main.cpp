#include <Stepper.h>

#include <esp_now.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <settings.h>

#include <LiquidCrystal.h>

#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19

#define stepsPerRev 2038
#define stepSpeed 15

#define WIFI_RETRIES 10

#define SOUND_PIN 23
#define SOUND_CHANNEL 0
#define SOUND_FREQUENCY 2093
#define SOUND_RES 12

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

remote_control_message remote_message;

bool clicking = false;
bool turn = false;

// Callback når data bliver modtaget
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    // Kopiér data der bliver modtaget, til en lokal variabel, så vi kan behandle det som forventet.
    memcpy(&remote_message, incomingData, sizeof(remote_message));

    // Behandl dataene, så det faktisk kan bruges i koden.
    // Hvis den modtagene input er "released", skal den ikke gøre andet noget.
    // Hvis den derimod er alt andet, og der ikke bliver holdt en knap nede,
    // skal den gøre forskellige ting, som at meddele at motoren skal dreje næste loop.
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

// Opsætning af LCD
void LCDSetup() {
    Serial.println("Setting up LCD...");

    // Beskriv LCD display på 16 "kolonner" og 2 linjer/rækker.
    lcd.begin(16, 2);
    lcd.setCursor(3, 0);

    lcd.print("MRW-piller");
}

// Print log på LCD, i midten
void LCDLog(String str) {
    Serial.print(str);

    size_t len = strlen(str.c_str());
    if (len > 0 && len < 16) {
        uint8_t pos = (16 - len) / 2;
        lcd.setCursor(pos, 1);
    }
    else {
        lcd.setCursor(0, 1);
    }
    lcd.print(str);
}

void LCDLogln(String str) {
    LCDLog(str);
    Serial.print("\n");
}

void LCDClearLine() {
    lcd.setCursor(0, 1);
    lcd.print("            ");
}

// Opsætning af ESPNOW
void ESPNOWSetup() {
    if (esp_now_init() != ESP_OK) {
        Serial.println("Kunne ikke initializere ESP-NOW");
        return;
    }

    // Registrer den forbundede peer
    memcpy(peerInfo.peer_addr, peer_address, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    
    // Tilføj peer  
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Kunne ikke tilføje peer");
        return;
    }

    // Registrer callback når data bliver modtaget
    esp_now_register_recv_cb(OnDataRecv);
}

// Opsætning af WiFi
void WifiSetup() {
    // Login til WiFi-nætværk, der svarer til værdierne.
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    LCDLog("Wi-Fi forbinder");


    // Vent indtil at Wifi er forbundet, eller at den har forsøgt for meget.
    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        
        if (i >= WIFI_RETRIES) {
            LCDLogln(" Offline mode");
            WiFi.disconnect(false, false);
            delay(1000);
            LCDClearLine();
            return;
        }

        i++;
        delay(1000);
    }

    LCDLogln(" Wi-Fi forbundet");
    delay(1000);
    LCDClearLine();
}

void soundSetup() {
    ledcSetup(SOUND_CHANNEL, SOUND_FREQUENCY, SOUND_RES);
    ledcAttachPin(SOUND_PIN, SOUND_CHANNEL);
}

// Setup funktion der kører én gang når ESP'en starter.
void setup() {
    Serial.begin(115200);

    // Opsætning af LCD
    LCDSetup();
    // Opsætning af lyd-output
    soundSetup();

    // Sæt WiFi-modulet til at fungere både som access point og station.
    // Dette gør at ESP-NOW OG POST-requests virker.
    WiFi.mode(WIFI_AP_STA);
    // Opsætning af Wifi
    WifiSetup();
    // Opsætning af ESPNOW
    ESPNOWSetup();

    // Sætter hastigheden på steppermotoren.
    stepper.setSpeed(stepSpeed);
}

// Send besked til "serveren", altså remoten, men kun hvis den er forbundet til internettet.
void sendToServer() {
    if(WiFi.status()== WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;
        
        // Server_ip'en bliver indtastet
        http.begin(client, SERVER_IP);
        
        // Beskriv at vi sender JSON til serveren, så serveren ved hvordan det skal behandles.
        http.addHeader("Content-Type", "application/json");

        // JSON-dataene som en rå string, der bliver sendt.
        String data = "{\"id\":\"" DATA_USER_ID "\",\"serial\":\"" DATA_SERIAL "\",\"data\":{\"pulse\": 70}}";
        // Faktisk send dataene, og modtag en status-kode. Burde gerne være 200, OK
        int httpResponseCode = http.POST(data);
        
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
            
        // Stop forbindelsen, så den ikke ligger og fylder i hukommelsen.
        http.end();
    }
    else {
        Serial.println(" Offline");
    }
}

// Kører hele tiden
void loop() {
    // Hvis motoren skal dreje
    if (turn) {
        // Skriv til LCD at den er ved at gøre noget, samtidig med at en lyd spiller.
        LCDLog("Pilling you...");

        ledcWriteTone(SOUND_CHANNEL, SOUND_FREQUENCY);
        
        stepper.step(seventh);

        ledcWrite(SOUND_CHANNEL, 0);

        LCDClearLine();
        turn = false;

        // Send besked til server for at logge det.
        sendToServer();
    }

    delay(100);
}
