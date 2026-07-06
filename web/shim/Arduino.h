#pragma once
#include <chrono>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <algorithm>

using byte = uint8_t;

#define LOW          0
#define HIGH         1
#define INPUT_PULLUP 2
#define OUTPUT       1

#define MOSI 11
#define SCK  13

// Monotonic millisecond clock using chrono (matches emscripten_get_now() semantics)
inline unsigned long millis() {
  using namespace std::chrono;
  static auto epoch = steady_clock::now();
  return (unsigned long)duration_cast<milliseconds>(steady_clock::now() - epoch).count();
}

// No-op: startup LED animation is skipped in web build
inline void delay(unsigned long) {}

// min/max as templates (Arduino.h defines these as macros; avoid conflicts)
template<typename T> T min(T a, T b) { return a < b ? a : b; }
template<typename T> T max(T a, T b) { return a > b ? a : b; }

struct SerialStub {
  void begin(int) {}
  void println(const char* s) { printf("%s\n", s); }
  void println(int n)         { printf("%d\n", n); }
  void print(const char* s)   { printf("%s", s); }
  void print(int n, int)      { printf("%x", n); }
  void print(int n)           { printf("%d", n); }
};

inline SerialStub Serial;
