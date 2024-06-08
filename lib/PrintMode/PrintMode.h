#ifndef PrintMode_h
#define PrintMode_h

#include <Mode.h>

class PrintMode: public Mode
{
public:
  PrintMode(SystemIO* systemIo);
  void step();
private:
  unsigned long stopAt = 0;
  unsigned long showDefaultTime();
};

#endif