#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <ChronosESP32.h>
#include "printLocalTime.h"

extern Adafruit_SSD1306 display;
extern ChronosESP32 watch;

void printLocalTime() {
  // myOLED.print(watch.getAmPmC(true),  RIGHT, 10);

  // myOLED.setFont(MediumNumbers);
  // myOLED.print(watch.getHourZ() + watch.getTime(":%M:%S"), CENTER, 0);
  // myOLED.setFont(SmallFont);
  // myOLED.print(watch.getTime("%a %d %b"), CENTER, 21);

  // if (watch.isConnected()) {
  //   myOLED.drawBitmap(0, 15, bluetooth, 16, 16);
  //   myOLED.drawRect(110, 23, 127, 30);
  //   myOLED.drawRectFill(108, 25, 110, 28);
  //   myOLED.drawRectFill(map(watch.getPhoneBattery(), 0, 100, 127, 110), 23, 127, 30);
  
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(watch.getHourZ() + watch.getTime(":%M:%S"));
  display.setTextSize(1);
  display.println(watch.getTime("%a %d %b"));
  display.println(watch.isConnected() ? "BLE connected" : "BLE disconnected");
  display.display();
}
