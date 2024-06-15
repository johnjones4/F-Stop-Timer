#include <Selector.h>
#include <Arduino.h>

Selector::Selector(Adafruit_MCP23X17* mcp, int* pins, int nPins)
{
  this->mcp = mcp;
  this->pins = pins;
  this->nPins = nPins;
}

void Selector::begin()
{
  for (int i = 0; i < this->nPins; i++)
  {
    if (this->mcp != NULL) 
    {
      this->mcp->pinMode(this->pins[i], INPUT_PULLUP);
    } 
    else 
    {
      pinMode(this->pins[i], INPUT_PULLUP);
    }
  }
}

int Selector::getSelectedIndex()
{
  for (int i = 0; i < this->nPins; i++) { 
    int status = 0;
    if (this->mcp != NULL)
    {
      status = this->mcp->digitalRead(this->pins[i]);
    } else {
      status = digitalRead(this->pins[i]);
    }
    if (status == LOW) {
      return i;
    }
  }
  return -1;
}