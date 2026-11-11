#ifndef TEST_ARDUINO_H
#define TEST_ARDUINO_H

#include <string>

class String {
 public:
  String(const char *text) : value(text) {}
  String() = default;
  size_t length() const { return value.length(); }
  void reserve(size_t size) { value.reserve(size); }
  const char *c_str() const { return value.c_str(); }
  String &operator+=(const char *text) { value += text; return *this; }
  String &operator+=(char character) { value += character; return *this; }

 private:
  std::string value;
};

#endif
