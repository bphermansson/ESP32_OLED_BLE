#ifndef UTF8_TO_DISPLAY_H
#define UTF8_TO_DISPLAY_H

#include <stdint.h>

inline uint8_t nextDisplayCharacter(const char *&text) {
  const uint8_t first = static_cast<uint8_t>(*text++);
  if (first < 0x80) {
    return first;
  }

  uint32_t codepoint;
  unsigned int remaining;
  uint32_t minimum;
  if (first >= 0xc2 && first <= 0xdf) {
    codepoint = first & 0x1f;
    remaining = 1;
    minimum = 0x80;
  } else if (first >= 0xe0 && first <= 0xef) {
    codepoint = first & 0x0f;
    remaining = 2;
    minimum = 0x800;
  } else if (first >= 0xf0 && first <= 0xf4) {
    codepoint = first & 0x07;
    remaining = 3;
    minimum = 0x10000;
  } else {
    return '?';
  }

  while (remaining > 0) {
    const uint8_t next = static_cast<uint8_t>(*text);
    if ((next & 0xc0) != 0x80) {
      return '?';
    }
    codepoint = (codepoint << 6) | (next & 0x3f);
    ++text;
    --remaining;
  }

  if (codepoint < minimum || codepoint > 0x10ffff ||
      (codepoint >= 0xd800 && codepoint <= 0xdfff)) {
    return '?';
  }

  switch (codepoint) {
    case 0x00c4: return 0x8e;
    case 0x00c5: return 0x8f;
    case 0x00d6: return 0x99;
    case 0x00e4: return 0x84;
    case 0x00e5: return 0x86;
    case 0x00f6: return 0x94;
    default: return '?';
  }
}

#endif
