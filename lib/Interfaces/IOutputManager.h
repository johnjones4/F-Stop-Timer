#ifndef IOutputManager_h
#define IOutputManager_h

class IOutputManager {
public:
  virtual ~IOutputManager() {}
  virtual int begin() = 0;
  virtual void displayStatus(int code) = 0;
  virtual void setStepIntervalLed(int n) = 0;
  virtual void setPrintStopLed(int n) = 0;
  virtual void setTime(unsigned long t) = 0;
  virtual void setEnlarger(bool en) = 0;
  virtual void click() = 0;
  virtual void step() = 0;
};

#endif
