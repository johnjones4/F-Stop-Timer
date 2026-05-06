#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int generateTimes(unsigned long *times, int nTimes, unsigned long baseTime, double stepInterval);
unsigned long generateTime(unsigned long baseTime, double stepInterval, int nStops);

#ifdef __cplusplus
}
#endif
