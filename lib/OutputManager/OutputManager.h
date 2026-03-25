#ifndef OutputManager_h
#define OutputManager_h

#include <LedSequence.h>
#include <LedControl.h>

class OutputManager {
public:
  OutputManager(int address);
  int begin();
  void displayStatus(int code);
  void setStepIntervalLed(int n);
  void setPrintStopLed(int n);
  void setTime(unsigned long t);
  void setEnlarger(bool en);
  void click();
  void step();
private:
  Adafruit_MCP23X17* mcp;
  int address;
  LedSequence *stepInterval;
  LedSequence *printStop;
  LedControl *lc;
  unsigned long nextBuzzerOff = 0;
};

#endif