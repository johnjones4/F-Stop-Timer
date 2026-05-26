#pragma once
// Stub: no I2C hardware in the web build.
struct WireStub {
  void begin() {}
  void beginTransmission(int) {}
  int  endTransmission() { return 0; }
};
inline WireStub Wire;
