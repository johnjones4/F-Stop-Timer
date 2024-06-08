#include <Arduino.h>
#include <Application.h>

Application app;

void setup() {
  app.begin();
}

void loop() {
  app.step();
}
