#include <Arduino.h>

#include "CubeOS\CubeOS.h"

void setup() {
  cubeOS.init();
  demoManager.setBri(100, true);
}

void loop() {
  cubeOS.update();
}