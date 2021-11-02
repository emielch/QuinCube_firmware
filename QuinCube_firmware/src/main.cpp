#include <Arduino.h>
#include <EEPROM.h>

#include "DemoManager.h"
#include "LedManager.h"
#include "MPR121_Manager.h"
#include "TouchbarManager.h"
#include "Orbs_animation/OrbsManager.h"



void renderInterrupt() {
  mpr121Manager.update();
}

unsigned long randomSeedVar = 0;
void exciteRandomizer() {
  EEPROM.get(0, randomSeedVar);
  randomSeedVar++;
  EEPROM.put(0, randomSeedVar);
  randomSeed(randomSeedVar);
}

void setup() {
  //while(!Serial){}
  exciteRandomizer();
  mpr121Manager.init();
  touchbarManager.init();
  ledManager.init();
  demoManager.init(&renderInterrupt);
  orbsManager.init(&renderInterrupt);
}

void loop() {
  //mpr121Manager.update();
  touchbarManager.update();
  //demoManager.update();
  orbsManager.update();
  ledManager.update();
}