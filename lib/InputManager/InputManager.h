#ifndef InputManager_h
#define InputManager_h

#include <Arduino.h>
#include <IInputManager.h>
#include <Button.h>
#include <Adafruit_MCP23X17.h>
#include <RotaryEncoder.h>
#include <Selector.h>
#include <../../include/defs.h>

class InputManager : public IInputManager {
public:
  InputManager(int address);
  int begin() override;
  void step() override;
  bool isPressed(ButtonName b) override;
  DialDirection getDialDirection(DialName d) override;
  unsigned long getDialSpeed(DialName d) override;
  Mode getSelectedMode() override;
private:
  int address;
  Adafruit_MCP23X17* mcp;
  Button* buttons[N_BUTTONS];
  RotaryEncoder* encoders[N_DIALS];
  Selector* modeSelector;
};

#endif
