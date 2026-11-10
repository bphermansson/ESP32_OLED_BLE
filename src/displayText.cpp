#include "displayText.h"
#include "utf8ToDisplay.h"

String displayText(const String &utf8) {
  String result;
  result.reserve(utf8.length());
  const char *text = utf8.c_str();
  while (*text) {
    result += static_cast<char>(nextDisplayCharacter(text));
  }
  return result;
}
