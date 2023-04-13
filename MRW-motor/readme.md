# MRW Pille-motor
Motoren forbinder til MRW remote via Bluetooth Low Energy. Den forbinder også til et WiFi-netværk.

## Materialer
1. ESP32 board (NodeMCU ESP32 blev brugt under udvikling)
2. Stepmotor og stepmotor-driver.
3. LCD
4. Højtaler

## WiFi
For at forbinde til et WiFi-netværk, skal du lave en fil ved siden af main.cpp, der hedder WiFiCreds.h, der indeholder følgende:
```c++
#define SSID "ssid"
#define PASSWORD "password"
```
Hvor du selvfølgelig inputter navnet på netværket i SSID og kodeordet i PASSWORD.
Grunden til at dette er lavet i en fil for sig selv, er for at det er nemmere at lave ændre koderne for nogle der har lidt mindre styr
på kode, samtidig med at det sikrer at credentialsne ikke bliver uploadet til GitHub eller andre steder, hvilket ville være et sikkerhedsbrud.

## Pins
| Driver | Pin |
|--------|-----|
| IN1    | 16  |
| IN2    | 17  |
| IN3    | 18  |
| IN4    | 19  |

| LCD | Pin |
|-----|-----|
|   s |   s |

Se dette link for mere: https://lastminuteengineers.com/esp32-pinout-reference/
