#pragma once

#include <Arduino.h>

#include "Touch.h"
#include "config.h"

class TouchbarManager {
 public:
  void init();
  void update();

 private:
  void render();
  Touch* createTouch(float pos, float pow);
  Touch* findTouchOnPos(Touch* t);
  Touch touches[MAX_TOUCH_AM];
  float hue = 0;
};

extern TouchbarManager touchbarManager;