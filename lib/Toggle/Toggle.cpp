#include <Toggle.h>
#include <Arduino.h>

Toggle::Toggle(int pin)
{
  this->pin = pin;
}

void Toggle::begin()
{
  pinMode(this->pin, INPUT_PULLUP);
}

bool Toggle::isOn()
{
  return digitalRead(this->pin) == LOW;
}