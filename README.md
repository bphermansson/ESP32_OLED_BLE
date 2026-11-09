# ESP32_OLED_BLE
Receive Android notifications on ESP32 with an OLED screen via Bluetooth LE. 

Also functions as a clock, the time is set automatically via BLE when connected to the app.

Based on https://github.com/fbiego/ESP32_OLED_BLE
Coded by me and Copilot.

## Features
+ Receive notifications
+ Clock display (12 & 24hr mode)
+ Screen & Notification timeout settings

## Audio

A MAX98357 I2S amplifier plays a tone at startup and for each notification.

## Visual

A LED can be connected, it flashes when a notifications arrive.

Chronos Quiet Hours and Sleep Time act as Do Not Disturb: notification messages
and tones are suppressed, and both LEDs are turned off. The clock remains
visible on the OLED.

## Settings

All setting are done in platformio.ini.

## Android app

Chronos

<a href='https://chronos.ke/app?id=esp32'><img alt='Download Chronos' height="100px" src='https://chronos.ke/img/chronos.png'/></a>

### Setup
- Go to the watch tab and click on the Watches button
- Pair new watch and select the ESP32
- To test, click Notifications and use Notifications Test to send test messages.
- In Notifications/Apps, set which apps to get notifications from.




