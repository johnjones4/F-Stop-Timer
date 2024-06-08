#ifndef SystemIO_h
#define SystemIO_h

#include <Adafruit_MCP23X17.h>
#include <Selector.h>
#include <Toggle.h>
#include <TM1637Display.h>


class SystemIO
{
public:
  SystemIO();
  bool begin();
  bool isPrintMode();
  bool isFocusMode();
  bool isStarted();
  double getBaseTimeSelector();
  double getStopDeltaSelector();
  int getPrintTimeSelector();
  void setLight(int mode);
  void printTime(unsigned long millis);
  void setBracketLight(int i);
private:
  Adafruit_MCP23X17** mcps;
  TM1637Display* display;
  Toggle* printMode;
  Toggle* focusMode;
  Toggle* startButton;
  Selector* baseTimeSelector;
  Selector* stopDeltaSelector;
  Selector* printTimeSelector;
  unsigned long lastStartPress = 0;
};

#endif