#ifndef Toggle_h
#define Toggle_h

class Toggle
{
public:
  Toggle(int pin);
  void begin();
  bool isOn();
private:
  int pin;
};

#endif