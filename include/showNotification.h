#ifndef SHOW_NOTIFICATION_H
#define SHOW_NOTIFICATION_H
#include <Arduino.h>

extern String msg, msgTitle;
extern int msglen, lines;

void showNotification();
#endif