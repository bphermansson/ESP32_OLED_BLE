#include "displayText.h"
#include "utf8ToDisplay.h"

String displayText(const String &utf8) {
  String result;
  result.reserve(utf8.length());
  const char *text = utf8.c_str();
  while (*text) {
    const uint8_t character = nextDisplayCharacter(text);
    switch (character) {
      case 0x8e: result += "\xc3\x84"; break;
      case 0x8f: result += "\xc3\x85"; break;
      case 0x99: result += "\xc3\x96"; break;
      case 0x84: result += "\xc3\xa4"; break;
      case 0x86: result += "\xc3\xa5"; break;
      case 0x94: result += "\xc3\xb6"; break;
      default: result += static_cast<char>(character); break;
    }
  }
  return result;
}
