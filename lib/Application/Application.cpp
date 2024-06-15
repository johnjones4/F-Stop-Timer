#include <Application.h>
#include <Arduino.h>

Application::Application()
{
  this->systemIo = new SystemIO();
  this->testMode = new TestMode(this->systemIo);
  this->printMode = new PrintMode(this->systemIo);
}

void Application::begin()
{
  Serial.begin(9600);
  if (!this->systemIo->begin()) {
    Serial.println("Failed to start system IO");
    while (true) {}
  }
  Serial.println("Ready");
}
  
void Application::step()
{
  this->systemIo->step();
  
  // Focus light overrides all
  if (this->systemIo->isFocusMode()) {
    this->systemIo->setLight(HIGH);

  // Print mode on
  } else if (this->systemIo->isPrintMode()) {
    this->printMode->step();

  // Default to test mode
  } else {
    this->testMode->step();

  }
}