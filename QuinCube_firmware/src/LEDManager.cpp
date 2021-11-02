#include "LedManager.h"

DMAMEM int displayMemory[LEDS_PER_CHANNEL * 96];
int drawingMemory[LEDS_PER_CHANNEL * 96];

void LEDManager::init() {
  const int config = WS2811_GRB | WS2811_800kHz;
  leds = new MultiWS2811(LEDS_PER_CHANNEL, displayMemory, drawingMemory, config);
  leds->begin();
}

void LEDManager::update() {
  leds->show();
  resetLEDs();
}

void LEDManager::setVoxel(int x, int y, int z, byte r, byte g, byte b) {
  setPixel(getVoxelId(x, y, z), r, g, b);
}

void LEDManager::setVoxel(int x, int y, int z, Color c) {
  setPixel(getVoxelId(x, y, z), c.red(), c.green(), c.blue());
}

Color LEDManager::getVoxel(int x, int y, int z) {
  return getPixel(getVoxelId(x, y, z));
}

void LEDManager::setPixel(int id, byte r, byte g, byte b) {
  leds->setPixel(id, r, g, b);
}

void LEDManager::setPixel(int id, Color c) {
  leds->setPixel(id, c.red(), c.green(), c.blue());
}

Color LEDManager::getPixel(int id) {
  uint32_t conn = leds->getPixel(id);  // retrieve the color that has already been saved
  byte b = conn & 255;                 // unpack the color
  byte g = conn >> 8 & 255;
  byte r = conn >> 16 & 255;
  Color c(r, g, b, RGB_MODE);
  return c;
}

int LEDManager::getVoxelId(int x, int y, int z) {
  int id = y;
  id += 15000 - z * 625;  // 24*25*25 - z*25*25;
  id += 600 - x * 25;     // 25*24 - x*25;

  return id;
}

void LEDManager::resetLEDs() {
  memset(drawingMemory, 0, bufsize);
}

LEDManager ledManager;