#ifndef InputManager_h
#define InputManager_h

#include <Arduino.h>
#include <Button.h>
#include <Adafruit_MCP23X17.h>
#include <RotaryEncoder.h>
#include <Selector.h>
#include <../../include/defs.h>

typedef enum {
  Reset   = 0,
  MemSet  = 1,
  Mem1    = 2,
  Mem2    = 3,
  Mem3    = 4,
  Start   = 5,
} ButtonName;

typedef enum {
  BaseTime      = 0,
  StepInterval  = 1,
  PrintStop     = 2,
} DialName;


typedef enum {
  Focus = 0,
  Test  = 1,
  Print = 2,
  None = 3,
} Mode;

class InputManager {
public:
  InputManager(int address);
  bool begin();
  void step();
  bool isPressed(ButtonName b);
  RotaryEncoder::Direction getDialDirection(DialName d);
  unsigned long getDialSpeed(DialName d);
  Mode getSelectedMode();
private:
  int address;
  Adafruit_MCP23X17* mcp;
  Button* buttons[N_BUTTONS];
  RotaryEncoder* encoders[N_DIALS];
  Selector* modeSelector;
};

#endif