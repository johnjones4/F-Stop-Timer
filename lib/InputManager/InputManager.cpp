#include <InputManager.h>

#define BUTTONS_BASE_PIN 0
#define DIALS_BASE_PIN 0

#define MODE_PIN 0

InputManager::InputManager(int address) {
  this->address = address;
}

bool InputManager::begin() {
  this->mcp = new Adafruit_MCP23X17();
  Serial.println("Starting MCP");
  // MCP23XXX_ADDR
  if (!this->mcp->begin_I2C(this->address)) {
    Serial.println("Failed to start input MCP");
    return false;
  }

  for (int i = 0; i < N_BUTTONS; i++) {
    this->buttons[i] = new Button(this->mcp, BUTTONS_BASE_PIN + i);
  }

  for (int i = 0; i < N_DIALS; i++) {
    this->encoders[i] = new RotaryEncoder(this->mcp, (BUTTONS_BASE_PIN + i) * 2, ((BUTTONS_BASE_PIN + i) * 2) + 1);
  }

  this->modeSelector = new Selector(this->mcp, MODE_PIN, N_MODES);

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