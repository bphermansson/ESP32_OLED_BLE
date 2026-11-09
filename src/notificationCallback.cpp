#include <Arduino.h>
#include <atomic>
#include "notificationCallback.h"
#include "showNotification.h"

static std::atomic<unsigned long> pendingNotificationTones{0};

extern long timer;
extern int scroll;
extern bool scrOff;
extern bool indicatorBlinking;
extern unsigned long indicatorTimer;

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

  indicatorTimer = millis();
  timer = indicatorTimer;
  indicatorBlinking = true;
  msgTitle = notification.title;
  msg = notification.message.substring(0, 126);
  msglen = msg.length();
  lines = ceil(float(msglen) / 21);
  scroll = 0;
  scrOff = false;
  pendingNotificationTones.fetch_add(1, std::memory_order_relaxed);
}

bool consumeNotificationTone()
{
  if (pendingNotificationTones.load(std::memory_order_relaxed) == 0) {
    return false;
  }
  pendingNotificationTones.fetch_sub(1, std::memory_order_relaxed);
  return true;
}