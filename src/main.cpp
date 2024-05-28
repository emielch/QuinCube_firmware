#include <Arduino.h>

#include "CubeOS/CubeOS.h"
#include "Time/TimeManager.h"

void setup() {
  cubeOS.init();
  demoManager.setBri(100, true);
  timeManager.init(&demoManager.enableDemo, &demoManager.disableDemo);
}

void loop() {
  cubeOS.update();
}