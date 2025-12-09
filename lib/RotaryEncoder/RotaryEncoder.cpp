#include <RotaryEncoder.h>

const int8_t KNOBDIR[] = {
  0, -1, 1, 0,
  1, 0, 0, -1,
  -1, 0, 0, 1,
  0, 1, -1, 0};

RotaryEncoder::RotaryEncoder(Adafruit_MCP23X17* mcp, int pin1, int pin2)
{
  // Remember Hardware Setup
  _pin1 = pin1;
  _pin2 = pin2;

  // Setup the input pins and turn on pullup resistor
  mcp->pinMode(pin1, INPUT_PULLUP);
  mcp->pinMode(pin2, INPUT_PULLUP);

  // when not started in motion, the current state of the encoder should be 3
  int sig1 = mcp->digitalRead(_pin1);
  int sig2 = mcp->digitalRead(_pin2);
  _oldState = sig1 | (sig2 << 1);

  // start with position 0;
  _position = 0;
  _positionExt = 0;
  _positionExtPrev = 0;
}

Direction RotaryEncoder::getDirection()
{
  Direction ret = Direction::NOROTATION;

  if (_positionExtPrev > _positionExt) {
    ret = Direction::COUNTERCLOCKWISE;
    _positionExtPrev = _positionExt;
  } else if (_positionExtPrev < _positionExt) {
    ret = Direction::CLOCKWISE;
    _positionExtPrev = _positionExt;
  } else {
    ret = Direction::NOROTATION;
    _positionExtPrev = _positionExt;
  }

  return ret;
}

void RotaryEncoder::step() {
  int sig1 = this->mcp->digitalRead(_pin1);
  int sig2 = this->mcp->digitalRead(_pin2);
  int8_t thisState = sig1 | (sig2 << 1);

  if (_oldState != thisState) {
    _position += KNOBDIR[thisState | (_oldState << 2)];
    _oldState = thisState;

    if ((thisState == 0) || (thisState == 3)) {
      // The hardware has 2 steps with a latch on the input state 0 and 3
      _positionExt = _position >> 1;
      _positionExtTimePrev = _positionExtTime;
      _positionExtTime = millis();
    }
  } // if
}