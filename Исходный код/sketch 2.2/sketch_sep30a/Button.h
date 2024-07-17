// Button.h
#ifndef Button_h
#define Button_h

#include <Arduino.h>
#include "Timer.h"

class Button {
  public:
    Button(int pin);
    int getState();
    void loop();

  private:
    int _pin;
    int _state;

    int _value;
    int _currentValue;
    int _targetValue;

    Timer _debounceTimer;
};

#endif
