#include <LedSequence.h>

LedSequence::LedSequence(Adafruit_MCP23X17* mcp, int pin, int nPins) {
  this->mcp = mcp;
  this->pin = pin;
  this->nPins = nPins;
  for (int i = 0; i < nPins; i++) {
    mcp->pinMode(pin + i, OUTPUT);
  }
}

void LedSequence::set(int n) {
  for (int i = 0; i < nPins; i++) {
    mcp->digitalWrite(pin + i, i == n ? HIGH : LOW);
  }
}