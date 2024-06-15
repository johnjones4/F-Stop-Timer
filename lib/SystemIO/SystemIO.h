#ifndef SystemIO_h
#define SystemIO_h

#include <Adafruit_MCP23X17.h>
#include <Selector.h>
#include <Toggle.h>
#include <Adafruit_LEDBackpack.h>
#include <RotaryEncoder.h>

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
  void step();
private:
  Adafruit_MCP23X17* mcp;
  Adafruit_7segment * display;
  RotaryEncoder *encoder;
  Toggle* printMode;
  Toggle* focusMode;
  Toggle* startButton;
  Selector* stopDeltaSelector;
  Selector* printTimeSelector;
  double baseTime = 15;
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