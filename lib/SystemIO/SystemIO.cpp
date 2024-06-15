#include <SystemIO.h>

#include <../../include/pins.h>
#include <../../include/values.h>
#include <math.h>
#include <Arduino.h>

double stopDeltas[N_STOP_DELTAS] = {
  0.25,
  1.0/3.0,
  0.5,
  1
};

SystemIO::SystemIO()
{
  this->mcps = (Adafruit_MCP23X17**)malloc(sizeof(Adafruit_MCP23X17*) * N_MCPS);
  for (int i = 0; i < N_MCPS; i++) {
    this->mcps[i] = new Adafruit_MCP23X17();
  }
  
  this->display = new Adafruit_7segment();
  
  this->printMode = new Toggle(MODE_PIN);
  this->focusMode = new Toggle(FOCUS_PIN);
  this->startButton = new Toggle(START_PIN);
  
  int *baseTimePins = (int*)malloc(sizeof(int) * N_BASE_TIMES);
  for (int i = 0; i < N_BASE_TIMES; i++)
  {
    baseTimePins[i] = BASE_TIME_FIRST_PIN + i;
  }
  this->baseTimeSelector = new Selector(mcps[BASE_TIME_CHIP], baseTimePins, N_BASE_TIMES);
  
  int *stopDeltaPins = (int*)malloc(sizeof(int) * N_STOP_DELTAS);
  for (int i = 0; i < N_STOP_DELTAS; i++)
  {
    stopDeltaPins[i] = STOP_DELTA_FIRST_PIN + i;
  }
  this->stopDeltaSelector = new Selector(mcps[DELTA_CHIP], stopDeltaPins, N_STOP_DELTAS);

  int *printTimePins = (int*)malloc(sizeof(int) * N_STEPS);
  for (int i = 0; i < N_STEPS; i++)
  {
    printTimePins[i] = PRINT_TIME_FIRST_PIN + i;
  }
  this->printTimeSelector = new Selector(mcps[PRINT_CHIP], printTimePins, N_STEPS);
}

bool SystemIO::begin()
{
  Serial.println("Starting MCP");
  for (int i = 0; i < N_MCPS; i++) {
    if (!this->mcps[i]->begin_I2C(MCP23XXX_ADDR + i)) {
      Serial.printf("Failed to start MCP #%d\n", i);
      return false;
    }
  }

  Serial.println("Setting up bracket lights");
  for (int i = 0; i < N_STEPS; i++) {
    mcps[STEP_CHIP]->pinMode(STEP_FIRST_PIN + i, OUTPUT);
    mcps[STEP_CHIP]->digitalWrite(STEP_FIRST_PIN + i, HIGH);
    delay(500);
    mcps[STEP_CHIP]->digitalWrite(STEP_FIRST_PIN + i, LOW);
  }

  Serial.println("Starting display");
  if (!this->display->begin(DISPLAY_ADDRESS))
  {
    Serial.println("Failed to start display");
    return false;
  }
  this->display->setBrightness(15);

  Serial.println("Setting up I/O");
  pinMode(LIGHT_PIN, OUTPUT);
  this->printMode->begin();
  this->focusMode->begin();
  this->startButton->begin();
  this->baseTimeSelector->begin();
  this->stopDeltaSelector->begin();
  this->printTimeSelector->begin();
  
  return true;
}

bool SystemIO::isPrintMode()
{
  return this->printMode->isOn();
}

bool SystemIO::isFocusMode()
{
  return this->focusMode->isOn();
}

bool SystemIO::isStarted()
{
  bool on = this->startButton->isOn();
  unsigned long now = millis();
  if (on && now - lastStartPress > DEBOUNCE) {
    lastStartPress = now;
    return true;
  }
  return false;
}

double SystemIO::getBaseTimeSelector()
{
  double i = (double)this->baseTimeSelector->getSelectedIndex();
  return pow(2, i) * BASE_TIME;
}

double SystemIO::getStopDeltaSelector()
{
  int i = this->stopDeltaSelector->getSelectedIndex();
  if (i < 0 || i >= N_STOP_DELTAS) {
    return -1;
  }
  return stopDeltas[i];
}

int SystemIO::getPrintTimeSelector()
{
  return this->printTimeSelector->getSelectedIndex();
}

void SystemIO::setLight(int mode)
{
  digitalWrite(LIGHT_PIN, mode);
}

void SystemIO::printTime(unsigned long millis)
{
  double seconds = (double)millis / 1000;
  this->display->printFloat(seconds, 1, 10);
  this->display->writeDisplay();
}

void SystemIO::setBracketLight(int ii)
{
  for (int i = 0; i < N_STEPS; i++) {
    mcps[STEP_CHIP]->digitalWrite(STEP_FIRST_PIN + i, i == ii ? HIGH : LOW);
  }
}