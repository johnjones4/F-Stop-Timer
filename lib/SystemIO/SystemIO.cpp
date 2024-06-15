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
  this->mcp = new Adafruit_MCP23X17();
  this->display = new Adafruit_7segment();
  
  this->printMode = new Toggle(MODE_PIN);
  this->focusMode = new Toggle(FOCUS_PIN);
  this->startButton = new Toggle(START_PIN);
  
  // int *baseTimePins = (int*)malloc(sizeof(int) * N_BASE_TIMES);
  // for (int i = 0; i < N_BASE_TIMES; i++)
  // {
  //   baseTimePins[i] = BASE_TIME_FIRST_PIN + i;
  // }
  // this->baseTimeSelector = new Selector(mcps[BASE_TIME_CHIP], baseTimePins, N_BASE_TIMES);
  
  int stopDeltaPins[N_STOP_DELTAS] = DELTA_PINS;
  this->stopDeltaSelector = new Selector(NULL, stopDeltaPins, N_STOP_DELTAS);

  int *printTimePins = (int*)malloc(sizeof(int) * N_STEPS);
  for (int i = 0; i < N_STEPS; i++)
  {
    printTimePins[i] = PRINT_TIME_FIRST_PIN + i;
  }
  this->printTimeSelector = new Selector(mcp, printTimePins, N_STEPS);
}

bool SystemIO::begin()
{
  this->encoder = new RotaryEncoder(TIME_CHANNEL_A_PIN, TIME_CHANNEL_B_PIN, RotaryEncoder::LatchMode::TWO03);

  Serial.println("Starting MCP");
  if (!this->mcp->begin_I2C(MCP23XXX_ADDR)) {
    Serial.println("Failed to start MCP");
    return false;
  }

  Serial.println("Setting up bracket lights");
  for (int i = 0; i < N_STEPS; i++) {
    mcp->pinMode(STEP_FIRST_PIN + i, OUTPUT);
    mcp->digitalWrite(STEP_FIRST_PIN + i, HIGH);
    delay(100);
    mcp->digitalWrite(STEP_FIRST_PIN + i, LOW);
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
  // this->baseTimeSelector->begin();
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
  return baseTime;
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
    mcp->digitalWrite(STEP_FIRST_PIN + i, i == ii ? HIGH : LOW);
  }
}

void SystemIO::step() {
  this->encoder->tick();
  this->baseTime += (0.5 * (double)this->encoder->getPosition());
}