#include <PrintMode.h>
#include <timescale.h>
#include <../../include/values.h>

PrintMode::PrintMode(SystemIO* systemIo) : Mode(systemIo)
{
  
}

void PrintMode::step()
{
  // Timer is running and should be stopped
  if (this->stopAt > 0 && millis() >= stopAt) 
  {
    this->systemIo->setLight(LOW);
    this->showDefaultTime();
    this->stopAt = 0;
  } 

  // Timer is running and should continue
  else if (this->stopAt > 0 && millis() < stopAt) 
  {    
    this->systemIo->setLight(HIGH);
    unsigned long remaining = this->stopAt - millis();
    this->systemIo->printTime(remaining);
  }

  // Timer has not yet been started 
  else 
  {
    //TODO update display
    this->systemIo->setLight(LOW);
    unsigned long timeMillis = this->showDefaultTime();
    
    if (this->systemIo->isStarted())
    {
      this->stopAt = millis() + timeMillis;
      this->systemIo->setLight(HIGH);
    }
  }
}

unsigned long PrintMode::showDefaultTime()
{
  int offset = this->systemIo->getPrintTimeSelector();
  double baseTime = this->systemIo->getBaseTimeSelector();
  double delta = this->systemIo->getStopDeltaSelector();

  double time = generateTime(baseTime, delta, (double)(offset + PRINT_TIME_OFFSET));
  unsigned long timeMillis = (unsigned long)(time * 1000);

  this->systemIo->printTime(timeMillis);
  this->systemIo->setBracketLight(offset);
  
  return timeMillis;
}