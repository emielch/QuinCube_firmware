#include "OrbsManager.h"

void OrbsManager::init(void (*_renderInterrupt)()) {
  renderInterrupt = _renderInterrupt;
  lastCalc = micros();
  for (int i = 0; i < orbAm; i++) {
    orbs[i].init(random(300, 850) / 100., random(300, 900) / 100., i * 137.508);
  }
}

void OrbsManager::update() {
  for (int i = 0; i < orbAm; i++) {
    renderInterrupt();
    orbs[i].move(dt);
    orbs[i].render();
  }

  calcDt();
  if (sincePrint > 1000) {
    Serial.print("FPS: ");
    Serial.println(getFPS());
    sincePrint = 0;
  }
}

void OrbsManager::calcDt() {
  uint32_t currTime = micros();
  dt = (currTime - lastCalc) / 1000000.;  // assume one frame per second
  lastCalc = currTime;
}

float OrbsManager::getFPS() {
  return 1 / dt;
}


OrbsManager orbsManager;