#pragma once

#include <Arduino.h>

class TimeManager {
 public:
  void init(void (*_enableCB)(), void (*_disableCB)());
  void update();
  void printCurrTime();
  unsigned long now();
  void setTime(unsigned long t);

 private:
  unsigned long onTime = 8.5 * 60 * 60;
  unsigned long offTime = 17.5 * 60 * 60;
  unsigned long offset = 2 * 60 * 60;
  elapsedMillis sincePrint = 0;
  void (*enableCB)();
  void (*disableCB)();
};

extern TimeManager timeManager;
