#include <Button.h>

#define DEBOUNCE 500

Button::Button(Adafruit_MCP23X17* mcp, uint8_t pin, uint8_t activeState) {
  this->mcp = mcp;
  this->pin = pin;
  this->activeState = activeState;
  this->mcp->pinMode(this->pin, INPUT_PULLUP);
}

bool Button::isPressed() {
  unsigned long now = millis();
  if (this->mcp->digitalRead(this->pin) == this->activeState && this->lastPress + DEBOUNCE < now) {
    lastPress = now;
    return true;
  } else {
    return false;
  }
}