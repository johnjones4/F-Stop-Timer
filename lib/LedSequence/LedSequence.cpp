#include <LedSequence.h>

LedSequence::LedSequence(Adafruit_MCP23X17* mcp, const int* pins, int nPins) {
  this->mcp = mcp;
  this->pins = (int*)malloc(sizeof(int) * nPins);
  memcpy(this->pins, pins, sizeof(int) * nPins);
  this->nPins = nPins;
  for (int i = 0; i < nPins; i++) {
    mcp->pinMode(pins[i], OUTPUT);
  }
}

void LedSequence::set(int n) {
  for (int i = 0; i < nPins; i++) {
    mcp->digitalWrite(pins[i], i == n ? HIGH : LOW);
  }
}