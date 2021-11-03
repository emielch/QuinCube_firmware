#include "TouchbarManager.h"

#include <Color.h>

#include "LedManager.h"
#include "MPR121_Manager.h"
#include "config.h"

void TouchbarManager::init() {
  scale = 1;
}

void TouchbarManager::update() {
  updateTouches();
  updateMovementValues();
  render();
}

void TouchbarManager::updateTouches() {
  bool elecChecked[ELEC_AM] = {false};  // to keep track of which electrodes are checked already

  ////// update any existing touches
  for (int i = 0; i < MAX_TOUCH_AM; i++) {
    if (!touches[i].active) continue;

    float newPos = 0, newPow = 0;

    /// search for "touching" electrodes down and up from the center of the current touch
    for (int d = -1; d < 2; d += 2) {                               // do once for d=-1 and once for d=1 (direction down and direction up)
      int pos = touches[i].pos;                                     // start at the center when searching downwards
      if (d == 1) pos += 1;                                         // start at the center +1 when searching upwards
      while ((pos >= 0 && d == -1) || (pos < ELEC_AM && d == 1)) {  // keep searching till we hit pos=0 when searching down, and till pos=ELEC_AM-1 when searching up
        elecChecked[pos] = true;                                    // mark the electrode as checked (no need to search for new touches here later)
        if (mpr121Manager.getTouchVal(pos) > 1) {                   // if the electrode is being touched and thus still part of the previous touch
          newPos += pos * mpr121Manager.getTouchVal(pos);           // add up the electrode's position, weighted by its current power
          newPow += mpr121Manager.getTouchVal(pos);                 // add up the electrode's power
          pos += d;                                                 // continue searching in the current direction
        } else                                                      // if the electrode is not being touched,
          break;                                                    // stop searching in this direction
      }
    }

    if (newPow != 0) {   // if we found electrodes that were being touched in the vicinity of the previous touch
      newPos /= newPow;  // calculate the avg weighted position
      touches[i].newPos(newPos);
      touches[i].pow = touches[i].pow * 0.5 + newPow * 0.5;  // smoothly transition the touch's power to its new current power

      if (findTouchOnPos(&touches[i]))  // deactivate if any other touch happens to exist on this touch's new position  (maybe implement a "merging" animation?)
        touches[i].active = false;
    } else
      touches[i].active = false;  // deactivate the touch if no electrodes are being touched in its vicinity
  }

  ///// look for any new touches on the electrodes that haven't been checked already during the updating of the existing touches
  float newPos = 0, newPow = 0;
  for (int i = 0; i <= ELEC_AM; i++) {                                          // keep looping through one extra at the end...
    if (i < ELEC_AM && mpr121Manager.getTouchVal(i) > 10 && !elecChecked[i]) {  // ... and skip directly over it into the else to register a new touch on the last electrode
      newPos += i * mpr121Manager.getTouchVal(i);                               // add up the position weighted by touch power
      newPow += mpr121Manager.getTouchVal(i);
    } else {
      if (newPow != 0) {
        newPos /= newPow;  // take the average position, weighted by power
        createTouch(newPos, newPow);
      }
      newPos = newPow = 0;
    }
  }
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
    float TbLedPos = TOUCH_SEG_LEN * touches[j].pos * 0.75;  // convert to pixel position on the touchbar (origin in back left corner)

    int startLed = TbLedPos - spread / 2;
    int endLed = TbLedPos + spread / 2;

    for (int i = startLed; i <= endLed; i++) {
      //if (pos < 0 || pos > TOUCH_LED_LEN - 1) continue;  // commented out as we allow it to wrap around the ends of the range
      float dist = constrain(1 - abs(i - TbLedPos) / spread * 2, 0, 1);
      Color col(touches[j].hue, 100, 100, HSB_MODE);
      Color c = ledManager.getPixel(TOUCH_LED_START + i);
      c.add(col, dist);
      ledManager.setPixelTB(i, c);
    }
  }
}

void TouchbarManager::updateMovementValues() {
  /// deccelerate all speeds
  spd.mult(0.97);
  rotSpd *= 0.97;
  scaleSpd *= 0;

  //// sort and count the touches into their sides of the touchbar ////
  Touch *leftTouches[10], *frontTouches[10], *rightTouches[10];
  int leftTouchAm, frontTouchAm, rightTouchAm;
  leftTouchAm = frontTouchAm = rightTouchAm = 0;

  for (int i = 0; i < MAX_TOUCH_AM; i++) {
    if (!touches[i].active) continue;
    if (touches[i].pos < 12)
      leftTouches[leftTouchAm++] = &touches[i];
    else if (touches[i].pos < 24)
      frontTouches[frontTouchAm++] = &touches[i];
    else
      rightTouches[rightTouchAm++] = &touches[i];
  }

  //// udpate the movement ////
  if (leftTouchAm == 1) spd.z = -leftTouches[0]->movedDist() * 2;
  if (leftTouchAm == 2) spd.y = leftTouches[0]->movedDist() + leftTouches[1]->movedDist();

  if (frontTouchAm == 1) spd.x = frontTouches[0]->movedDist() * 2;
  if (frontTouchAm == 2) {
    if (frontTouches[0]->pos > frontTouches[1]->pos)
      scaleSpd = 0.1 * (frontTouches[0]->movedDist() - frontTouches[1]->movedDist());
    else
      scaleSpd = 0.1 * (frontTouches[1]->movedDist() - frontTouches[0]->movedDist());
  }

  if (rightTouchAm == 1) rotSpd = rightTouches[0]->movedDist() * 0.2;
  if (rightTouchAm == 2) rotSpd = rightTouches[0]->movedDist() * 0.1 + rightTouches[1]->movedDist() * 0.1;

  /// update the transform (not used anywhere now but could be useful to stream to Unity, or other processes that don't run at the same frequency)
  pos.add(spd);
  rot += rotSpd;
  scale += scaleSpd;
}

TouchbarManager touchbarManager;