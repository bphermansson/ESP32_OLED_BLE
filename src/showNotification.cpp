#include "showNotification.h"
#include "display.h"
#include "displayText.h"

String msg, msgTitle;

static String notificationText() {
  return displayText(msgTitle + "\n" + (msglen == 0 ? String("No message") : msg));
}

void showNotification() {
  display.setTextSize(NOTIFICATION_TEXT_SIZE);
  display.setTextWrap(true);
  display.setTextColor(ST77XX_WHITE);
  display.setCursor(0, 0);
  display.print(notificationText());
  updateScreen();
}
