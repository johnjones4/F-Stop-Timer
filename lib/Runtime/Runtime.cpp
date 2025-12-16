#include <Runtime.h>
#include <Wire.h>
#include <Arduino.h>
#include <../../include/defs.h>

const double stepIntervals[N_STEP_INTERVALS] = STEP_INTERVALS;
const int stops[N_STOPS] = STOPS;

int generateTimes(unsigned long *times, int nTimes, unsigned long baseTime, double stepInterval);
unsigned long generateTime(unsigned long baseTime, double stepInterval, int nStops);

void Runtime::begin() {
  Serial.begin(9600);
  Wire.begin();

  this->input = new InputManager(INPUT_ADDR);
  this->output = new OutputManager(OUTPUT_ADDR);
  this->memory = new Memory(EEPROM_ADDR);

  if (!this->input->begin()) {
    while (true) {}
  }
  if (!this->output->begin()) {
    while (true) {}
  }
  if (!this->memory->begin()) {
    while (true) {}
  }
#ifdef TEST_MODE
  this->start = 30000;
#else
  this->memory->read(0, &this->settings);
  this->lastMode = this->input->getSelectedMode();
  this->reset();
#endif
  Serial.println("Ready");
}

void Runtime::step() {
  this->input->step();
  this->output->step();
#ifdef TEST_MODE
  unsigned long now = millis();
  this->output->setTime(this->start);
  this->output->setPrintStopLed((now / 1000) % N_STOPS);
  this->output->setStepIntervalLed((now / 1000) % N_STEP_INTERVALS);
  this->output->setEnlarger(now / 1000 % 2);
  Direction bt = this->input->getDialDirection(BaseTime);
  switch (bt) {
  case CLOCKWISE: {
    this->start += 500;
    break;
  }
  case COUNTERCLOCKWISE: {
    this->start -= 500;
    break;
  }
  }
#else
  Mode nextMode = this->input->getSelectedMode();
  if (nextMode != this->lastMode || this->input->isPressed(Reset) || this->changedMem()) {
    this->reset();
  }
  if (this->start > 0) {
    this->runningTimer();
  } else if (this->input->isPressed(Start)) {
    this->start = millis();
    this->output->setEnlarger(true);
    this->runningTimer();
  } else if (this->changedBaseTime() || this->changedStepInterval() || this->changedPrintStop()) {
    this->reset();
  } else if (this->input->isPressed(MemSet)) {
    this->reset();
    this->writeMode = true;
  }
#endif
}

void Runtime::reset() {
  this->lastMode = this->input->getSelectedMode();
  this->writeMode = false;
  this->start = 0;
  this->currentTime = 0;
  this->nextClick = 0;
  switch (this->lastMode) {
  case Focus:
    this->output->setTime(0);
    this->output->setEnlarger(true);
    this->output->setPrintStopLed(-1);
    this->output->setStepIntervalLed(-1);
    break;
  case Test:
    this->output->setEnlarger(false);
    this->output->setStepIntervalLed(this->settings.stepIntervalIndex);
    this->output->setPrintStopLed(0);
    generateTimes(this->times, N_STOPS, this->settings.baseTime, stepIntervals[this->settings.stepIntervalIndex]);
    this->nTimes = N_STOPS;
    this->output->setTime(this->times[this->currentTime]);
    this->memory->write(0, &this->settings);
    break;
  case Print:
    this->output->setEnlarger(false);
    this->output->setStepIntervalLed(this->settings.stepIntervalIndex);
    this->output->setPrintStopLed(this->settings.stopIndex);
    this->times[0] = generateTime(this->settings.baseTime, stepIntervals[this->settings.stepIntervalIndex], stops[this->settings.stopIndex]);
    this->nTimes = 1;
    this->output->setTime(this->times[this->currentTime]);
    this->memory->write(0, &this->settings);
    break;
  }
}

bool Runtime::changedBaseTime() {
  if (this->lastMode == Focus) {
    return false;
  }
  Direction bt = this->input->getDialDirection(BaseTime);
  switch (bt) {
  case CLOCKWISE: {
    unsigned long next = this->settings.baseTime + 500;
    this->settings.baseTime = min(99500, next);
    return true;
  }
  case COUNTERCLOCKWISE: {
    unsigned long next = this->settings.baseTime - 500;
    this->settings.baseTime = max(0, next);
    return true;
  }
  default:
    return false;
  }
}

bool Runtime::changedStepInterval() {
  if (this->lastMode == Focus) {
    return false;
  }
  Direction bt = this->input->getDialDirection(StepInterval);
  switch (bt) {
  case CLOCKWISE: {
    int next = this->settings.stepIntervalIndex + 1;
    this->settings.stepIntervalIndex = min(N_STEP_INTERVALS, next);
    return true;
  }
  case COUNTERCLOCKWISE: {
    int next = this->settings.stepIntervalIndex - 1;
    this->settings.stepIntervalIndex = max(0, next);
    return true;
  }
  default:
    return false;
  }
}

bool Runtime::changedPrintStop() {
  if (this->lastMode != Print) {
    return false;
  }
  Direction bt = this->input->getDialDirection(PrintStop);
  switch (bt) {
  case CLOCKWISE: {
    int next = this->settings.stopIndex + 1;
    this->settings.stopIndex = min(N_STOPS, next);
    return true;
  }
  case COUNTERCLOCKWISE: {
    int next = this->settings.stopIndex - 1;
    this->settings.stopIndex = max(0, next);
    return true;
  }
  default:
    return false;
  }
}

bool Runtime::changedMem() {
  ButtonName mems[] = {Mem1, Mem2, Mem3};
  for (int i = 0; i < 3; i++) {
    if (this->input->isPressed(mems[i])) {
      int slot = i + 1;
      if (this->writeMode) {
        this->memory->write(slot, &this->settings);
      } else {
        this->memory->read(slot, &this->settings);
      }
      return true;
    }
  }
  return false;
}

void Runtime::runningTimer() {
  if (this->start == 0) {
    return;
  }
  unsigned long now = millis();
  unsigned long elapsed = millis() - this->start;
  if (elapsed >= this->times[this->currentTime]) {
    this->output->setEnlarger(false);
    this->currentTime = (this->currentTime + 1) % this->nTimes;
    this->output->setTime(this->times[this->currentTime]);
    this->start = 0;
    this->nextClick = 0;
    if (this->lastMode == Test) {
      this->output->setPrintStopLed(this->currentTime);
    }
  } else {
    unsigned long remaining = this->times[this->currentTime] - elapsed;
    this->output->setTime(remaining);
    if (this->nextClick != 0 && now > this->nextClick) {
      this->output->click();
      this->nextClick = now + 5000;
    }
  }
}

int generateTimes(unsigned long *times, int nTimes, unsigned long baseTime, double stepInterval)
{
  if (nTimes % 2 == 0) {
    return -1;
  }
  int base = -1 * (float)((nTimes - 1)/2);
  for (int i = 0; i < nTimes; i++) {
    unsigned long time = generateTime(baseTime, stepInterval, base + i);
    if (i > 0) {
      time = time - generateTime(baseTime, stepInterval, base + (i-1));
    }
    times[i] = time;
  }
  return 0;
}

unsigned long generateTime(unsigned long baseTime, double stepInterval, int nStops)
{
  double stops = (double)nStops * stepInterval;
  return baseTime * ((unsigned long)pow(2, stops));
}