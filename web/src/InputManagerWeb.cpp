#include "InputManagerWeb.h"
#include <emscripten.h>

// Singleton pointer so exported C functions can reach the instance
static InputManagerWeb* g_input = nullptr;

int InputManagerWeb::begin() {
  g_input = this;
  return STATUS_OK;
}

void InputManagerWeb::step() {
  // Snapshot accumulated dial movement for this tick, then reset accumulator
  for (int i = 0; i < N_DIALS; i++) {
    dialDrained[i] = dialAccum[i];
    dialAccum[i]   = 0;
  }
}

bool InputManagerWeb::isPressed(ButtonName b) {
  if (b < 0 || b >= N_BUTTONS) return false;
  bool v = buttonState[b];
  buttonState[b] = false; // one-shot: consume on read
  return v;
}

DialDirection InputManagerWeb::getDialDirection(DialName d) {
  if (d < 0 || d >= N_DIALS) return DialDirection::NONE;
  int v = dialDrained[d];
  if (v > 0) return DialDirection::CLOCKWISE;
  if (v < 0) return DialDirection::COUNTERCLOCKWISE;
  return DialDirection::NONE;
}

unsigned long InputManagerWeb::getDialSpeed(DialName d) {
  // Return a fixed RPM-equivalent; acceleration is not simulated in web build
  return 1;
}

Mode InputManagerWeb::getSelectedMode() {
  return mode;
}

void InputManagerWeb::pushButtonPress(int b) {
  if (b >= 0 && b < N_BUTTONS) buttonState[b] = true;
}

void InputManagerWeb::pushDialTurn(int d, int dir) {
  if (d >= 0 && d < N_DIALS) dialAccum[d] += dir;
}

void InputManagerWeb::setMode(int m) {
  if (m >= 0 && m <= (int)None) mode = (Mode)m;
}

// ---- Exported C functions called from JavaScript ----

extern "C" {

EMSCRIPTEN_KEEPALIVE void web_button_press(int b) {
  if (g_input) g_input->pushButtonPress(b);
}

EMSCRIPTEN_KEEPALIVE void web_dial_turn(int d, int dir) {
  if (g_input) g_input->pushDialTurn(d, dir);
}

EMSCRIPTEN_KEEPALIVE void web_mode_select(int m) {
  if (g_input) g_input->setMode(m);
}

} // extern "C"
