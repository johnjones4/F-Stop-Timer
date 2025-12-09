#ifndef InputManager_h
#define InputManager_h

#include <Arduino.h>
#include <Button.h>
#include <Adafruit_MCP23X17.h>
#include <RotaryEncoder.h>
#include <Selector.h>

typedef enum {
  MemSet  = 0,
  Mem1    = 1,
  Mem2    = 2,
  Mem3    = 3,
  Mem4    = 4,
  Start   = 5,
} ButtonName;

#define N_BUTTONS 6

typedef enum {
  BaseTime    = 0,
  TimeDelta   = 1,
  PrintStop   = 2,
} DialName;

#define N_DIALS 3

typedef enum {
  Focus = 0,
  Test  = 1,
  Print = 2,
} Mode;

#define N_MODES 3

class InputManager {
public:
  InputManager(int address);
  bool begin();
  void step();
  bool isPressed(ButtonName b);
  Direction getDialDirection(DialName d);
  Mode getSelectedMode();
private:
  int address;
  Adafruit_MCP23X17* mcp;
  Button* buttons[N_BUTTONS];
  RotaryEncoder* encoders[N_DIALS];
  Selector* modeSelector;
};

#endif