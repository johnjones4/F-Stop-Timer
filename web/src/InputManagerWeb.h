#pragma once
#include <IInputManager.h>
#include <../../include/defs.h>

class InputManagerWeb : public IInputManager {
public:
  int begin() override;
  void step() override;
  bool isPressed(ButtonName b) override;
  DialDirection getDialDirection(DialName d) override;
  unsigned long getDialSpeed(DialName d) override;
  Mode getSelectedMode() override;

  // Called by exported C functions from JavaScript
  void pushButtonPress(int b);
  void pushDialTurn(int d, int dir);
  void setMode(int m);

private:
  bool buttonState[N_BUTTONS] = {};
  int  dialAccum[N_DIALS]     = {};
  int  dialDrained[N_DIALS]   = {};
  Mode mode = Focus;
};
