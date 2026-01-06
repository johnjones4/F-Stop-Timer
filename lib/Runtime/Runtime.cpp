#include <Runtime.h>
#include <Wire.h>
#include <Arduino.h>
#include <../../include/defs.h>
#include <Wire.h>

const double stepIntervals[N_STEP_INTERVALS] = STEP_INTERVALS;
const int stops[N_STOPS] = STOPS;

int generateTimes(unsigned long *times, int nTimes, unsigned long baseTime, double stepInterval);
unsigned long generateTime(unsigned long baseTime, double stepInterval, int nStops);

void Runtime::begin() {
  Serial.begin(9600);
  Wire.begin();

#ifdef SCAN_I2C
  this->scanI2C();
#endif

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
  this->afterRead();
  this->lastMode = this->input->getSelectedMode();
  this->reset();
#endif
  Serial.println("Ready");
  this->output->click();
  // while (1) {}
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
  Mode nextMode = this->input->getSelectedMode();
  if (nextMode == None) {
    return;
  }
  this->lastMode = nextMode;
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
    this->output->setTime(this->settings.baseTime);
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
  RotaryEncoder::Direction bt = this->input->getDialDirection(BaseTime);
  unsigned long speed = this->input->getDialSpeed(BaseTime);
  unsigned long inc = speed * 10;
  switch (bt) {
  case RotaryEncoder::Direction::CLOCKWISE: {
    unsigned long next = this->settings.baseTime + inc;
    this->settings.baseTime = min(99500, next);
    return true;
  }
  case RotaryEncoder::Direction::COUNTERCLOCKWISE: {
    if (inc > this->settings.baseTime) {
      this->settings.baseTime = 0;
    } else {
      this->settings.baseTime = this->settings.baseTime - inc;
    }    
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
  RotaryEncoder::Direction bt = this->input->getDialDirection(StepInterval);
  switch (bt) {
  case RotaryEncoder::Direction::CLOCKWISE: {
    int next = this->stepIntervalPositionBuffer + 1;
    this->stepIntervalPositionBuffer = min(N_STEP_INTERVALS * POSITION_BUFFER_SIZE, next);
    this->settings.stepIntervalIndex = this->stepIntervalPositionBuffer / POSITION_BUFFER_SIZE;
    return true;
  }
  case RotaryEncoder::Direction::COUNTERCLOCKWISE: {
    int next = this->stepIntervalPositionBuffer - 1;
    this->stepIntervalPositionBuffer = max(0, next);
    this->settings.stepIntervalIndex = this->stepIntervalPositionBuffer / POSITION_BUFFER_SIZE;
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
  RotaryEncoder::Direction bt = this->input->getDialDirection(PrintStop);
  switch (bt) {
  case RotaryEncoder::Direction::CLOCKWISE: {
    int next = this->printStopPositionBuffer + 1;
    this->printStopPositionBuffer = min(N_STOPS * POSITION_BUFFER_SIZE, next);
    this->settings.stopIndex = this->printStopPositionBuffer / POSITION_BUFFER_SIZE;
    return true;
  }
  case RotaryEncoder::Direction::COUNTERCLOCKWISE: {
    int next = this->printStopPositionBuffer - 1;
    this->printStopPositionBuffer = max(0, next);
    this->settings.stopIndex = this->printStopPositionBuffer / POSITION_BUFFER_SIZE;
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
        this->afterRead();
      }
      return true;
    }
  }
  return false;
}

void Runtime::afterRead() {
  this->printStopPositionBuffer = (this->settings.stopIndex * POSITION_BUFFER_SIZE) + (POSITION_BUFFER_SIZE / 2);
  this->stepIntervalPositionBuffer = (this->settings.stepIntervalIndex * POSITION_BUFFER_SIZE) + (POSITION_BUFFER_SIZE / 2);
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
    if (this->currentTime == 0) {
      this->reset();
    } else if (this->lastMode == Test) {
      this->output->setPrintStopLed(this->currentTime);
    }
  } else {
    unsigned long remaining = this->times[this->currentTime] - elapsed;
    this->output->setTime(remaining);
    if (this->input->isPressed(Start)) {
      this->times[this->currentTime] = remaining;
      this->start = 0;
      this->output->setEnlarger(false);
    }
    if (now > this->nextClick) {
      this->output->click();
      this->nextClick = now + 1000;
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
  return (unsigned long)((double)baseTime * pow(2, stops));
}

void Runtime::scanI2C() {
   byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}