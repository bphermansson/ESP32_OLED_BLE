#include "showNotification.h"
#include <Arduino.h>

extern int scroll;
String msg, msgTitle;
String msg0, msg1, msg2, msg3, msg4, msg5;
    
void showNotification() {
  copyMsg(msg);
  if (msglen == 0) {
    msg0 = "No message";
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, -scroll);
  display.println(msgTitle.substring(0, 21));
  display.println(msg0);
  display.println(msg1);
  display.println(msg2);
  display.println(msg3);
  display.println(msg4);
  display.println(msg5);
  display.display();
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