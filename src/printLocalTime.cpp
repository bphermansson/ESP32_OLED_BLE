#include <Arduino.h>
#include <ChronosESP32.h>
#include "display.h"
#include "displayText.h"
#include "printLocalTime.h"

extern ChronosESP32 watch;

void printLocalTime() {
  display.setTextSize(CLOCK_TEXT_SIZE);
  display.setTextColor(DISPLAY_WHITE, DISPLAY_BLACK);
  display.setCursor(0, 10);
  display.println(watch.getHourZ() + watch.getTime(":%M"));
  display.setTextSize(CLOCK_DETAIL_TEXT_SIZE);
  display.println(displayText(watch.getTime("%a %d %b")));
  display.println(watch.isConnected() ? "BLE connected   " : "BLE disconnected");
  updateScreen();
}
