#include <Wire.h>
#include <ChronosESP32.h>
#include "display.h"
#include "displayText.h"
#include "notificationCallback.h"
#include "playPling.h"
#include <lvgl.h>
#include "ui.h"

static_assert(SCREEN_ROTATION >= 0 && SCREEN_ROTATION <= 3,
              "SCREEN_ROTATION must be between 0 and 3");
static constexpr size_t draw_width =
    (SCREEN_ROTATION % 2 == 0) ? SCREEN_WIDTH : SCREEN_HEIGHT;
static uint16_t draw_buffer[draw_width * 20];

static uint32_t lvgl_tick() {
  return static_cast<uint32_t>(millis());
}


static void flush_display(lv_display_t *lv_display, const lv_area_t *area,
                          uint8_t *pixels) {
  const uint32_t width = area->x2 - area->x1 + 1;
  const uint32_t height = area->y2 - area->y1 + 1;
#if defined(SSD1306_ENABLED) && SSD1306_ENABLED
  const uint16_t *colors = reinterpret_cast<const uint16_t *>(pixels);
  for (uint32_t y = 0; y < height; ++y) {
    for (uint32_t x = 0; x < width; ++x) {
      const uint16_t color = colors[y * width + x];
      const uint32_t red = ((color >> 11) & 0x1f) * 255 / 31;
      const uint32_t green = ((color >> 5) & 0x3f) * 255 / 63;
      const uint32_t blue = (color & 0x1f) * 255 / 31;
      const bool white = (77 * red + 150 * green + 29 * blue) >= 128 * 256;
      display.drawPixel(area->x1 + x, area->y1 + y, white ? SSD1306_WHITE : SSD1306_BLACK);
    }
  }
  if (lv_display_flush_is_last(lv_display)) display.display();
#else
  display.startWrite();
  display.setAddrWindow(area->x1, area->y1, width, height);
  display.writePixels(reinterpret_cast<uint16_t *>(pixels), width * height);
  display.endWrite();
#endif
  lv_display_flush_ready(lv_display);
}

#if defined(SSD1306_ENABLED) && SSD1306_ENABLED
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#elif defined(ST7789_ENABLED) && ST7789_ENABLED
  Adafruit_ST7789 display(&SPI, TFT_CS, TFT_DC, TFT_RST);
#endif

ChronosESP32 watch("OLED Watch");

static bool audioReady = false;
bool indicatorBlinking = false;
bool notificationActive = false;
unsigned long indicatorTimer = 0;
const unsigned long timeout = 30000;
unsigned long timer = 0, scrTimer = 0;
bool screenOff = false, scrOff = false;

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

#if defined(SSD1306_ENABLED) && SSD1306_ENABLED
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

  lv_init();
  lv_tick_set_cb(lvgl_tick);
  lv_display_t *lv_display = lv_display_create(display.width(), display.height());
  if (lv_display == nullptr) {
    Serial.println(F("Failed to create LVGL display"));
    while (true) delay(1000);
  }
  lv_display_set_color_format(lv_display, LV_COLOR_FORMAT_RGB565);
  lv_display_set_buffers(lv_display, draw_buffer, nullptr, sizeof(draw_buffer),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_flush_cb(lv_display, flush_display);
  ui_init();

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

  const unsigned long cur = millis();
  const bool dndActive = isDndActive();
  digitalWrite(BUILTINLED, !dndActive && watch.isConnected());

  if (dndActive) {
    notificationActive = false;
    indicatorBlinking = false;
  }

  if (indicatorBlinking && cur - indicatorTimer >= 5000) {
    indicatorBlinking = false;
  }
  digitalWrite(INDICATORLED,
               !dndActive && indicatorBlinking && (cur / 300) % 2 ? LED_ON : LED_OFF);

  if (!dndActive && notificationActive && cur - timer < timeout) {
    scrTimer = cur;
    const unsigned long revision = notificationRevision();
    if (displayedView != ScreenView::Notification || displayedNotification != revision) {
      const String title = displayText(msgTitle);
      const String message = displayText(msg.isEmpty() ? String("No message") : msg);
      ui_show_notification(title.c_str(), message.c_str());
      displayedView = ScreenView::Notification;
      displayedNotification = revision;
    }
  } else {
    notificationActive = false;
    if (!scrOff && screenOff && cur - scrTimer >= timeout) {
      scrOff = true;
    }

    if (scrOff && screenOff) {
      if (displayedView != ScreenView::Off) {
        ui_show_off();
        displayedView = ScreenView::Off;
      }
    } else {
      const String time = watch.getHourZ() + watch.getTime(":%M");
      const String date = displayText(watch.getTime("%a %d %b"));
      const String clock = time + " " + date;
      const bool connected = watch.isConnected();
      if (displayedView != ScreenView::Clock ||
          displayedClock != clock || displayedConnection != connected) {
        ui_show_clock(time.c_str(), date.c_str(), connected);
        displayedView = ScreenView::Clock;
        displayedClock = clock;
        displayedConnection = connected;
      }
    }
  }
  if (AUDIO_ENABLED && consumeNotificationTone() && audioReady && !dndActive) {
    playPling(1046.5, 0.6);
  }
  lv_timer_handler();
  delay(5);
}
