// LED.cpp
#include "LED.h"

LED::LED(int pin) {
  _pin = pin;

  // Init pins
  pinMode(_pin, OUTPUT);
  this->off();

  // Defaults
  _blinkDuration = 500;
}

void LED::toggle(bool value) {
  if (value) {
    this->on();
  } else {
    this->off();
  }
}

void LED::on() {
  _blink = false;
  _state = 1;
  digitalWrite(_pin, HIGH);
}

void LED::off() {
  _blink = false;
  _state = 0;
  digitalWrite(_pin, LOW);
}

void LED::blink() {
  if (_blink) {
    return;
  }
  _blink = true;
  _state = 1;
  digitalWrite(_pin, HIGH);
  _blinkTimer.start(_blinkDuration);
}

void LED::loop() {
  if (_blink && _blinkTimer.done()) {
    if (_state == 1) {
      _state = 0;
      digitalWrite(_pin, LOW);
    } else {
      _state = 1;
      digitalWrite(_pin, HIGH);
    }
    _blinkTimer.start(_blinkDuration);
  }
}
