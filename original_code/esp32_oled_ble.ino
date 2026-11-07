/*
   MIT License

   Copyright (c) 2021 Felix Biego

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ChronosESP32.h>
#include "graphics.h"

#define BUILTINLED 2
#define BUTTON 0 // ESP32 BOOT button

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define your custom I2C pins here
#define CUSTOM_SDA 8
#define CUSTOM_SCL 9

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
ChronosESP32 watch("OLED Watch");

extern uint8_t SmallFont[], MediumNumbers[];

static bool deviceConnected = false;
static int id = 0;
long timeout = 10000, timer = 0, scrTimer = 0;
bool rotate = false, flip = false, hr24 = true, notify = true, screenOff = false, scrOff = false, b1;
int scroll = 0, bat = 0, lines = 0, msglen = 0;
String msg;
String msg0, msg1, msg2, msg3, msg4, msg5;


void configCallback(Config config, uint32_t a, uint32_t b)
{
  switch (config)
  {
    case CF_USER:

      flip = ((b >> 24) & 0xFF) == 1; // imperial normal, metric flipped
      rotate = ((b >> 8) & 0xFF) == 1; // celsuis normal, fahrenheit rotated
      timeout = ((b >> 16) & 0xFF) * 1000; //

      break;
  }
}


void notificationCallback(Notification notification)
{
  Serial.print("Notification received at ");
  Serial.println(notification.time);
  Serial.print("From: ");
  Serial.print(notification.app);
  Serial.print("\tIcon: ");
  Serial.println(notification.icon);
  Serial.println(notification.message);
   Serial.println(notification.title);

  timer = millis();
  msg = notification.message.substring(0, 126);
  msglen = msg.length();
  lines = ceil(float(msglen) / 21);
  scroll = 0;
  scrOff = false;

  Serial.println(msglen);

}

void ringerCallback(String caller, bool state)
{
  if (state)
  {
    Serial.print("Ringer: Incoming call from ");
    Serial.println(caller);

    timer = millis() + 15000;
    msg = "Incoming call        " + caller;
    msglen = msg.length();
    lines = ceil(float(msglen) / 21);
    scroll = 0;

    scrOff = false;
  }
  else
  {
    Serial.println("Ringer dismissed");
    timer = millis() - timeout;
    scrOff = true;
  }
}

void copyMsg(String ms) {
  switch (lines) {
    case 1:
      msg0 = ms.substring(0, msglen);
      msg1 = "";
      msg2 = "";
      msg3 = "";
      msg4 = "";
      msg5 = "";
      break;
    case 2:
      msg0 = ms.substring(0, 21);
      msg1 = ms.substring(21, msglen);
      msg2 = "";
      msg3 = "";
      msg4 = "";
      msg5 = "";
      break;
    case 3:
      msg0 = ms.substring(0, 21);
      msg1 = ms.substring(21, 42);
      msg2 = ms.substring(42, msglen);
      msg3 = "";
      msg4 = "";
      msg5 = "";
      break;
    case 4:
      msg0 = ms.substring(0, 21);
      msg1 = ms.substring(21, 42);
      msg2 = ms.substring(42, 63);
      msg3 = ms.substring(63, msglen);
      msg4 = "";
      msg5 = "";
      break;
    case 5:
      msg0 = ms.substring(0, 21);
      msg1 = ms.substring(21, 42);
      msg2 = ms.substring(42, 63);
      msg3 = ms.substring(63, 84);
      msg4 = ms.substring(84, msglen);
      msg5 = "";
      break;
    case 6:
      msg0 = ms.substring(0, 21);
      msg1 = ms.substring(21, 42);
      msg2 = ms.substring(42, 63);
      msg3 = ms.substring(63, 84);
      msg4 = ms.substring(84, 105);
      msg5 = ms.substring(105, msglen);
      break;
  }
}

void button(bool b) {
  if (b) {
    if (!b1) {
      // button debounce, code to be executed between this and the next comment

      long current = millis();
      if (scrOff) {
        scrOff = false;
        scrTimer = current;
      } else {
        if (current < timer + timeout) {
          timer = current - timeout;
        } else {
          timer = current;
        }
        scroll = 0;
        scrOff = false;
      }


      // end button debounce
      b1 = true;
    }
  } else {
    b1 = false;
  }
}


void setup() {
  Serial.begin(115200);
  delay(4000);

  Serial.println("Starting watch");
  
  pinMode(BUILTINLED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Wire.begin(CUSTOM_SDA, CUSTOM_SCL);

  // Initialize the OLED display (default address 0x3C)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while(true);
  }


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Custom Pins Working!"));
  display.display();

  watch.setConfigurationCallback(configCallback);
  watch.setNotificationCallback(notificationCallback);
  watch.setRingerCallback(ringerCallback);
  
  watch.begin();

  watch.set24Hour(true);
  watch.setBattery(80);

  timer = millis() - timeout;

}

void loop() {
  watch.loop();

  button(digitalRead(BUTTON) == LOW);   // input pin

  display.clearDisplay();


  digitalWrite(BUILTINLED, watch.isConnected());

  long cur = millis();
  if (cur < timer + timeout) {
    scrTimer = cur;
    long rem = (timer + timeout - cur);
    if (lines > 3 && rem < timeout - (timeout / 3) && rem > (timeout / 3)) {
      scroll = map(rem, timeout - (timeout / 3), (timeout / 3), 0, (lines - 3) * 11); //scroll if longer than 3 lines
    }
    showNotification();   // show notification
  } else {
    if (!scrOff && screenOff && cur > scrTimer + timeout) {
      scrOff = true;
    }

    printLocalTime();   // display time
    if (scrOff & screenOff) {
  display.clearDisplay();
    }
  }

  // myOLED.update();

}

void showNotification() {
  // myOLED.setFont(SmallFont);
  copyMsg(msg);
  if (msglen == 0) {
    msg0 = "No messsage";
  }
  // myOLED.print(msg0, LEFT, 1 - scroll);
  // myOLED.print(msg1, LEFT, 12 - scroll);
  // myOLED.print(msg2, LEFT, 23 - scroll);
  // myOLED.print(msg3, LEFT, 34 - scroll);
  // myOLED.print(msg4, LEFT, 45 - scroll);
  // myOLED.print(msg5, LEFT, 56 - scroll);


}

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
  }



