#include <Arduino.h>
#include "notificationCallback.h"
#include "showNotification.h"

extern long timer;
extern int scroll;
extern bool scrOff;

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
  msgTitle = notification.title;
  msg = notification.message.substring(0, 126);
  msglen = msg.length();
  lines = ceil(float(msglen) / 21);
  scroll = 0;
  scrOff = false;
}