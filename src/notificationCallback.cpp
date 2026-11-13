#include <Arduino.h>
#include <atomic>
#include "notificationCallback.h"

String msg, msgTitle;

static std::atomic<unsigned long> pendingNotificationTones{0};
static std::atomic<unsigned long> notificationCount{0};

extern unsigned long timer;
extern bool scrOff;
extern bool notificationActive;
extern bool indicatorBlinking;
extern unsigned long indicatorTimer;

void notificationCallback(Notification notification)
{
  Serial.print("Notification received at ");
  Serial.println(notification.time);
  Serial.print("From: ");
  Serial.println(notification.title);
  Serial.println(notification.message);

  if (isDndActive()) {
    return;
  }

  indicatorTimer = millis();
  timer = indicatorTimer;
  notificationActive = true;
  indicatorBlinking = true;
  //msgTitle = notification.title;
  msgTitle = notification.title.substring(0, 12);
  msg = notification.message.substring(0, 126);
  scrOff = false;
  notificationCount.fetch_add(1, std::memory_order_release);
  pendingNotificationTones.fetch_add(1, std::memory_order_relaxed);
}

unsigned long notificationRevision()
{
  return notificationCount.load(std::memory_order_acquire);
}

bool consumeNotificationTone()
{
  if (pendingNotificationTones.load(std::memory_order_relaxed) == 0) {
    return false;
  }
  pendingNotificationTones.fetch_sub(1, std::memory_order_relaxed);
  return true;
}