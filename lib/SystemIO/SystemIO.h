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
  void showNumberFloat(float num,
									int prec /*= -1*/,
									bool leading_zero /*= false*/,
									uint8_t length /*= 4*/,
									uint8_t pos /*= 0*/);
  void showString(char *str);
  uint8_t selectDigit(char digit);
};

#endif