#include <cassert>
#include "nightlyRestart.h"

int main() {
  tm localTime = {};
  localTime.tm_year = 126;
  localTime.tm_yday = 319;
  localTime.tm_hour = 4;
  localTime.tm_min = 0;
  const int today = nightlyRestartDate(localTime);

  assert(!shouldRestartTonight(false, localTime, -1));
  assert(shouldRestartTonight(true, localTime, -1));
  assert(!shouldRestartTonight(true, localTime, today));
  localTime.tm_hour = 3;
  localTime.tm_min = 59;
  assert(!shouldRestartTonight(true, localTime, -1));
  localTime.tm_hour = 4;
  localTime.tm_min = 1;
  assert(!shouldRestartTonight(true, localTime, -1));
  localTime.tm_min = 0;
  localTime.tm_yday++;
  assert(shouldRestartTonight(true, localTime, today));
  localTime.tm_year++;
  localTime.tm_yday = 0;
  assert(shouldRestartTonight(true, localTime, today));
}
