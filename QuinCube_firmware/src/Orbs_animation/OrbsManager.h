#pragma once

#include <Arduino.h>

#include "Orb.h"
#include "config.h"

class OrbsManager {
 public:
  void init(void (*_renderInterrupt)());
  void update();

 private:
  const static int orbAm = 20;
  Orb orbs[orbAm];
  elapsedMillis sincePrint = 0;
  float dt;
  void calcDt();
  float getFPS();
  uint32_t lastCalc;  // variable to keep track of the loops per second
  void (*renderInterrupt)();
};

extern OrbsManager orbsManager;
