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

void LEDManager::show() {
  leds->show();
}

void LEDManager::setVoxel(byte x, byte y, byte z, Color c) {
  setPixel(getVoxelLedId(x, y, z), c.red(), c.green(), c.blue());
}

void LEDManager::setVoxel(byte x, byte y, byte z, byte r, byte g, byte b) {
  setPixel(getVoxelLedId(x, y, z), r, g, b);
}

Color LEDManager::getVoxel(byte x, byte y, byte z) {
  return getPixel(getVoxelLedId(x, y, z));
}

void LEDManager::setPixelTB(int pos, Color c) {
  setPixel(getTbLedId(pos), c.red(), c.green(), c.blue());
}

Color LEDManager::getPixelTB(int pos) {
  return getPixel(getTbLedId(pos));
}

void LEDManager::setPixel(int id, Color c) {
  setPixel(id, c.red(), c.green(), c.blue());
}

void LEDManager::setPixel(int id, int c) {
  leds->setPixel(id, c);
}

void LEDManager::setPixel(int id, byte r, byte g, byte b) {
  if (id < 0 || id > TOTAL_LEDS_AM - 1) return;
  leds->setPixel(id, r, g, b);
}

Color LEDManager::getPixel(int id) {
  if (id < 0 || id > TOTAL_LEDS_AM - 1) return Color(0, 0, 0, RGB_MODE);
  ;
  uint32_t conn = leds->getPixel(id);  // retrieve the color that has already been saved
  byte b = conn & 255;                 // unpack the color
  byte g = conn >> 8 & 255;
  byte r = conn >> 16 & 255;
  Color c(r, g, b, RGB_MODE);
  return c;
}

int LEDManager::getVoxelLedId(byte x, byte y, byte z) {
  if (x > 24 || y > 24 || z > 24) return -1;
  int id = y;
  id += 15000 - z * 625;  // 24*25*25 - z*25*25;
  id += 600 - x * 25;     // 25*24 - x*25;

  return id;
}

int LEDManager::getTbLedId(int pos) {
  if (pos < -TOUCH_LED_LEN) return -1;          // we allow to wrap around the ends of the range (modulo later), but only limited when negative
  pos = TOUCH_LED_LEN - pos;                    // invert the direction
  pos += TOUCH_LED_LEN / 2;                     // shift half of the touchbar's length to align
  pos -= TOUCH_SEG_LEN * 0.2;                   // align the segments more precicely
  pos = (pos + TOUCH_LED_LEN) % TOUCH_LED_LEN;  // wrap around the edges, roll over
  return TOUCH_LED_START + pos;                 // make it start at the actual start postion of the TB LEDs
}

void LEDManager::resetLEDs() {
  memset(drawingMemory, 0, bufsize);
}

LEDManager ledManager;