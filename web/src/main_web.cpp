#include <emscripten.h>
#include <Runtime.h>
#include "InputManagerWeb.h"
#include "OutputManagerWeb.h"

static Runtime          rt;
static InputManagerWeb  inputMgr;
static OutputManagerWeb outputMgr;

static void loop_step() {
  rt.step();
}

int main() {
  rt.begin(&inputMgr, &outputMgr);
  // 0 fps = use requestAnimationFrame; simulate_infinite_loop=1 keeps control in emscripten
  emscripten_set_main_loop(loop_step, 0, 1);
  return 0;
}
