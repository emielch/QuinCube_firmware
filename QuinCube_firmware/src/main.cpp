#include <Arduino.h>
#include <EEPROM.h>

#include "DemoManager.h"
#include "LedManager.h"

#include "SerialStreamManager.h"
#include "Touch/MPR121_Manager.h"


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
  ledManager.init();
  demoManager.init(&renderInterrupt);
  
}

void loop() {
  serialStreamManager.update();
  demoManager.update();
}