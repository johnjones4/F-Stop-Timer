#ifndef OutputManager_h
#define OutputManager_h

#include <IOutputManager.h>
#include <LedSequence.h>
#include <LedControl.h>

class OutputManager : public IOutputManager {
public:
  OutputManager(int address);
  int begin() override;
  void displayStatus(int code) override;
  void setStepIntervalLed(int n) override;
  void setPrintStopLed(int n) override;
  void setTime(unsigned long t) override;
  void setEnlarger(bool en) override;
  void click() override;
  void step() override;
private:
  Adafruit_MCP23X17* mcp;
  int address;
  LedSequence *stepInterval;
  LedSequence *printStop;
  LedControl *lc;
  unsigned long nextBuzzerOff = 0;
};

#endif
