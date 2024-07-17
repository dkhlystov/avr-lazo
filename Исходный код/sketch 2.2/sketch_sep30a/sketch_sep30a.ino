// === НАСТРОЙКИ ===

// Максимальное количество попыток запуска, после 
// которых устанавливается состояние ошибки запуска
const int START_MAX_ATTEMPTS = 5;

// Продолжительность включения стартера
const int START_DURATION = 3000; // 3 сек

// Пауза между попытками запуска
const int START_PAUSE = 10000; // 10 сек

// Время прогрева
const uint32_t WARMING_DURATION = 180000; // 3 мин
//const uint32_t WARMING_DURATION = 30000; // Debug

// Работа без нагрузки перед глушением
const uint32_t COOLING_DURATION = 180000; // 3 мин
//const uint32_t COOLING_DURATION = 30000; // Debug



// === ПИНЫ ===
const int PIN_INPUT = 14; // Вход
const int PIN_BUTTON_AUTO = 11; // Кнопка авто-режима
const int PIN_ENGINE_CHECK_RUNNING = 12; // Проверка работы двигателя
const int PIN_ENGINE_CHECK_BLOCK = 19; // Проверка блокировки двигателя
const int PIN_ENGINE_START = 16; // Старт (стартер)
const int PIN_ENGINE_STOP = 17; // Стоп (блокировка работы)
const int PIN_ENGINE_LOAD = 18; // Отключение нагрузки (прогрев/остывание)
const int PIN_LED_ON = 7; // Индикатор вкл
const int PIN_LED_AUTO = 10; // Индикатор авто-режима
const int PIN_LED_INPUT = 8; // Индикатор сигнала на входе
const int PIN_LED_LOAD = 9; // Индикатор включения нагрузки (мигает - прогрев)
const int PIN_LED_RUNNING = 6; // Индикатор работы двигателя
const int PIN_LED_START = 5; // Индикатор запуска (мигает - пауза)
const int PIN_LED_START_ERROR = 3; // Индикатор ошибки запуска (мигает - заблокирован ключем)
const int PIN_LED_STOP = 4; // Индикатор остановки двигателя
const int PIN_LED_STOP_ERROR = 2; // Индикатор ошибки остановки



// === ПЕРЕМЕННЫЕ ===
#include "Input.h"
#include "Button.h"
#include "Engine.h"
#include "LED.h"

// Вход
Input input(PIN_INPUT);

// Кнопка авто-режима
Button buttonAuto(PIN_BUTTON_AUTO);

// Двигатель
Engine engine(PIN_ENGINE_CHECK_RUNNING, PIN_ENGINE_CHECK_BLOCK, PIN_ENGINE_START, PIN_ENGINE_STOP, PIN_ENGINE_LOAD);

// Индикаторы
LED ledOn(PIN_LED_ON);
LED ledAuto(PIN_LED_AUTO);
LED ledInput(PIN_LED_INPUT);
LED ledLoad(PIN_LED_LOAD);
LED ledRunning(PIN_LED_RUNNING);
LED ledStart(PIN_LED_START);
LED ledStartError(PIN_LED_START_ERROR);
LED ledStop(PIN_LED_STOP);
LED ledStopError(PIN_LED_STOP_ERROR);



void setup() {
  engine.setStartDuration(START_DURATION);
  engine.setStartPauseDuration(START_PAUSE);
  engine.setStartMaxAttampts(START_MAX_ATTEMPTS);
  engine.setWarmingDuration(WARMING_DURATION);
  engine.setCoolingDuration(COOLING_DURATION);

  ledOn.on();
}

void loop() {
  input.loop();
  // buttonAuto.loop(); Без кнопки "Авто"
  // engine.setEnabled(buttonAuto.getState() == 1);
  engine.setEnabled(1);
  engine.loop();
  ledAuto.loop();
  ledInput.loop();
  ledLoad.loop();
  ledRunning.loop();
  ledStart.loop();
  ledStartError.loop();
  ledStop.loop();
  ledStopError.loop();

  // Действия
  if (engine.isInitialized()) {
    // if (buttonAuto.getState()) {
    if (1) { // Всегда "Авто"
      if (input.getState()) {
        if (!engine.isRunning()) {
          if (!engine.isStartError()) {
            engine.start();
          }
        } else {
          engine.resume(); // load on if cooling
        }
      } else {
        if (!engine.isCooling() && !engine.isStopping()) {
          if (!engine.isStopError()) {
            engine.stop();
          }
        }
      }
    } else {
      if (engine.isStopError() || engine.isStartError()) {
        engine.clearErrors();
      }
    }
  }

  // Индикация
  // ledAuto.toggle(buttonAuto.getState() == 1);
  // ledInput.toggle(input.getState() == 1);
  if (input.getState() == 1) {
    ledInput.on();
  } else {
    ledInput.blink();
  }
  if (!engine.isRunning()) {
    ledLoad.off();
  } else if (engine.isLoad()) {
    ledLoad.on();
  } else if (engine.isWarming() || engine.isCooling()) {
    ledLoad.blink();
  } else {
    ledLoad.off();
  }
  ledRunning.toggle(engine.isRunning());
  if (engine.isStartPause()) {
    ledStart.blink();
  } else {
    ledStart.toggle(engine.isStarting());
  }
  if (!engine.isStopping() && engine.isBlocked()) {
    ledStartError.blink();
  } else {
    ledStartError.toggle(engine.isStartError());
  }
  ledStop.toggle(engine.isStopping());
  ledStopError.toggle(engine.isStopError());
}
