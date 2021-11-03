#include "DemoManager.h"

#include "LEDManager.h"
#include "Orbs_animation/OrbsManager.h"
#include "Rainbow_animation/RainbowManager.h"
#include "SerialStreamManager.h"
#include "Touch/TouchbarManager.h"

void DemoManager::init(void (*_renderInterrupt)()) {
  renderInterrupt = _renderInterrupt;
  touchbarManager.init();
  orbsManager.init(renderInterrupt);
  rainbowManager.init(renderInterrupt, brightness);
  currAnim = Orbs;
  enableDemo();
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
    if (demoRunning) {
      touchbarManager.update();
      if (currAnim == Orbs)
        orbsManager.update();
      else if (currAnim == Rainbow)
        rainbowManager.update();
    }
  }

  ledManager.update();
}

void DemoManager::switchAnim(DemoAnim d) {
  currAnim = d;
}

void DemoManager::adjBri(int v) {
  brightness += v * brightness * 0.2;
  Serial.println(brightness);
  rainbowManager.init(renderInterrupt, constrain(brightness, 0, 100));
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