#pragma once

#include <Arduino.h>

#include "config.h"

class DemoManager {
 public:
  void init(void (*_renderInterrupt)());
  void update();
  void enableDemo();
  void disableDemo();

 private:
  void startDemo();
  void stopDemo();
  bool demoRunning, demoEnabled;
  elapsedMillis sinceUpdate;
  void (*renderInterrupt)();
};

extern DemoManager demoManager;
