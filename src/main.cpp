#include <Arduino.h>

#include "CubeOS/CubeOS.h"
#include "CubeOS/Time/TimeManager.h"
#include "CubeOS/Anim/AnimManager.h"
#include "Touch/MPR121_Manager.h"

void setup() {
  mpr121Manager.init();
  cubeOS.init();

  animManager.setAnim(2);
  
  cubeOS.setBri(75);
  timeManager.setOnTime(8,30,0);
  timeManager.setOffTime(18,0,0);
  timeManager.setOnDay(6, false);  // off on saturday
  timeManager.setOnDay(0, false);  // off on saturday
}

void loop() {
  mpr121Manager.update();
  cubeOS.update();
}