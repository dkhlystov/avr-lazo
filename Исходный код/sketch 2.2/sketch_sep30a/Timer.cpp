// Timer.cpp
#include "Timer.h"

Timer::Timer() {
  _started = false;
}

void Timer::start(uint32_t duration) {
  _duration = duration;
  _time = millis();
  _started = true;
}

bool Timer::done() {
  if (_started) {
    if (millis() > (_time + _duration)) {
      _started = false;
    }
  }
  return !_started;
}

bool Timer::started() {
  return _started;
}

void Timer::clear() {
  _started = false;
}
