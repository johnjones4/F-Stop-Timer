#include <Selector.h>

Selector::Selector(Adafruit_MCP23X17* mcp, int* pins, int nPins)
{
  this->mcp = mcp;
  this->pins = pins;
  this->nPins = nPins;
}

void Selector::begin()
{
  for (int i = 0; i < this->nPins; i++) {
    this->mcp->pinMode(this->pins[i], INPUT_PULLUP);
  }
}

int Selector::getSelectedIndex()
{
  for (int i = 0; i < this->nPins; i++) { 
    if (this->mcp->digitalRead(this->pins[i]) == LOW) {
      return i;
    }
  }
  return -1;
}