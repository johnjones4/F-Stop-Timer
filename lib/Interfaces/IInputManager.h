#ifndef IInputManager_h
#define IInputManager_h

enum class DialDirection { NONE = 0, CLOCKWISE = 1, COUNTERCLOCKWISE = -1 };

typedef enum {
  Reset   = 0,
  MemSet  = 1,
  Mem1    = 2,
  Mem2    = 3,
  Mem3    = 4,
  Start   = 5,
} ButtonName;

typedef enum {
  BaseTime      = 0,
  StepInterval  = 1,
  PrintStop     = 2,
} DialName;

typedef enum {
  Focus      = 0,
  Test       = 1,
  PrintMode  = 2,
  None       = 3,
} Mode;

class IInputManager {
public:
  virtual ~IInputManager() {}
  virtual int begin() = 0;
  virtual void step() = 0;
  virtual bool isPressed(ButtonName b) = 0;
  virtual DialDirection getDialDirection(DialName d) = 0;
  virtual unsigned long getDialSpeed(DialName d) = 0;
  virtual Mode getSelectedMode() = 0;
};

#endif
