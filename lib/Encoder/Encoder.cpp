#include <Encoder.h>
#include <Arduino.h>

// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Counter-clockwise step.
#define DIR_CCW 0x20

#define R_START 0x0
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

const unsigned char ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};

Encoder::Encoder(int pinA, int pinB, double min, double max, double delta)
{
    this->pinA = pinA;
    this->pinB = pinB;
    this->min = min;
    this->max = max;
    this->delta = delta;
}

void Encoder::begin()
{
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
}

void Encoder::step()
{
  // Grab state of input pins.
  unsigned char pinstate = ((0 ^ digitalRead(pinB)) << 1) | (0 ^ digitalRead(pinA));
  // Determine new state from the pins and state table.
  state = ttable[state & 0xf][pinstate];
  // Return emit bits, ie the generated event.

  double direction = 0;
  switch (state & 0x30) {
    case DIR_CW:
        direction = 1;
        break;
    case DIR_CCW:
        direction = -1;
        break;
  };

  value += (delta * direction);
  Serial.printf("v: %f\n", value);
}

double Encoder::getValue()
{
    return this->value;
}