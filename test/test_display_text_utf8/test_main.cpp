#include <cassert>
#include <string>
#include "displayText.h"

int main() {
  assert(std::string(displayText("Hej\n123").c_str()) == "Hej\n123");
  assert(std::string(displayText("\xc3\xa5\xc3\xa4\xc3\xb6 \xc3\x85\xc3\x84\xc3\x96").c_str()) ==
         "\xc3\xa5\xc3\xa4\xc3\xb6 \xc3\x85\xc3\x84\xc3\x96");
  assert(std::string(displayText("A\xf0\x9f\x98\x80!").c_str()) == "A?!");
  assert(std::string(displayText("\xc3").c_str()) == "?");
  assert(std::string(displayText("\xc3" "A").c_str()) == "?A");
}
