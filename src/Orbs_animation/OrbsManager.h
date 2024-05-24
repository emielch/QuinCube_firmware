#pragma once

#include <Arduino.h>

#include "Orb.h"
#include "config.h"

class OrbsManager {
 public:
  void init(void (*_renderInterrupt)());
  void update();

 private:
  void touchInput();
  const static int orbAm = 20;
  Orb orbs[orbAm];

  void (*renderInterrupt)();
};

extern OrbsManager orbsManager;
