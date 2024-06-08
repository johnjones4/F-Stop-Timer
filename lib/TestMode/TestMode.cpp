#include <TestMode.h>
#include <../../include/values.h>
#include <timescale.h>

TestMode::TestMode(SystemIO* systemIo) : Mode(systemIo)
{
  this->times = (double*)malloc(sizeof(double) * N_STEPS);
}

void TestMode::step()
{
  if (this->stopAt > 0 && millis() >= stopAt) 
  {
    this->systemIo->setLight(LOW);

    this->timesIndex++;
    if (!nextTime(false)) {
      this->showDefaultTime();
      this->stopAt = 0;     
      this->timesIndex = -1; 
    }
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
    this->systemIo->setLight(LOW);
    this->showDefaultTime();

    if (this->systemIo->isStarted())
    {
      if (this->timesIndex < 0)
      {
        this->timesIndex = 0;
        double baseTime = this->systemIo->getBaseTimeSelector();
        double delta = this->systemIo->getStopDeltaSelector();
        generateTimes(this->times, N_STEPS, baseTime, delta);
      }

      if (this->nextTime(true)) {
        this->systemIo->setLight(HIGH);
        this->systemIo->setBracketLight(this->timesIndex);
      }
    }
  }
}

bool TestMode::nextTime(bool start)
{
  if (this->timesIndex < 0 || this->timesIndex >= N_STEPS) {
    return false;
  }
  double time = this->times[this->timesIndex];
  unsigned long timeMillis = (unsigned long)(time * 1000);
  if (start) {
    this->stopAt = millis() + timeMillis;
  }
  this->systemIo->printTime(timeMillis);
  this->systemIo->setBracketLight(-1);
  return true;
}

unsigned long TestMode::showDefaultTime()
{
  double baseTime = this->systemIo->getBaseTimeSelector();
  unsigned long timeMillis = (unsigned long)(baseTime * 1000);
  this->systemIo->printTime(timeMillis);
  return timeMillis;
}