#pragma once

#include <Arduino.h>

#include "Touch.h"
#include "Vector3.h"
#include "config.h"

class TouchbarManager {
 public:
  void init();
  void update();
  Vector3 getSpd() { return spd; }
  float getRotationSpd() { return rotSpd; }
  float getScaleSpd() { return scaleSpd; }
  int getTouchAm() { return touchAm; }
  int getSinceTouched() { return sinceTouched; }

 private:
  void updateTouches();
  void render();
  Touch* createTouch(float pos, float pow);
  Touch* findTouchOnPos(Touch* t);
  Touch touches[MAX_TOUCH_AM];
  int touchAm = 0;
  elapsedMillis sinceTouched = 0;
  float hue = 0;

  void updateMovementValues();
  Vector3 pos, spd;
  float rot, scale, rotSpd, scaleSpd;
};

extern TouchbarManager touchbarManager;