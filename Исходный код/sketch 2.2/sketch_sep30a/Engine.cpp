// Engine.cpp
#include "Engine.h"

Engine::Engine(int pinCheckRunning, int pinCheckBlocked, int pinStart, int pinStop, int pinLoad) {
  _pinCheckRunning = pinCheckRunning;
  _pinCheckBlocked = pinCheckBlocked;
  _pinStart = pinStart;
  _pinStop = pinStop;
  _pinLoad = pinLoad;

  // Init pins
  pinMode(_pinCheckRunning, INPUT_PULLUP);
  pinMode(_pinCheckBlocked, INPUT_PULLUP);
  pinMode(_pinStart, OUTPUT);
  pinMode(_pinStop, OUTPUT);
  pinMode(_pinLoad, OUTPUT);
  digitalWrite(_pinStart, HIGH);
  digitalWrite(_pinStop, HIGH);
  digitalWrite(_pinLoad, HIGH);

  // Default settings
  _initTimeout = 2000;
  _runningCheckDuration = 200;
  _runningMinRPM = 300;
  _runningCheckTimeout = 500;
  _startDuration = 3000;
  _startCheckTimeout = 3000;
  _startPauseDuration = 10000;
  _startMaxAttempts = 5;
  _warmingDuration = 180000;
  _coolingDuration = 300000;
  _stopMaxDuration = 20000;
  _stopTimeout = 3000;
  _enabled = false;

  // Initialization
  _initTimer.start(_initTimeout);

  // Check blocking
  _isBlocked = false;

  // Check running
  _isRunning = false;
  _runningValueCount = 0;
  _RPM = 0;
  _runningCheckTimer.start(_runningCheckDuration);

  _isInitialized = false;
  _isHot = true;
  _isWarming = false;
  _isCooling = false;
}

bool Engine::isInitialized() {
  return _isInitialized;
}

bool Engine::isBlocked() {
  return _isBlocked;
}

bool Engine::isRunning() {
  return _isRunning;
}

bool Engine::isStarting() {
  return _isStarting;
}

bool Engine::isStopping() {
  return _isStopping;
}

bool Engine::isStartPause() {
  return _isStarting && _isStartPause;
}

bool Engine::isStartError() {
  return _isStartError;
}

bool Engine::isStopError() {
  return _isStopError;
}

bool Engine::isWarming() {
  return _isWarming;
}

bool Engine::isCooling() {
  return _isCooling;
}

bool Engine::isLoad() {
  return _isLoad;
}

int Engine::getStartDuration() {
  return _startDuration;
}

void Engine::setStartDuration(int value) {
  _startDuration = value;
}

int Engine::getStartPauseDuration() {
  return _startPauseDuration;
}

void Engine::setStartPauseDuration(int value) {
  _startPauseDuration = value;
}

int Engine::getStartMaxAttempts() {
  return _startMaxAttempts;
}

void Engine::setStartMaxAttampts(int value) {
  _startMaxAttempts = value;
}

int Engine::getWarmingDuration() {
  return _warmingDuration;
}

void Engine::setWarmingDuration(uint32_t value) {
  _warmingDuration = value;
}

int Engine::getCoolingDuration() {
  return _coolingDuration;
}

void Engine::setCoolingDuration(uint32_t value) {
  _coolingDuration = value;
}

bool Engine::getEnabled() {
  return _enabled;
}

void Engine::setEnabled(bool value) {
  _enabled = value;
}

void Engine::start() {
  if (!_enabled) {
    return;
  }

  if (!_isInitialized) {
    return;
  }

  if (_isStarting) {
    return;
  }

  _isCooling = false;
  _isStopping = false;
  digitalWrite(_pinStop, HIGH);
  _isStartError = false;

  if (_isRunning) {
    return;
  }

  _isHot = false;
  _isWarming = false;

  _isStarting = true;
  _isStartPause = false;
  _startAttempts = 0;
  _startTimer.clear();
}

void Engine::stop() {
  if (!_enabled) {
    return;
  }

  if (!_isInitialized) {
    return;
  }

  if (_isCooling || _isStopping) {
    return;
  }

  _isWarming = false;
  _isStarting = false;
  digitalWrite(_pinStart, HIGH);
  _isStopError = false;

  if (!_isRunning) {
    return;
  }

  if (_isHot) {
    _isCooling = true;
    _coolingTimer.start(_coolingDuration);
  } else {
    _isStopping = true;
    _stopTimer.clear();
  }
}

void Engine::resume() {
  if (!_enabled) {
    return;
  }

  if (!_isInitialized) {
    return;
  }

  if (!_isRunning || !_isCooling) {
    return;
  }

  _isCooling = false;
}

