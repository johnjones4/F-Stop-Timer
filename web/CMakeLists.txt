cmake_minimum_required(VERSION 3.20)
project(fstop_web)
set(CMAKE_CXX_STANDARD 17)

set(ROOT ${CMAKE_SOURCE_DIR}/..)

add_executable(fstop_web
  src/main_web.cpp
  src/InputManagerWeb.cpp
  src/OutputManagerWeb.cpp
  ${ROOT}/lib/Runtime/Runtime.cpp
  ${ROOT}/lib/Memory/Memory.cpp
  ${ROOT}/lib/fstop/fstop.c
)

target_include_directories(fstop_web PRIVATE
  # Shim headers come first so they shadow any hardware Arduino headers
  ${CMAKE_SOURCE_DIR}/shim

  # Interface headers (IInputManager.h, IOutputManager.h)
  ${ROOT}/lib/Interfaces
  ${ROOT}/lib

  # Library source directories (needed so <../../include/defs.h> resolves correctly
  # relative to each lib subdirectory that's on the include path)
  ${ROOT}/lib/Runtime
  ${ROOT}/lib/Memory
  ${ROOT}/lib/fstop

  # Canonical include directory
  ${ROOT}/include

  # Web stub implementations
  ${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(fstop_web PRIVATE WASM_BUILD=1)

set_target_properties(fstop_web PROPERTIES SUFFIX ".js")

target_link_options(fstop_web PRIVATE
  "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
  "-sEXPORTED_FUNCTIONS=['_main','_web_button_press','_web_dial_turn','_web_mode_select']"
  -sMODULARIZE=1
  -sEXPORT_NAME=FStopModule
  -sALLOW_MEMORY_GROWTH=1
  -lm
)
