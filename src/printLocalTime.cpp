#include <Arduino.h>
#include <ChronosESP32.h>
#include "display.h"
#include "displayText.h"
#include "printLocalTime.h"
#include "icon.h"

static const uint8_t* const frames[] = {
  frame0, frame1, frame2, frame3, frame4, frame5, frame6, frame7,
  frame8, frame9, frame10, frame11, frame12, frame13, frame14, frame15,
  frame16, frame17, frame18, frame19, frame20, frame21, frame22, frame23,
  frame24, frame25, frame26, frame27
};
static const int16_t ANIM_W = 128;
static const int16_t ANIM_H = 64;
static const size_t frameCount = sizeof(frames) / sizeof(frames[0]);

extern ChronosESP32 watch;

void printLocalTime() {
  display.setTextSize(CLOCK_TEXT_SIZE);
  display.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  display.setCursor(0, 10);
  display.println(watch.getHourZ() + watch.getTime(":%M"));
  display.setTextSize(CLOCK_DETAIL_TEXT_SIZE);
  display.println(displayText(watch.getTime("%a %d %b")));

  // display.println(watch.isConnected() ? "BLE connected   " : "BLE disconnected");
  updateScreen();
}

// Animation sits at the bottom centre; skipped on displays too small for it with the clock.
static bool invalidated = false;
static bool circleShown = false;

void invalidateAnimation() {
  invalidated = true;
}

void drawDisconnectedAnimation(bool active) {
  // const int16_t x = (display.width() - ANIM_W) / 2;
  // const int16_t y = display.height() - ANIM_H;
  const int16_t x = 200;
  const int16_t y = 10;
  
  if (display.height() < 224) {
    return;
  }
  static uint16_t idx = 0;
  static unsigned long t = 0;
  static bool shown = false;
  if (invalidated) {
    shown = false;
    circleShown = false;
    invalidated = false;
  }
  if (!active) {
    if (!circleShown) {
      display.fillRect(x, y, ANIM_W, ANIM_H, ST77XX_BLACK);
      display.fillCircle(x+5 + ANIM_W / 2, y-3 + ANIM_H / 2, ANIM_H / 2 - 15, ST77XX_GREEN);
      updateScreen();
      circleShown = true;
      shown = false;
    }
    return;
  }
  circleShown = false;
  if (shown && millis() - t < 100) {
    return;
  }
  t = millis();
  display.drawBitmap(x, y, frames[idx], ANIM_W, ANIM_H, ST77XX_WHITE, ST77XX_BLACK);
  updateScreen();
  idx = (idx + 1) % frameCount;
  shown = true;
}
