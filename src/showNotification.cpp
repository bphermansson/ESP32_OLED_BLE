#include "showNotification.h"
#include <Arduino.h>

String msg;
String msg0, msg1, msg2, msg3, msg4, msg5;
    
void showNotification() {
  // myOLED.setFont(SmallFont);
  copyMsg(msg);
  if (msglen == 0) {
    msg0 = "No message";
  }
  // myOLED.print(msg0, LEFT, 1 - scroll);
  // myOLED.print(msg1, LEFT, 12 - scroll);
  // myOLED.print(msg2, LEFT, 23 - scroll);
  // myOLED.print(msg3, LEFT, 34 - scroll);
  // myOLED.print(msg4, LEFT, 45 - scroll);
  // myOLED.print(msg5, LEFT, 56 - scroll);


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