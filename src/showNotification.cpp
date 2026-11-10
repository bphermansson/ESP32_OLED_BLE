#include "showNotification.h"
#include "display.h"
#include "displayText.h"

extern int scroll;
String msg, msgTitle;

static String notificationText() {
  return displayText(msgTitle + "\n" + (msglen == 0 ? String("No message") : msg));
}

int notificationScrollLimit() {
  display.setTextSize(NOTIFICATION_TEXT_SIZE);
  display.setTextWrap(true);
  int16_t x, y;
  uint16_t width, height;
  display.getTextBounds(notificationText(), 15, 0, &x, &y, &width, &height);
  return max(0, static_cast<int>(y) + height - display.height());
}

void showNotification() {
  display.setTextSize(NOTIFICATION_TEXT_SIZE);
  display.setTextWrap(true);
  display.setTextColor(DISPLAY_WHITE);
  display.setCursor(0, -scroll);
  display.print(notificationText());
  updateScreen();
}
