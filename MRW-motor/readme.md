# MRW Pille-motor
Motoren forbinder til MRW remote via Bluetooth Low Energy. Den forbinder også til et WiFi-netværk.

## Materialer
1. ESP32 board (NodeMCU ESP32)
1. Stepmotor  
1. stepmotor-driver (ULN2003)
1. LCD
1. Højtaler

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
| VSS | GND |
| VDD | 5V  |
| RS  | 21  |
| RW  | GND |
| E   | 22  |
| D0  | N.C |
| D1  | N.C |
| D2  | N.C |
| D3  | N.C |
| D4  | 27  |
| D5  | 26  |
| D6  | 25  |
| D7  | 32  |
| A   | 5V  |
| K   | GND |

Se dette link for mere: https://lastminuteengineers.com/esp32-pinout-reference/
