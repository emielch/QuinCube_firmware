#pragma once

#include <Arduino.h>

class RainbowManager {
 public:
  void init(void (*_renderInterrupt)(), float bri);
  void update();

 private:
  int rainbowColors[180];
  float brighntess = 5;
  int currColorId = 0;
  elapsedMillis sinceUpdate = -9999;
  void (*renderInterrupt)();
};

extern RainbowManager rainbowManager;
