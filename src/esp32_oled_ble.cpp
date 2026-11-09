#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ChronosESP32.h>
#include "showNotification.h"
#include "printLocalTime.h"
#include "notificationCallback.h"
#include "playPling.h"

#define BUILTINLED 2
#define INDICATORLED 21
#define LED_ON HIGH // GPIO10 LED is active-low
#define LED_OFF LOW
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define your custom I2C pins here
#define CUSTOM_SDA 8
#define CUSTOM_SCL 9

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
ChronosESP32 watch("OLED Watch");

extern uint8_t SmallFont[], MediumNumbers[];

static bool deviceConnected = false;
static bool audioReady = false;
bool indicatorBlinking = false;
unsigned long indicatorTimer = 0;
static int id = 0;
long timeout = 30000, timer = 0, scrTimer = 0;
bool rotate = false, flip = false, hr24 = true, notify = true, screenOff = false, scrOff = false, b1;
int scroll = 0, bat = 0, lines = 0, msglen = 0;

void setup() {
  Serial.begin(115200);
  delay(4000);
  Serial.println("Starting watch");
  pinMode(BUILTINLED, OUTPUT);
  pinMode(INDICATORLED, OUTPUT);
  digitalWrite(INDICATORLED, LED_OFF);
  Wire.begin(CUSTOM_SDA, CUSTOM_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("BLE Notify"));
  display.display();

  watch.setNotificationCallback(notificationCallback);
  watch.begin();
  watch.set24Hour(true);
  watch.setBattery(80);

  timer = millis() - timeout;

  audioReady = audioSetup();
  if (audioReady) {
    playPling(1046.5, 0.6); // 1046.5 Hz (C6 note) for 0.6 seconds
  }
}

void loop() {
  watch.loop();
  display.clearDisplay();
  digitalWrite(BUILTINLED, watch.isConnected());

  long cur = millis();
  if (indicatorBlinking && (unsigned long)cur - indicatorTimer >= 5000) {
    indicatorBlinking = false;
  }
  digitalWrite(INDICATORLED,
               indicatorBlinking && (cur / 300) % 2 ? LED_ON : LED_OFF);

  if (cur < timer + timeout) {
    scrTimer = cur;
    long rem = (timer + timeout - cur);
    if (lines > 3 && rem < timeout - (timeout / 3) && rem > (timeout / 3)) {
      scroll = map(rem, timeout - (timeout / 3), (timeout / 3), 0, (lines - 3) * 11); //scroll if longer than 3 lines
    }
    showNotification();
  } else {
    if (!scrOff && screenOff && cur > scrTimer + timeout) {
      scrOff = true;
    }

    printLocalTime();   // display time
    if (scrOff & screenOff) {
      display.clearDisplay();
    }
  }
  if (consumeNotificationTone() && audioReady) {
    playPling(1046.5, 0.6);
  }
}



