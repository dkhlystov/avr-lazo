// Engine.h
#ifndef Engine_h
#define Engine_h

#include <Arduino.h>
#include "Timer.h"

class Engine {
  public:
    Engine(int pinCheckRunning, int pinCheckBlocked, int pinStart, int pinStop, int pinLoad);
    bool isInitialized();
    bool isBlocked();
    bool isRunning();
    bool isStarting();
    bool isStopping();
    bool isStartPause();
    bool isStartError();
    bool isStopError();
    bool isWarming();
    bool isCooling();
    bool isLoad();

    int getStartDuration();
    void setStartDuration(int value);
    int getStartPauseDuration();
    void setStartPauseDuration(int value);
    int getStartMaxAttempts();
    void setStartMaxAttampts(int value);
    int getWarmingDuration();
    void setWarmingDuration(uint32_t value);
    int getCoolingDuration();
    void setCoolingDuration(uint32_t value);
    bool getEnabled();
    void setEnabled(bool value);

    void start();
    void stop();
    void resume();
    void clearErrors();

    void loop();

  private:
    int _pinCheckRunning;
    int _pinCheckBlocked;
    int _pinStart;
    int _pinStop;
    int _pinLoad;

    // Settings
    uint32_t _initTimeout; // Время инициализации
    uint32_t _runningCheckDuration;  // Подсчет RPM через это время
    int _runningMinRPM; // Если RPM больше этого значения - двигатель работает
    uint32_t _runningCheckTimeout; // Меняем статус через это время
    int _startDuration; // Время прокрутки стартера
    uint32_t _startCheckTimeout; // Проверка запуска через это время, после остановки стартера
    uint32_t _startPauseDuration; // Время паузы между стартами
    int _startMaxAttempts; // Максимальное количество попыток запуска
    uint32_t _warmingDuration; // Время прогрева без нагрузки
    uint32_t _coolingDuration; // Время на остывание без нагрузки перед глушением
    uint32_t _stopMaxDuration; // Максимальное время глушения
    uint32_t _stopTimeout; // Отключение сигнала "Стоп" через это время после остановки
    bool _enabled; // Включено управление

    // State
    bool _isInitialized;
    bool _isBlocked;
    bool _isRunning;
    bool _isStarting;
    bool _isStopping;
    bool _isStartError;
    bool _isStopError;
    bool _isWarming;
    bool _isCooling;
    bool _isLoad;
    bool _isHot;

    // Initialization
    Timer _initTimer;

    // Check blocking
    void loopCheckBlocking();

    // Check running
    int _runningValue;
    int _runningValueCount;
    int _RPM;
    Timer _runningCheckTimer;
    Timer _runningTimer;
    void loopCheckRunning();

    // Starting
    bool _isStartPause;
    int _startAttempts;
    Timer _startTimer;
    Timer _startCheckTimer;
    Timer _startPauseTimer;
    void loopStart();

    // Warming
    Timer _warmingTimer;
    void loopWarming();

    // Stopping
    Timer _coolingTimer;
    Timer _stopTimer;
    Timer _stopCheckTimer;
    void loopCooling();
    void loopStop();

    // Load
    void loopLoad();
};

#endif
