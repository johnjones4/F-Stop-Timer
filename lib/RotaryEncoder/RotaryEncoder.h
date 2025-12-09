#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#include <Adafruit_MCP23X17.h>

typedef enum {
  NOROTATION = 0,
  CLOCKWISE = 1,
  COUNTERCLOCKWISE = -1
} Direction;

class RotaryEncoder {
public:
  RotaryEncoder(Adafruit_MCP23X17* mcp, int pin1, int pin2);
  void step();
  Direction getDirection();
private:
  Adafruit_MCP23X17* mcp;
  int _pin1, _pin2; // Arduino pins used for the encoder.
  volatile int8_t _oldState;
  volatile long _position;        // Internal position (4 times _positionExt)
  volatile long _positionExt;     // External position
  volatile long _positionExtPrev; // External position (used only for direction checking)
  unsigned long _positionExtTime;     // The time the last position change was detected.
  unsigned long _positionExtTimePrev; // The time the previous position change was detected.
};

#endif