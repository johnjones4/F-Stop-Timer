#include "OutputManagerWeb.h"
#include <emscripten.h>

// All output is routed through Module.onOutput callbacks registered by index.html.
// EM_ASM guards with a null-check so missing callbacks fail silently.

int OutputManagerWeb::begin() {
  return 0; // STATUS_OK
}

void OutputManagerWeb::displayStatus(int code) {
  EM_ASM({ if (Module.onOutput && Module.onOutput.displayStatus) Module.onOutput.displayStatus($0); }, code);
}

void OutputManagerWeb::setStepIntervalLed(int n) {
  EM_ASM({ if (Module.onOutput && Module.onOutput.setStepIntervalLed) Module.onOutput.setStepIntervalLed($0); }, n);
}

void OutputManagerWeb::setPrintStopLed(int n) {
  EM_ASM({ if (Module.onOutput && Module.onOutput.setPrintStopLed) Module.onOutput.setPrintStopLed($0); }, n);
}

void OutputManagerWeb::setTime(unsigned long t) {
  EM_ASM({ if (Module.onOutput && Module.onOutput.setTime) Module.onOutput.setTime($0); }, t);
}

void OutputManagerWeb::setEnlarger(bool en) {
  EM_ASM({ if (Module.onOutput && Module.onOutput.setEnlarger) Module.onOutput.setEnlarger($0); }, (int)en);
}

void OutputManagerWeb::click() {
  EM_ASM({ if (Module.onOutput && Module.onOutput.click) Module.onOutput.click(); });
}

void OutputManagerWeb::step() {
  // Nothing to poll — callbacks fire synchronously on demand
}
