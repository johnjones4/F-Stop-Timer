#ifndef Mode_h
#define Mode_h

#include <SystemIO.h>


class Mode
{
public:
  Mode(SystemIO* systemIo);
  virtual void step() = 0;
protected:
  SystemIO* systemIo;
};

#endif