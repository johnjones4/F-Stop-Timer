#ifndef LedSequence_h
#define LedSequence_h

#include <Adafruit_MCP23X17.h>

class LedSequence {
public:
  LedSequence(Adafruit_MCP23X17* mcp, const int* pins, int nPins);
  void set(int n);
private:
  Adafruit_MCP23X17* mcp;
  int* pins;
  int nPins;
};

#endif