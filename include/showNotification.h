#ifndef SHOW_NOTIFICATION_H
#define SHOW_NOTIFICATION_H
#include <Arduino.h>

extern String msg;
extern int msglen, lines;

void showNotification();
void copyMsg(String ms);
#endif