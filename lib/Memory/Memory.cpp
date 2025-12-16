#include <Memory.h>
#include <../../include/defs.h>

Memory::Memory(int address) {
  this->address = address;
}

bool Memory::begin() {
#ifdef ENABLE_EEPROM_MEMORY
  this->ee = new I2C_eeprom(this->address, I2C_DEVICESIZE_24LC512);
  if (!this->ee->begin()) {
    return false;
  }
  if (!this->ee->isConnected()) {
    return false;
  }
#endif
  return true;
}

void Memory::write(int slot, Settings* settings) {
#ifdef ENABLE_EEPROM_MEMORY
  settings->magicNumber = MAGIC_NUMBER;
  this->ee->writeBlock(slot, (uint8_t *)settings, sizeof(Settings));
#else
  memcpy(&this->slots[slot], settings, sizeof(Settings));
#endif
}
  
void Memory::read(int slot, Settings* settings) {
#ifdef ENABLE_EEPROM_MEMORY
  this->ee->readBlock(slot, (uint8_t *)settings, sizeof(Settings));
#else
  memcpy(settings, &this->slots[slot], sizeof(Settings));
#endif
  if (settings->magicNumber != MAGIC_NUMBER) {
    settings->baseTime = 0;
    settings->stopIndex = 3;
    settings->stepIntervalIndex = 0;
  }
}