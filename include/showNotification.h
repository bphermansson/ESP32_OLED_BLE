#ifndef SHOW_NOTIFICATION_H
#define SHOW_NOTIFICATION_H
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

extern String msg, msgTitle;
extern int msglen, lines;

void showNotification();
void copyMsg(String ms);
#endif