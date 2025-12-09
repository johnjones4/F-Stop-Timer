#ifndef Memory_h
#define Memory_h

#include <I2C_eeprom.h>

typedef struct {
  int magicNumber;
  float baseTime;
  float stopInterval;
  int stop;
} Settings;

class Memory {
  Memory(int address);
  bool begin();
  void write(int slot, Settings* settings);
  void read(int slot, Settings* settings);
private:
  int address;
  I2C_eeprom* ee;
};

#endif