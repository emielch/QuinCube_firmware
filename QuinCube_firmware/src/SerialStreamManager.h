#pragma once

#include <Arduino.h>

class SerialStreamManager {
 public:
  void init();
  void update();
  unsigned long getSinceNewFrame() { return sinceNewFrame; }

 private:
  elapsedMillis sinceNewFrame = 999999;
};

extern SerialStreamManager serialStreamManager;