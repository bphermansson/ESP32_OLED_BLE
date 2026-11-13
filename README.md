# ESP32_OLED_BLE
Receive Android notifications on ESP32 with an OLED screen via Bluetooth LE. Optional, add a Led and/or DAC/Amp + speaker to get light and sound when a new notification is detected.

Also functions as a clock, the time is set automatically via BLE when connected to the app.

Support for two types of Oled/LCD, but more can be added.

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

Select exactly one display by enabling `SSD1306_ENABLED` or `ST7789_ENABLED`
in the build flags, and set the matching screen dimensions and pins.
Set `SCREEN_ROTATION` to 0, 1, 2, or 3 for 0, 90, 180, or 270 degrees
clockwise. Keep the screen dimensions set to the display's native dimensions.
LVGL renders the clock, notifications, and connection indicator. The layout
automatically uses a smaller clock font on smaller displays. The line at the
top of the clock is green when connected and red when disconnected. On a
monochrome SSD1306 it appears white when connected and is not visible when
disconnected. The runtime layout is defined in
`src/ui.c`; `ui_definition.yaml` is a designer reference and is not loaded
by the firmware.
The ST7789 uses hardware SPI on `TFT_SCLK` and `TFT_MOSI` for faster updates.
Notifications use LVGL's Montserrat 38 font, with matching-size Swedish glyphs;
other
unsupported Unicode characters are displayed as `?`.
The generated Montserrat font is licensed under the SIL Open Font License;
see [MONTSERRAT-OFL.txt](include/MONTSERRAT-OFL.txt).
The large clock font is a digits-and-colon subset of `src/my_font_133.c`.
After changing that source font, regenerate the firmware subset with
`node tools/subset-clock-font.js`; the full font is excluded from the build.
The date uses an ASCII-only subset of LVGL's Montserrat 48; regenerate it after
installing the PlatformIO dependencies with `node tools/subset-date-font.js`.
Its LVGL license is in [LVGL-MIT.txt](include/LVGL-MIT.txt). The build checks
the final binary size against the app partition before uploading.

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

## Android app

Chronos

<a href='https://chronos.ke/app?id=esp32'><img alt='Download Chronos' height="100px" src='https://chronos.ke/img/chronos.png'/></a>

### Setup
- Go to the watch tab and click on the Watches button
- Pair new watch and select the ESP32
- To test, click Notifications and use Notifications Test to send test messages.
- In Notifications/Apps, set which apps to get notifications from.
