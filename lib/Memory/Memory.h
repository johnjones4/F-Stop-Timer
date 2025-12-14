#ifndef Memory_h
#define Memory_h

#include <I2C_eeprom.h>

typedef struct {
  int magicNumber;
  float baseTime;
  int stepIntervalIndex;
  int stopIndex;
} Settings;

class Memory {
public:
  Memory(int address);
  bool begin();
  void write(int slot, Settings* settings);
  void read(int slot, Settings* settings);
private:
  int address;
  I2C_eeprom* ee;
};

#endif