#include <InputManager.h>
#include <../../include/defs.h>

const int modePins[N_MODES] = MODE_PINS;
const int dialPins[N_DIALS][2] = DIAL_PINS;

InputManager::InputManager(int address) {
  this->address = address;
}

bool InputManager::begin() {
  this->mcp = new Adafruit_MCP23X17();
  Serial.println("Starting input MCP");
  if (!this->mcp->begin_I2C(this->address)) {
    Serial.println("Failed to start input MCP");
    return false;
  }

  for (int i = 0; i < N_BUTTONS; i++) {
    this->buttons[i] = new Button(this->mcp, BUTTONS_BASE_PIN + i);
  }

  for (int i = 0; i < N_DIALS; i++) {
    this->encoders[i] = new RotaryEncoder(this->mcp, dialPins[i][0], dialPins[i][1]);
  }

  int *buffer = (int*)malloc(sizeof(int)*N_MODES);
  memcpy(buffer, modePins, sizeof(int)*N_MODES);
  this->modeSelector = new Selector(this->mcp, buffer, N_MODES);

  return true;
}

void InputManager::step() {
  for (int i = 0; i < N_DIALS; i++) {
    this->encoders[i]->step();
  }
}

bool InputManager::isPressed(ButtonName b) {
  if (b < 0 || b >= N_BUTTONS) {
    return false;
  }
  return this->buttons[b]->isPressed();
}

Direction InputManager::getDialDirection(DialName d) {
  if (d < 0 || d >= N_BUTTONS) {
    return NOROTATION;
  }
  return this->encoders[d]->getDirection();
}

Mode InputManager::getSelectedMode() {
  int s = this->modeSelector->getSelectedIndex();
  if (s < 0 || s >= N_MODES) {
    return Focus;
  }
  return (Mode)s;
}