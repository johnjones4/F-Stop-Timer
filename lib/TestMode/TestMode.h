#ifndef TestMode_h
#define TestMode_h

#include <Mode.h>

class TestMode: public Mode
{
public:
  TestMode(SystemIO* systemIo);
  void step();
private:
  int timesIndex = -1;
  double* times;
  unsigned long stopAt = 0;
  unsigned long showDefaultTime();
  bool nextTime(bool start);
};

#endif