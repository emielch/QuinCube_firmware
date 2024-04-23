#include <Arduino.h>
#include <EEPROM.h>

#include "DemoManager.h"
#include "LedManager.h"
#include "SerialStreamManager.h"
#include "TimeManager.h"
#include "Touch/MPR121_Manager.h"
#include "Touch/TouchbarManager.h"

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
  serialStreamManager.init();
  mpr121Manager.init();
  touchbarManager.init();
  ledManager.init();
  demoManager.init(&renderInterrupt);

  //Teensy3Clock.set(1713889270);  // set the time manually, auto set during compile time doesn't seem to work...
}

void loop() {
  timeManager.update();
  serialStreamManager.update();
  mpr121Manager.update();  // also called from within the animations through renderInterrupt() for higher sample rate
  touchbarManager.update();
  demoManager.update();
}