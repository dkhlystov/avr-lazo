// Button.cpp
#include "Button.h"

Button::Button(int pin) {
  _pin = pin;
  _state = 0;

  _value = 0;
  _targetValue = 0;
  _currentValue = 0;

  // Init pins
  pinMode(_pin, INPUT_PULLUP);
}

int Button::getState() {
  return _state;
}

void Button::loop() {
  int value = digitalRead(_pin);
  int _currentValue = value ? 0 : 1;

  if (_currentValue != _targetValue) {
    _targetValue = _currentValue;
    _debounceTimer.start(500);
  } else if (_targetValue != _value && _debounceTimer.done()) {
    _value = _targetValue;
    if (_value == 1) {
      _state = 1 - _state;
    }
  }
//
//  
//  
//  if (_debounceTimer.done()) {
//    int value = digitalRead(_pin);
//    if (value != _value) {
//      _value = value;
//      if (_value == 0) {
//        _state = 1 - _state;
//      }
//      _debounceTimer.start(100);
//    }
//  }
}
