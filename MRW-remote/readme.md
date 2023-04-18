# MRW Pille-remote
Dette er en kort beskrivelse af hvordan denne remote skal sættes til.

## Materialer
Projektet kræver nogle ting:
1. ESP32 board (NodeMCU ESP32 el. andet)
2. 5 knapper

## Opsætning
Husk at modificere mac.h så MAC-adresserne passer til dine ESP32.
peer_address skal være Mac-adressen på den ESP32 som den skal forbinde til.
mac_address er ikke nødvendig, men kan sættes til MAC-adressen som koden kører på.

## Pins
De 5 knapper bliver tilsluttet til ESP32. Dette gøres, for det første, ved at alle knapperne bliver tilsluttet til GND.
Hver af de 5 knapper betyder én af fem ting: op, ned, højre, venstre og send. Ingen modstand behøves, da en indre modstand i ESP32 benyttes, med en pinmode der hedder INPUT_PULLUP.

| Knap    | Pin |
|---------|-----|
| Send    | 33  |
| Op      | 32  |
| Ned     | 27  |
| Venstre | 26  |
| Højre   | 25  |

Se dette link for mere: https://lastminuteengineers.com/esp32-pinout-reference/