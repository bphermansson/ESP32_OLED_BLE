# desktopNotifier
Receive and display Android notifications!
This device emulates a Chronos smart watch and let you see selected notifications directly without the need to pick up the phone or smartwatch.
When no new notification arrives, the screen show current time and date. The GUI also has a green/red line. Red when not connected to the phone, otherwise green.

Based on https://github.com/fbiego/ESP32_OLED_BLE.
Coded by me and Copilot.

## Hardware
The hardware used is a ESP32-C3 with an OLED display. Support for SSD1306- and ST7789-based displays. Optional, add a Led and/or DAC/Amp (MAX98357A) + speaker to get light and sound when a new notification is detected. These can be muted during nighttime.

## Settings
All setting are done in platformio.ini.
- Set pins for leds and amplifier, if used.
- Select exactly one display by enabling `SSD1306_ENABLED` or `ST7789_ENABLED`
in the build flags, and set the matching screen dimensions and pins.
- Set `SCREEN_ROTATION` to 0, 1, 2, or 3 for 0, 90, 180, or 270 degrees
clockwise. Keep the screen dimensions set to the display's native dimensions.


## Android app

Chronos

<a href='https://chronos.ke/app?id=esp32'><img alt='Download Chronos' height="100px" src='https://chronos.ke/img/chronos.png'/></a>

### Setup
- Go to the watch tab and click on the Watches button
- Pair new watch and select the ESP32
- To test, click Notifications and use Notifications Test to send test messages.
- In Notifications/Apps, set which apps to get notifications from.
- If you wish, set do not disturb times.

## UI tests

After PlatformIO has installed the dependencies, run the native LVGL UI test:

```sh
cmake -S test/test_ui -B /tmp/esp32-oled-ui-test \
  -DLVGL_DIR="$PWD/.pio/libdeps/nologo_esp32c3_super_mini/lvgl"
cmake --build /tmp/esp32-oled-ui-test
/tmp/esp32-oled-ui-test/ui_test
```

The UTF-8 conversion test can be run with:

```sh
g++ -std=c++11 -Itest/test_display_text_utf8 -Iinclude \
  test/test_display_text_utf8/test_main.cpp src/displayText.cpp -o /tmp/display-text-test
/tmp/display-text-test
```

The nightly restart schedule can be tested with:

```sh
g++ -std=c++11 -Iinclude test/test_nightly_restart/test_main.cpp -o /tmp/nightly-restart-test
/tmp/nightly-restart-test
```

