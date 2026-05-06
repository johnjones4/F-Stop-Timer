#include <fstop.h>
#include <math.h>

int generateTimes(unsigned long *times, int nTimes, unsigned long baseTime, double stepInterval)
{
  if (nTimes % 2 == 0) {
    return -1;
  }
  int base = -1 * (float)((nTimes - 1)/2);
  for (int i = 0; i < nTimes; i++) {
    unsigned long time = generateTime(baseTime, stepInterval, base + i);
    if (i > 0) {
      time = time - generateTime(baseTime, stepInterval, base + (i-1));
    }
    times[i] = time;
  }
  return 0;
}

unsigned long generateTime(unsigned long baseTime, double stepInterval, int nStops)
{
  double stops = (double)nStops * stepInterval;
  return (unsigned long)((double)baseTime * pow(2, stops));
}