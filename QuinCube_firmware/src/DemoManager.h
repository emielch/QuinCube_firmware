#pragma once

#include <Arduino.h>

#include "config.h"

class DemoManager {
 public:
  void init(void (*_renderInterrupt)());
  void update();

 private:
  float pos = 0;
  float spd = 10;
  elapsedMillis sinceUpdate;
  void (*renderInterrupt)();
};

extern DemoManager demoManager;
