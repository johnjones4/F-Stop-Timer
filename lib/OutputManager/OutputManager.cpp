#include <OutputManager.h>
#include <../../include/defs.h>

const int stepIntervalPins[N_STEP_INTERVALS] = STEP_INTERVAL_PINS;
const int printStopPins[N_STOPS] = PRINT_STOP_PINS;

int getDigitAtPos(double value, int n) {
  // Work with positive values only
  if (value < 0) value = -value;

  // Remove fractional part
  unsigned long long intPart = (unsigned long long)value;

  // Count digits
  unsigned long long temp = intPart;
  int count = 0;
  do {
      temp /= 10;
      count++;
  } while (temp > 0);

  // Out of range?
  if (n < 1 || n > count) return -1;

  // Compute divisor to isolate the digit
  unsigned long long divisor = 1;
  for (int i = 0; i < (count - n); i++) {
      divisor *= 10;
  }

  // Extract digit
  int digit = (int)((intPart / divisor) % 10);
  return digit;
}

OutputManager::OutputManager(int address) {
  this->address = address;
}

bool OutputManager::begin() {
  this->mcp = new Adafruit_MCP23X17();
  Serial.println("Starting output MCP");
  //MCP23XXX_ADDR
  if (!this->mcp->begin_I2C(this->address)) {
    Serial.println("Failed to start input MCP");
    return false;
  }
  this->mcp->pinMode(ENLARGER_PIN, OUTPUT);
  this->mcp->pinMode(BUZZER_PIN, OUTPUT);

  this->stepInterval = new LedSequence(this->mcp, stepIntervalPins, N_STEP_INTERVALS);
  this->printStop = new LedSequence(this->mcp, printStopPins, N_STOPS);
  this->lc = new LedControl(MOSI, SCK, DISPLAY_CS_PIN, 1);
  this->lc->shutdown(0,false);
  this->lc->setIntensity(0,8);
  this->lc->clearDisplay(0);
  return true;
}

void OutputManager::step() {
  if (this->nextBuzzerOff > 0 && millis() > this->nextBuzzerOff) {
    this->mcp->digitalWrite(BUZZER_PIN, LOW);  
    this->nextBuzzerOff = 0;
  }
}

void OutputManager::click() {
  this->mcp->digitalWrite(BUZZER_PIN, HIGH);
  this->nextBuzzerOff = millis() + 50;
}

void OutputManager::setStepIntervalLed(int n) {
  this->stepInterval->set(n);
}

void OutputManager::setPrintStopLed(int n) {
  this->printStop->set(n);
}

void OutputManager::setTime(unsigned long t) {
  float value = ((float)t) / 1000.0f;

  if (value < 0.0f) value = 0.0f;
  if (value > 99.9f) value = 99.9f;

  // Convert to fixed 1-decimal representation
  int scaled = (int)(value * 10 + 0.5f);  // e.g., 12.34 → 123

  int ones  = (scaled / 10) % 10;  // digit before decimal
  int tens  = (scaled / 100);      // leftmost digit
  int tenths = scaled % 10;        // digit after decimal

  // Rightmost digit: tenths (no decimal point)
  this->lc->setDigit(0, 2, tenths, false);

  // Middle digit: ones, WITH decimal point
  this->lc->setDigit(0, 1, ones, true);

  // Leftmost digit: tens (blank if zero)
  if (tens > 0) {
  this->lc->setDigit(0, 0, tens, false);
  } else {
  // Blank leading digit
  this->lc->setChar(0, 0, ' ', false);
  }
}

void OutputManager::setEnlarger(bool en) {
  this->mcp->digitalWrite(ENLARGER_PIN, en ? HIGH : LOW);
}