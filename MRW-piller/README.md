# Ældrepleje og pille-dispenser
Dette repository indeholder en server til et projekt i UP.

Serveren modtager data (id, serienummer og andet) på et REST endpoint, hvor det så bliver gemt i en database eller en fil på serveren. Man kan så åbne siden som serveren er startet på, og se hvilke data der findes på serveren.

Dette bruges til at se om dem der har brug for produktet, ældre der skal tage pille og ud over det også har meget kontakt med læger, ældrepleje eller andet, faktisk også bruger produktet.

# REST API
## Sending to the server
Lav en POST-request til `/`, med en body der indeholder følgende: 
```json
{
    "id": "123",        // Et id der tilhører personen
    "serial": "123",    // Et serienummer på enheden der sendte dataene.
    "data": {           // Eventuelle data der skal med.
        "puls": 72,     // Fx en puls, vægt eller andet.
        "vægt": 80
    }
}
```
POST-requesten skal have headeren `Content-Type: application/json`

## Receiving data from the server
Lav en GET-request til `/`, og så får du dataene fra i dag.