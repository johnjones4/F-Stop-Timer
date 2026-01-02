#ifndef Button_h
#define Button_h

#include <Adafruit_MCP23X17.h>

class Button {
public:
  Button(Adafruit_MCP23X17* mcp, uint8_t pin, uint8_t activeState);
  bool isPressed();
private:
  Adafruit_MCP23X17* mcp;
  uint8_t pin;
  uint8_t activeState;
  unsigned long lastPress = 0;
};

#endif