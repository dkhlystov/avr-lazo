// Input.h
#ifndef Input_h
#define Input_h

#include <Arduino.h>
#include "Timer.h"

class Input {
  public:
    Input(int pin);
    int getState();
    void loop();

  private:
    int _pin;
    int _state;

    int _currentState;
    int _targetState;
    
    Timer _debounceTimer;
};

#endif
