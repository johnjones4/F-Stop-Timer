#include <timescale.h>
#include <math.h>

int generateTimes(double *times, int nTimes, double baseTime, double stopDelta)
{
  if (nTimes % 2 == 0) {
    return -1;
  }
  int base = -1 * (double)((nTimes - 1)/2);
  for (int i = 0; i < nTimes; i++) {
    double time = generateTime(baseTime, stopDelta, base + (double)i);
    if (i > 0) {
      time = time - times[i - 1];
    }
    times[i] = time;
  }
  return 0;
}

double generateTime(double baseTime, double stopDelta, double nSteps)
{
  float stops = nSteps * stopDelta;
  return baseTime * pow(2, stops);
}