void Engine::clearErrors() {
  _isStartError = false;
  _isStopError = false;
}

void Engine::loop() {
  if (_initTimer.done()) {
    _isInitialized = true;
  }

  //  this->loopCheckBlocking(); // Если не замкнуто - сопротивление 1 Ом (причина не известна)
  this->loopCheckRunning();
  this->loopStart();
  this->loopWarming();
  this->loopCooling();
  this->loopStop();
  this->loopLoad();
}

void Engine::loopCheckBlocking() {
  int value = digitalRead(_pinCheckBlocked);
  _isBlocked = value == 1;
}

void Engine::loopCheckRunning() {
  // Count pin state changes
  int runningValue = digitalRead(_pinCheckRunning);
  if (runningValue == HIGH && _runningValue == LOW) {
    _runningValueCount++;
  }
  _runningValue = runningValue;

  // Calc the RPM
  if (_runningCheckTimer.done()) {
    _RPM = _runningValueCount * (1000 / _runningCheckDuration) * 60;
    _runningValueCount = 0;
    _runningCheckTimer.start(_runningCheckDuration);
  }

  // Debug
//  if (_isStarting || _isRunning) {
//    _RPM = 600;
//  }

  // Is running now
  bool isRunning = _RPM > _runningMinRPM;

  // Updating state with timeout
  if (isRunning != _isRunning) {
    if (!_runningTimer.started()) {
      _runningTimer.start(_runningCheckTimeout);
    } else if (_runningTimer.done()) {
      _isRunning = isRunning;
    }
  }
}

void Engine::loopStart() {
  if (!_isStarting) {
    return;
  }
  if (!_enabled) {
    digitalWrite(_pinStart, HIGH);
    _isStarting = false;
    return;
  }
  if (!_isStartPause) {
    if (!_startTimer.started()) {
      _startAttempts++;
      digitalWrite(_pinStart, LOW);
      _startTimer.start(_startDuration);
    } else if (_startTimer.done()) {
      digitalWrite(_pinStart, HIGH);
      _isStartPause = true;
    }
  } else {

    if (!_startCheckTimer.started()) {
      _startCheckTimer.start(_startCheckTimeout);
    } else if (_startCheckTimer.done()) {
      if (_isRunning) {
        _isStarting = false;
        if (!_isHot) {
          _isWarming = true;
          _warmingTimer.start(_warmingDuration);
        }
      } else if (_startAttempts >= _startMaxAttempts) {
        _isStarting = false;
        _isStartError = true;
      }
    }

    if (!_startPauseTimer.started()) {
      _startPauseTimer.start(_startPauseDuration);
    } else if (_startPauseTimer.done()) {
      _isStartPause = false;
    }
  }
}

void Engine::loopWarming() {
  if (!_enabled) {
    _isHot = false;
    _isWarming = false;
    return;
  }
  if (!_isWarming) {
    return;
  }
  if (_isStarting) {
    return;
  }
  if (!_isRunning) {
    _isHot = false;
    _isWarming = false;
  } else if (_isHot) {
    _isWarming = false;
  } else if (_warmingTimer.done()) {
    _isHot = true;
    _isWarming = false;
  }
}

void Engine::loopCooling() {
  if (!_isCooling) {
    return;
  }
  if (!_enabled) {
    _isCooling = false;
    return;
  }
  if (_coolingTimer.done()) {
    _isCooling = false;
    _isStopping = true;
    _stopTimer.clear();
  }
}

void Engine::loopStop() {
  if (!_isStopping) {
    return;
  }
  if (!_enabled) {
    digitalWrite(_pinStop, HIGH);
    _isStopping = false;
    return;
  }
  if (!_stopTimer.started()) {
    digitalWrite(_pinStop, LOW);
    _stopTimer.start(_stopMaxDuration);
    _stopCheckTimer.clear();
  } else if (_stopTimer.done()) {
    digitalWrite(_pinStop, HIGH);
    _isStopping = false;
    _isStopError = true;
  } else if (!_isRunning) {
    if (!_stopCheckTimer.started()) {
      _stopCheckTimer.start(_stopTimeout);
    } else if (_stopCheckTimer.done()) {
      digitalWrite(_pinStop, HIGH);
      _isStopping = false;
    }
  }
}

void Engine::loopLoad() {
  if (_isStarting || _isWarming || _isCooling || _isStopping) {
    _isLoad = false;
    digitalWrite(_pinLoad, LOW);
  } else {
    _isLoad = true;
    digitalWrite(_pinLoad, HIGH);
  }
}
