#ifndef Selector_h
#define Selector_h

#include <Adafruit_MCP23X17.h>

class Selector
{
public:
  Selector(Adafruit_MCP23X17* mcp, int* pins, int nPins);
  void begin();
  int getSelectedIndex();
private:
  Adafruit_MCP23X17* mcp;
  int* pins;
  int nPins;
};

#endif