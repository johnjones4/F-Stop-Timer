#ifndef Runtime_h
#define Runtime_h

#include <InputManager.h>
#include <OutputManager.h>
#include <Memory.h>

#define N_STOPS 7

class Runtime {
public:
  void begin();
  void step();
private:
  void reset();
  bool changedBaseTime();
  bool changedStepInterval();
  bool changedPrintStop();
  bool changedMem();
  void runningTimer();
  InputManager* input;
  OutputManager* output;
  Memory* memory;
  Settings settings;
  Mode lastMode;
  unsigned long times[N_STOPS];
  int nTimes = 1;
  int currentTime = 0;
  unsigned long start = 0;
  bool writeMode = false;
  unsigned long nextClick = 0;
};

#endif