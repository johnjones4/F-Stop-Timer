#ifndef OutputManager_h
#define OutputManager_h

#include <LedSequence.h>
#include <LedControl.h>

//TODO speaker

class OutputManager {
public:
  OutputManager(int address);
  bool begin();
  void setStepDeltaLed(int n);
  void setPrintStopLed(int n);
  void setTime(unsigned long t);
  void setEnlarger(bool en);
  void click();
  void step();
private:
  Adafruit_MCP23X17* mcp;
  int address;
  LedSequence *stepDelta;
  LedSequence *printStop;
  LedControl *lc;
  unsigned long nextBuzzerOff = 0;
};

#endif