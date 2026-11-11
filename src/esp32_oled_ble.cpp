#include <Wire.h>
#include <ChronosESP32.h>
#include "display.h"
#include "showNotification.h"
#include "printLocalTime.h"
#include "notificationCallback.h"
#include "playPling.h"

static_assert(SCREEN_ROTATION >= 0 && SCREEN_ROTATION <= 3,
              "SCREEN_ROTATION must be between 0 and 3");
static_assert(CLOCK_TEXT_SIZE > 0 && CLOCK_TEXT_SIZE <= 255 &&
              CLOCK_DETAIL_TEXT_SIZE > 0 && CLOCK_DETAIL_TEXT_SIZE <= 255 &&
              NOTIFICATION_TEXT_SIZE > 0 && NOTIFICATION_TEXT_SIZE <= 255,
              "Text sizes must be between 1 and 255");

#if defined(SSD1306_ENABLED) && SSD1306_ENABLED
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#elif defined(ST7789_ENABLED) && ST7789_ENABLED
  Adafruit_ST7789 display(&SPI, TFT_CS, TFT_DC, TFT_RST);
#endif

ChronosESP32 watch("OLED Watch");

extern uint8_t SmallFont[], MediumNumbers[];

static bool deviceConnected = false;
static bool audioReady = false;
bool indicatorBlinking = false;
bool notificationActive = false;
unsigned long indicatorTimer = 0;
static int id = 0;
long timeout = 30000, timer = 0, scrTimer = 0;
bool rotate = false, flip = false, hr24 = true, notify = true, screenOff = false, scrOff = false, b1;
int bat = 0, lines = 0, msglen = 0;

bool isDndActive() {
  return watch.isQuietActive() || watch.isSleepActive();
}

void setup() {
  Serial.begin(115200);
  delay(4000);
  Serial.println("Starting watch");
  pinMode(BUILTINLED, OUTPUT);
  pinMode(INDICATORLED, OUTPUT);
  digitalWrite(INDICATORLED, LED_OFF);

#if defined(SSD1306_ENABLED)// && SSD1306_ENABLED
  Wire.begin(CUSTOM_SDA, CUSTOM_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while(true);
  }
#else
  if (!SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS)) {
    Serial.println(F("Failed to initialize TFT SPI"));
    while (true) delay(1000);
  }
  display.init(SCREEN_WIDTH, SCREEN_HEIGHT);
#endif

  display.setRotation(SCREEN_ROTATION);
  display.cp437(true);
  clearScreen();
  display.setTextSize(4);
  display.setTextColor(ST77XX_WHITE);
  display.setCursor(0, 10);
  display.println(F("Welcome!"));
  updateScreen();

  watch.setNotificationCallback(notificationCallback);
  watch.begin();
  watch.set24Hour(true);
  watch.setBattery(80);

  timer = millis() - timeout;

  if (AUDIO_ENABLED) {
    audioReady = audioSetup();
    if (audioReady && !isDndActive()) {
      playPling(1046.5, 0.6); // 1046.5 Hz (C6 note) for 0.6 seconds
    }
  }
}

void loop() {
  watch.loop();

  enum class ScreenView { Startup, Clock, Notification, Off };
  static ScreenView displayedView = ScreenView::Startup;
  static String displayedClock;
  static bool displayedConnection = false;
  static unsigned long displayedNotification = 0;

  long cur = millis();
  const bool dndActive = isDndActive();
  digitalWrite(BUILTINLED, !dndActive && watch.isConnected());

  if (dndActive) {
    notificationActive = false;
    indicatorBlinking = false;
  }

  if (indicatorBlinking && (unsigned long)cur - indicatorTimer >= 5000) {
    indicatorBlinking = false;
  }
  digitalWrite(INDICATORLED,
               !dndActive && indicatorBlinking && (cur / 300) % 2 ? LED_ON : LED_OFF);

  if (!dndActive && notificationActive && cur < timer + timeout) {
    scrTimer = cur;
    const unsigned long revision = notificationRevision();
    if (displayedView != ScreenView::Notification || displayedNotification != revision) {
      clearScreen();
      showNotification();
      displayedView = ScreenView::Notification;
      displayedNotification = revision;
    }
  } else {
    notificationActive = false;
    if (!scrOff && screenOff && cur > scrTimer + timeout) {
      scrOff = true;
    }

    if (scrOff && screenOff) {
      if (displayedView != ScreenView::Off) {
        clearScreen();
        updateScreen();
        displayedView = ScreenView::Off;
      }
    } else {
      const String clock = watch.getHourZ() + watch.getTime(":%M %a %d %b");
      const bool connected = watch.isConnected();
      if (displayedView != ScreenView::Clock ||
          displayedClock != clock || displayedConnection != connected) {
        if (displayedView != ScreenView::Clock) {
          clearScreen();
          invalidateAnimation();
        }
        printLocalTime();
        displayedView = ScreenView::Clock;
        displayedClock = clock;
        displayedConnection = connected;
      }
      drawDisconnectedAnimation(!connected && displayedView == ScreenView::Clock);
    }
  }
  if (AUDIO_ENABLED && consumeNotificationTone() && audioReady && !dndActive) {
    playPling(1046.5, 0.6);
  }
}
