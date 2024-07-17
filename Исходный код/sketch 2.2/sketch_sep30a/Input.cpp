// Input.cpp
#include "Input.h"

Input::Input(int pin) {
  _pin = pin;
  _state = 0;

  _targetState = 0;
  _currentState = 0;

  // Init pins
  pinMode(_pin, INPUT_PULLUP);
}

int Input::getState() {
  return _state;
}

void Input::loop() {
  int value = digitalRead(_pin);
  int _currentState = value ? 0 : 1;

  if (_currentState != _targetState) {
    _targetState = _currentState;
    _debounceTimer.start(2000);
  } else if (_targetState != _state && _debounceTimer.done()) {
    _state = _targetState;
  }
}
