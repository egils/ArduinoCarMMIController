# ArduinoCarMMIController

This is an arduino controller code to work as a transreceiver for various messages via BLE (Bluetooth Low Energy) and VAG CAN BUS. 

## Currently working

### Receive messages via BLE 4.0 and write them on a DIS display

BLE messages are read and written to a CANBUS with message codes for DIS display lines 1 and 2 (VAG CAN BUS only). Message format and protocol is custom because of BLE package size limitations. Example of working Android app can be found in [SpotifyBLE](https://github.com/egils/SpotifyBLE) repository. This app reads title of a song currently playing on Spotify and sends it to a hardcoded (for now) BLE device.

### Read all incomming CAN messages from convenience* bus

*Reading is bound to 200kbps bus speed, so to work with other bus on a car you just need to change that to a speed of the bus you are working on.

## Planned

- [ ] Reading car state messages and sending accoring messages via custom BLE protocol (to report driving state, vol up/down, play/pause button actions and etc.)
- [ ] Read car ECU data and report via custom BLE protocol (to elimnate the need of elm327 adapter)
