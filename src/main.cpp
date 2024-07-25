#include <Arduino.h>

#include "CubeOS/CubeOS.h"
#include "Touch/MPR121_Manager.h"

void setup() {
  cubeOS.init();
  mpr121Manager.init();
  demoManager.setBri(75, true);
  timeManager.setOnTime(8.5 * 60 * 60);
  timeManager.setOffTime(18 * 60 * 60);
  timeManager.setOnDay(6, false);  // off on saturday
  timeManager.setOnDay(0, false);  // off on saturday
}

void loop() {
  cubeOS.update();
  mpr121Manager.update();
}