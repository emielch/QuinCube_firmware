#include "DemoManager.h"

#include "LEDManager.h"
#include "Orbs_animation/OrbsManager.h"
#include "SerialStreamManager.h"
#include "Touch/TouchbarManager.h"

void DemoManager::init(void (*_renderInterrupt)()) {
  renderInterrupt = _renderInterrupt;  
  touchbarManager.init();
  orbsManager.init(renderInterrupt);
  enableDemo();
  sinceUpdate = 0;
}

void DemoManager::update() {
  if (serialStreamManager.getSinceNewFrame() < STREAM_BLACKOUT_DELAY) {
    if (demoRunning) stopDemo();
    return;  // if a streaming frame has been received recently, we are still in streaming mode and we shouldn't interfere with the LEDs
  }

  if (serialStreamManager.getSinceNewFrame() > DEMO_DELAY) {
    if (!demoRunning && demoEnabled) {
      startDemo();
    }
    if (demoRunning){
      touchbarManager.update();
      orbsManager.update();
    }
  }

  ledManager.update();
}

void DemoManager::enableDemo() {
  demoEnabled = true;
}
void DemoManager::disableDemo() {
  demoEnabled = false;
  stopDemo();
}
void DemoManager::startDemo() {
  demoRunning = true;
  orbsManager.init(renderInterrupt);
}
void DemoManager::stopDemo() {
  demoRunning = false;
}

DemoManager demoManager;