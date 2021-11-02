#include "DemoManager.h"

#include "LEDManager.h"

void DemoManager::init(void (*_renderInterrupt)()) {
  renderInterrupt = _renderInterrupt;
  sinceUpdate = 0;
}

void DemoManager::update() {
  pos += sinceUpdate / 1000. * spd;
  if (pos > CUBE_SIZE - 1) pos = 0;
  sinceUpdate = 0;
  for (int x = 0; x < CUBE_SIZE; x++) {
    for (int y = 0; y < CUBE_SIZE; y++) {
      ledManager.setVoxel(x, y, pos, 0, 1, 1);
    }
  }
}

DemoManager demoManager;