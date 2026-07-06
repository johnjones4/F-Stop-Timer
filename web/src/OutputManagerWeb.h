#pragma once
#include <IOutputManager.h>

class OutputManagerWeb : public IOutputManager {
public:
  int begin() override;
  void displayStatus(int code) override;
  void setStepIntervalLed(int n) override;
  void setPrintStopLed(int n) override;
  void setTime(unsigned long t) override;
  void setEnlarger(bool en) override;
  void click() override;
  void step() override;
};
