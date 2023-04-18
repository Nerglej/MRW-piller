# MRW Pille-motor
Motoren forbinder til MRW remote via Bluetooth Low Energy.

## Materialer
1. ESP32 board (NodeMCU ESP32)
1. Stepmotor  
1. stepmotor-driver (ULN2003)
1. LCD
1. Højtaler

## Opsætning
Husk at modificere mac.h så MAC-adresserne passer til dine ESP32.
peer_address skal være Mac-adressen på den ESP32 som den skal forbinde til.
mac_address er ikke nødvendig, men kan sættes til MAC-adressen som koden kører på.

## Pins
| Speaker | Pin |
| SOUND   | 23  |

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
