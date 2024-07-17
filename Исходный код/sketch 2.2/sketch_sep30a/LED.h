// LED.h
#ifndef LED_h
#define LED_h

#include <Arduino.h>
#include "Timer.h"

class LED {
  public:
    LED(int pin);
    void toggle(bool value);
    void on();
    void off();
    void blink();
    void loop();

  private:
    int _pin;
    int _state;
    bool _blink;
    int _blinkDuration;
    Timer _blinkTimer;
};

#endif
