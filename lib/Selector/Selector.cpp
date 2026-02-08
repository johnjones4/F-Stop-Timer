#include <Selector.h>
#include <Arduino.h>

Selector::Selector(Adafruit_MCP23X17* mcp, const int* pins, int nPins)
{
  this->mcp = mcp;
  this->pins = (int*)malloc(sizeof(int) * nPins);
  memcpy(this->pins, pins, sizeof(int) * nPins);
  this->nPins = nPins;
  for (int i = 0; i < this->nPins; i++)
  {
    this->mcp->pinMode(this->pins[i], INPUT_PULLUP);
  }
}

int Selector::getSelectedIndex()
{
  for (int i = 0; i < this->nPins; i++) { 
    int status = this->mcp->digitalRead(this->pins[i]);
    if (status == LOW) {
      return i;
    }
  }
  return -1;
}