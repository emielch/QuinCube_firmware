#include "RainbowManager.h"

#include "LEDManager.h"
#include "config.h"
#include "makeColor.h"

void RainbowManager::init(void (*_renderInterrupt)(), int bri) {
  renderInterrupt = _renderInterrupt;
  for (int i = 0; i < 180; i++) {
    int hue = i * 2;
    int saturation = 100;
    int lightness = bri;
    // pre-compute the 180 rainbow colors
    rainbowColors[i] = makeColor(hue, saturation, lightness);
  }
}

void RainbowManager::update() {
  if (sinceUpdate < 20) return;
  sinceUpdate = 0;

  int phaseShift = 10;
  for (int x = 0; x < LEDS_PER_CHANNEL; x++) {
    renderInterrupt();
    for (int y = 0; y < 128; y++) {
      int index = (currColorId + x + y * phaseShift / 2) % 180;
      int i = x + y * LEDS_PER_CHANNEL;
      if (i >= TOUCH_LED_START) continue;
      ledManager.setPixel(i, rainbowColors[index]);
    }
  }
  currColorId++;
  if (currColorId >= 180) currColorId = 0;
}

RainbowManager rainbowManager;