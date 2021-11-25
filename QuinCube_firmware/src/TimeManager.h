#pragma once

#include <Arduino.h>

#include "config.h"

class TimeManager {
 public:
  void update();
  void printCurrTime();
  unsigned long now();
  void setTime(unsigned long t);
  static void printTime(long t);
  static void printDigits(int digits);
  static int getWeekDay(unsigned long val);
  static int TimeManager::getHours(unsigned long val);
  static int TimeManager::getMinutes(unsigned long val);
  static int TimeManager::getSeconds(unsigned long val);

 private:
  unsigned long onTime = 8.5 * 60 * 60;
  unsigned long offTime = 17 * 60 * 60 +6.5*60;
  unsigned long offset = 1 * 60 * 60;
  elapsedMillis sincePrint = 0;
};

extern TimeManager timeManager;
