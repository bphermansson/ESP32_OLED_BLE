#include <cassert>
#include <string>
#include "utf8ToDisplay.h"

static std::string convert(const char *text) {
  std::string result;
  while (*text) {
    result += static_cast<char>(nextDisplayCharacter(text));
  }
  return result;
}

int main() {
  assert(convert("Hej\n123") == "Hej\n123");
  assert(convert("\xc3\xa5\xc3\xa4\xc3\xb6\xc3\x85\xc3\x84\xc3\x96") ==
         std::string("\x86\x84\x94\x8f\x8e\x99", 6));
  assert(convert("A\xc3\xa5" "B") == std::string("A\x86" "B", 3));
  assert(convert("\xf0\x9f\x98\x80!") == "?!");
  assert(convert("\xc3") == "?");
  assert(convert("\xc3" "A") == "?A");
  assert(convert("\xc0\x80") == "??");
  assert(convert("\xed\xa0\x80") == "?");
}
