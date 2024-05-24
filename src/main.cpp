#include <Arduino.h>

#include "CubeOS\Audio\AudioManager.h"
#include "CubeOS\Demo\DemoManager.h"
#include "CubeOS\Driver.h"
#include "CubeOS\SerialStreamManager.h"

void setup() {
  cube->init();
  serialStreamManager.init();
  audioManager.init();
  demoManager.init();
}

void loop() {
  serialStreamManager.update();
  audioManager.update();
  demoManager.update();
}