#include "TouchbarManager.h"

#include <Color.h>

#include "LedManager.h"
#include "MPR121_Manager.h"
#include "config.h"

void TouchbarManager::init() {
}
void TouchbarManager::update() {
  bool touchChecked[EL_AM];
  for (int i = 0; i < EL_AM; i++)
    touchChecked[i] = false;
  //   leftTouchAm = 0;
  //   frontTouchAm = 0;
  //   rightTouchAm = 0;

  for (int i = 0; i < MAX_TOUCH_AM; i++) {
    if (!touches[i].active) continue;

    int pos = touches[i].pos;
    float newPos = 0;
    float newPow = 0;
    while (pos >= 0) {
      touchChecked[pos] = true;
      if (mpr121Manager.getTouchVal(pos) > 1) {
        newPos += pos * mpr121Manager.getTouchVal(pos);
        newPow += mpr121Manager.getTouchVal(pos);
        pos--;
      } else
        break;
    }
    pos = touches[i].pos + 1;
    while (pos <= EL_AM) {
      touchChecked[pos] = true;
      if (mpr121Manager.getTouchVal(pos) > 1) {
        newPos += pos * mpr121Manager.getTouchVal(pos);
        newPow += mpr121Manager.getTouchVal(pos);
        pos++;
      } else
        break;
    }
    if (newPow != 0) {
      newPos /= newPow;
      touches[i].newPos(newPos);
      touches[i].pow = touches[i].pow * 0.5 + newPow * 0.5;

      if (findTouchOnPos(&touches[i]))
        touches[i].active = false;
    } else
      touches[i].active = false;
  }

  float newPos = 0;
  float newPow = 0;

  for (int i = 0; i < EL_AM; i++) {
    if (mpr121Manager.getTouchVal(i) > 10 && !touchChecked[i]) {
      newPos += i * mpr121Manager.getTouchVal(i);
      newPow += mpr121Manager.getTouchVal(i);
    } else {
      if (newPow != 0) {
        newPos /= newPow;

        createTouch(newPos, newPow);
      }
      newPos = 0;
      newPow = 0;
    }
  }
  render();
}

Touch* TouchbarManager::createTouch(float pos, float pow) {
  for (uint8_t i = 0; i < MAX_TOUCH_AM; i++) {
    if (touches[i].active) continue;
    touches[i].init(hue);
    hue += 137.508;
    if (hue >= 360) hue -= 360;
    touches[i].pos = pos;
    touches[i].pow = pow;
    return &touches[i];
  }
  return NULL;
}

Touch* TouchbarManager::findTouchOnPos(Touch* t) {
  for (uint8_t i = 0; i < MAX_TOUCH_AM; i++) {
    if (t == &touches[i]) continue;
    if (!touches[i].active) continue;

    if ((int)touches[i].pos == (int)t->pos) return &touches[i];
  }
  return NULL;
}

void TouchbarManager::render() {
  for (uint8_t j = 0; j < MAX_TOUCH_AM; j++) {
    if (!touches[j].active) continue;

    float spread = touches[j].pow * 0.5;
    float position = TOUCH_LED_LEN - TOUCH_SEG_LEN * touches[j].pos * 0.75 - TOUCH_LED_LEN / 2 - TOUCH_SEG_LEN / 2;

    if (touches[j].pos > 23) position += TOUCH_LED_LEN;
    int startLed = position - spread / 2;
    int endLed = position + spread / 2;

    for (int i = startLed; i <= endLed; i++) {
      if (i >= 0 && i < TOUCH_LED_LEN) {
        float dist = constrain(1 - abs(i - position) / spread * 2, 0, 1);
        Color col(touches[j].hue, 100, 100, HSB_MODE);
        Color c = ledManager.getPixel(TOUCH_LED_START + i);
        c.add(col, dist);
        ledManager.setPixel(TOUCH_LED_START + i, c);
      }
    }
  }
}

TouchbarManager touchbarManager;