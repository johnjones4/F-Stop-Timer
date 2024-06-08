#ifndef Application_h
#define Application_h

#include <SystemIO.h>
#include <PrintMode.h>
#include <TestMode.h>

class Application
{
public:
  Application();
  void begin();
  void step();
private:
  SystemIO* systemIo;
  PrintMode* printMode;
  TestMode* testMode;
};

#endif