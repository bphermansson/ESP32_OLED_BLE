#ifndef DISPLAY_H
#define DISPLAY_H

#if defined(SSD1306_ENABLED) && SSD1306_ENABLED && defined(ST7789_ENABLED) && ST7789_ENABLED
#error "Select only one display type"
#elif defined(SSD1306_ENABLED) && SSD1306_ENABLED
#include <Adafruit_SSD1306.h>
extern Adafruit_SSD1306 display;
// constexpr uint16_t DISPLAY_WHITE = SSD1306_WHITE;
// constexpr uint16_t DISPLAY_BLACK = SSD1306_BLACK;

inline void clearScreen() { display.clearDisplay(); }
inline void updateScreen() { display.display(); }
#elif defined(ST7789_ENABLED) && ST7789_ENABLED
#include <Adafruit_ST7789.h>
extern Adafruit_ST7789 display;
// constexpr uint16_t DISPLAY_WHITE = ST77XX_WHITE;
// constexpr uint16_t DISPLAY_BLACK = ST77XX_BLACK;
// constexpr uint16_t DISPLAY_GREEN = 0x07E0;

inline void clearScreen() { display.fillScreen(ST77XX_BLACK); }
inline void updateScreen() {}
#else
#error "Enable either SSD1306_ENABLED or ST7789_ENABLED in platformio.ini"
#endif

#endif
