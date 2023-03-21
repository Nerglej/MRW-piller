#include <Arduino.h>
#include <Stepper.h>
#include <Wifi.h>

#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19

#define stepsPerRev 2038
#define stepSpeed 15

#define wifi_ssid "Wills Spot"
#define wifi_password "Jelgrens"

const int half = stepsPerRev / 2;
const int seventh = stepsPerRev / 7;

Stepper stepper = Stepper(stepsPerRev, IN1, IN3, IN2, IN4);

void bleSetup() {

}

void connectWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);

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

    connectWifi();

    stepper.setSpeed(stepSpeed);
}

void loop() {
    stepper.step(half);
    delay(250);
}


