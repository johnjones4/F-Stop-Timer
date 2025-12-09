#ifndef Button_h
#define Button_h

#include <Adafruit_MCP23X17.h>

class Button {
public:
  Button(Adafruit_MCP23X17* mcp, int pin);
  bool isPressed();
private:
  Adafruit_MCP23X17* mcp;
  int pin;
  unsigned long lastPress = 0;
};

#endif