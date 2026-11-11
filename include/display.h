#ifndef DISPLAY_H
#define DISPLAY_H

#if defined(SSD1306_ENABLED) && SSD1306_ENABLED && defined(ST7789_ENABLED) && ST7789_ENABLED
#error "Select only one display type"
#elif defined(SSD1306_ENABLED) && SSD1306_ENABLED
#include <Adafruit_SSD1306.h>
extern Adafruit_SSD1306 display;
#elif defined(ST7789_ENABLED) && ST7789_ENABLED
#include <Adafruit_ST7789.h>
extern Adafruit_ST7789 display;
#else
#error "Enable either SSD1306_ENABLED or ST7789_ENABLED in platformio.ini"
#endif

#endif
