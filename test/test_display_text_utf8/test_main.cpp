#include <cassert>
#include <string>
#include "displayText.h"

int main() {
  assert(std::string(displayText("Hej\n123").c_str()) == "Hej\n123");
  assert(std::string(displayText("\xc3\xa5\xc3\xa4\xc3\xb6 \xc3\x85\xc3\x84\xc3\x96").c_str()) ==
         "\xc3\xa5\xc3\xa4\xc3\xb6 \xc3\x85\xc3\x84\xc3\x96");
  assert(std::string(displayText("A\xf0\x9f\x98\x80!").c_str()) == "A:)!");
  assert(std::string(displayText("A\xf0\x9f\x90\xb6!").c_str()) == "A?!");
  assert(std::string(displayText("\xe2\x9d\xa4\xef\xb8\x8f \xf0\x9f\x91\x8d").c_str()) ==
         "<3 (Y)");
  assert(std::string(displayText("\xf0\x9f\x91\x8d\xf0\x9f\x8f\xbd").c_str()) == "(Y)");
  assert(std::string(displayText("\xf0\x9f\x98\x80 \xf0\x9f\x98\x82 \xf0\x9f\x98\x8d").c_str()) ==
         ":) xD <3");
  assert(std::string(displayText("\xf0\x9f\x8e\x89 \xf0\x9f\x94\xa5").c_str()) ==
         "[party] [fire]");
  assert(std::string(displayText("\xc3").c_str()) == "?");
  assert(std::string(displayText("\xc3" "A").c_str()) == "?A");
}
