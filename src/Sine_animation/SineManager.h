#pragma once

#include <Arduino.h>

class SineManager {
 public:
  void init(void (*_renderInterrupt)());
  void update();

 private:
  int rainbowColors[180];
  void (*renderInterrupt)();

  float x_min = -2;
  float x_max = 2;
  float z_min = -2;
  float z_max = 2;
  float phase = 0.0f;
  float phaseSpd = 2;
  float hueOffset = 0;
  float hueScale = 4;
};

extern SineManager sineManager;
