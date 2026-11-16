#ifndef NIGHTLY_RESTART_H
#define NIGHTLY_RESTART_H

#include <time.h>

inline int nightlyRestartDate(const tm &localTime) {
  return (localTime.tm_year + 1900) * 1000 + localTime.tm_yday;
}

inline bool shouldRestartTonight(bool timeSynced, const tm &localTime,
                                 int lastRestartDate) {
  return timeSynced && localTime.tm_hour == 4 && localTime.tm_min == 0 &&
         nightlyRestartDate(localTime) != lastRestartDate;
}

#endif
