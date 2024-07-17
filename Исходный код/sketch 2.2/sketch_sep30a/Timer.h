// Timer.h
#ifndef Timer_h
#define Timer_h

#include <Arduino.h>

class Timer {
  public:
    Timer();
    void start(uint32_t duration);
    bool done();
    bool started();
    void clear();

  private:
    uint32_t _duration;
    bool _started;
    uint32_t _time;
};

#endif
