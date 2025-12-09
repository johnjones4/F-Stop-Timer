#ifndef OutputManager_h
#define OutputManager_h

#include <LedSequence.h>
#include <LedControl.h>

class OutputManager {
public:
  OutputManager(int address);
  bool begin();
  void setStepDeltaLed(int n);
  void setPrintStopLed(int n);
  void setTime(float n);
private:
  Adafruit_MCP23X17* mcp;
  int address;
  LedSequence *stepDelta;
  LedSequence *printStop;
  LedControl *lc;
};

#endif