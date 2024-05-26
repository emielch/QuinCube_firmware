#include <Arduino.h>

#include "CubeOS\CubeOS.h"

void setup() {
  cubeOS.init();
}

void loop() {
  cubeOS.update();
}