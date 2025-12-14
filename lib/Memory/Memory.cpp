#include <Memory.h>

#define MAGIC_NUMBER 1989

Memory::Memory(int address) {
  this->address = address;
}

bool Memory::begin() {
  this->ee = new I2C_eeprom(this->address, I2C_DEVICESIZE_24LC512);
  if (!this->ee->begin()) {
    return false;
  }
  if (!this->ee->isConnected()) {
    return false;
  }
  return true;
}

void Memory::write(int slot, Settings* settings) {
  settings->magicNumber = MAGIC_NUMBER;
  this->ee->writeBlock(slot, (uint8_t *)settings, sizeof(Settings));
}
  
void Memory::read(int slot, Settings* settings) {
  this->ee->readBlock(slot, (uint8_t *)settings, sizeof(Settings));
  if (settings->magicNumber != MAGIC_NUMBER) {
    settings->baseTime = 0;
    settings->stopIndex = 3;
    settings->stepIntervalIndex = 0;
  }
}