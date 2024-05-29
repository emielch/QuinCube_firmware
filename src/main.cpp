#include <Arduino.h>

#include "CubeOS/CubeOS.h"
#include "Touch/MPR121_Manager.h"

void setup() {
  cubeOS.init();
  mpr121Manager.init();
  demoManager.setBri(100, true);
  timeManager.setOnTime(8.5 * 60 * 60);
  timeManager.setOffTime(17.5 * 60 * 60);
}

void loop() {
  cubeOS.update();
  mpr121Manager.update();
}