#include <Arduino.h>

#include "CubeOS/CubeOS.h"

void setup() {
  cubeOS.init();
  demoManager.setBri(100, true);
  timeManager.setOnTime(8.5 * 60 * 60);
  timeManager.setOffTime(17.5 * 60 * 60);
}

void loop() {
  cubeOS.update();
}