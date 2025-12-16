#ifndef Memory_h
#define Memory_h

#include <I2C_eeprom.h>
#include <../../include/defs.h>

typedef struct {
  int magicNumber;
  unsigned long baseTime;
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
#ifdef ENABLE_EEPROM_MEMORY
  I2C_eeprom* ee;
#else
  Settings slots[4];
#endif
};

#endif