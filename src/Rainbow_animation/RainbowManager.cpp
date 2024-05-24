#include "RainbowManager.h"

#include "LEDManager.h"
#include "Touch/TouchbarManager.h"
#include "config.h"
#include "makeColor.h"

void RainbowManager::init(void (*_renderInterrupt)(), float bri) {
  renderInterrupt = _renderInterrupt;
  brighntess = bri;
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

  float briInc = touchbarManager.getSpd().x * 0.5;
  if (abs(briInc) > 0) {
    brighntess += touchbarManager.getSpd().x;
    init(renderInterrupt, constrain(brighntess, 0, 100));
  }

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