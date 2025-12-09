#include <Button.h>

#define DEBOUNCE 200

Button::Button(Adafruit_MCP23X17* mcp, int pin) {
  this->mcp = mcp;
  this->pin = pin;
  this->mcp->pinMode(this->pin, INPUT_PULLUP);
}

bool Button::isPressed() {
  unsigned long now = millis();
  if (this->mcp->digitalRead(this->pin) == LOW && this->lastPress + DEBOUNCE < now) {
    lastPress = now;
    return true;
  } else {
    return false;
  }
}