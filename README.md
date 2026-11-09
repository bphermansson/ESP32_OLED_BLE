# ESP32_OLED_BLE
Receive Android notifications on ESP32 with an OLED screen via Bluetooth LE. 

Also functions as a clock, the time is set automatically via BLE when connected to the app.

Based on https://github.com/fbiego/ESP32_OLED_BLE

## Features
+ Receive notifications
+ Clock display (12 & 24hr mode)
+ Screen & Notification timeout settings

## Audio

A MAX98357 I2S amplifier plays a tone at startup and for each notification.
Connect BCLK to GPIO2, LRC to GPIO1, and DIN to GPIO3. The tone uses the Audio Tools I2S stream; avoid
mixing it with calls to the legacy `driver/i2s.h` API.


## Android app

Chronos

<a href='https://chronos.ke/app?id=esp32'><img alt='Download Chronos' height="100px" src='https://chronos.ke/img/chronos.png'/></a>

### Setup
- Go to the watch tab and click on the Watches button
- Pair new watch and select the ESP32
- To test, click Notifications and use Notifications Test to send test messages.
- In Notifications/Apps, set which apps to get notifications from.

## Libraries

Download [`ChronosESP32`](https://www.arduinolibraries.info/libraries/chronos-esp32) library from Arduino Library Manager or [`ChronosESP32`](https://github.com/fbiego/chronos-esp32)

Extract the `OLED_I2C` & `ChronosESP32` library to your Arduino libraries folder

The `OLED_I2C` library has been modified to include the `flipMode()` function

