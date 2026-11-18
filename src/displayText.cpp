#include "displayText.h"
#include "utf8ToDisplay.h"

static const char *emojiAlias(uint32_t codepoint) {
  switch (codepoint) {
    case 0x2665:
    case 0x2764:
    case 0x1f90d:
    case 0x1f90e:
    case 0x1f9e1:
    case 0x1f49b:
    case 0x1f49a:
    case 0x1f499:
    case 0x1f49c:
    case 0x1f5a4:
    case 0x1fa77:
    case 0x1fa78:
      return "<3";
    case 0x1f44d:
      return "(Y)";
    case 0x1f44e:
      return "(N)";
    case 0x1f600:
    case 0x1f603:
    case 0x1f604:
    case 0x1f601:
    case 0x1f606:
    case 0x1f60a:
    case 0x1f642:
    case 0x263a:
      return ":)";
    case 0x1f605:
    case 0x1f609:
      return ";)";
    case 0x1f602:
    case 0x1f923:
      return "xD";
    case 0x1f60d:
    case 0x1f970:
      return "<3";
    case 0x1f622:
    case 0x1f62d:
      return ":'(";
    case 0x1f641:
    case 0x1f61e:
    case 0x1f615:
      return ":(";
    case 0x1f621:
    case 0x1f92c:
      return ">:(";
    case 0x1f60e:
      return "B-)";
    case 0x1f44f:
      return "[clap]";
    case 0x1f64f:
      return "[pray]";
    case 0x1f389:
      return "[party]";
    case 0x1f525:
      return "[fire]";
    case 0x2705:
      return "[ok]";
    case 0x274c:
    case 0x274e:
      return "[x]";
    default:
      return nullptr;
  }
}

String displayText(const String &utf8) {
  String result;
  result.reserve(utf8.length());
  const char *text = utf8.c_str();
  while (*text) {
    const uint32_t codepoint = nextDisplayCodepoint(text);
    if ((codepoint >= 0xfe00 && codepoint <= 0xfe0f) ||
        (codepoint >= 0x1f3fb && codepoint <= 0x1f3ff) ||
        codepoint == 0x200d || codepoint == 0x20e3) {
      continue;
    }

    const char *alias = emojiAlias(codepoint);
    if (alias != nullptr) {
      result += alias;
      continue;
    }

    uint8_t character = '?';
    switch (codepoint) {
      case 0x00c4: character = 0x8e; break;
      case 0x00c5: character = 0x8f; break;
      case 0x00d6: character = 0x99; break;
      case 0x00e4: character = 0x84; break;
      case 0x00e5: character = 0x86; break;
      case 0x00f6: character = 0x94; break;
      default:
        if (codepoint < 0x80) character = static_cast<uint8_t>(codepoint);
        break;
    }
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
