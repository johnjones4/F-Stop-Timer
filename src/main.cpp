#include <Arduino.h>
#include <Runtime.h>

Runtime rt;

void setup() {
  rt.begin();
}

void loop() {
  rt.step();
}
