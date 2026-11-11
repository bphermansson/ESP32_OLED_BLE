#ifndef NOTIFICATION_CALLBACK_H
#define NOTIFICATION_CALLBACK_H
#include <ChronosESP32.h>
extern String msg, msgTitle;
void notificationCallback(Notification notification);
bool consumeNotificationTone();
bool isDndActive();
unsigned long notificationRevision();

#endif