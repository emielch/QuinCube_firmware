#pragma once

#include <Arduino.h>

#include "config.h"

enum DemoAnim {
  Orbs,
  Rainbow
};

class DemoManager {
 public:
  void init(void (*_renderInterrupt)());
  void update();
  void enableDemo();
  void disableDemo();
  void switchAnim(DemoAnim d);
  void adjBri(int v);

 private:
  float brightness = 5;
  DemoAnim currAnim;
  void startDemo();
  void stopDemo();
  bool demoRunning, demoEnabled;
  void (*renderInterrupt)();
};

extern DemoManager demoManager;
