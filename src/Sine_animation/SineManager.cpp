#include "SineManager.h"

#include "LEDManager.h"
#include "Touch/TouchbarManager.h"
#include "config.h"
#include "math8.h"

void SineManager::init(void (*_renderInterrupt)()) {
  renderInterrupt = _renderInterrupt;
}

void SineManager::update() {
  phaseSpd += touchbarManager.getSpd().x * 0.5;
  phaseSpd = constrain(phaseSpd, 0, 10);
  hueOffset += touchbarManager.getSpd().z * 10;
  hueScale += touchbarManager.getRotationSpd() * 10;

  phase += phaseSpd * ledManager.getDt();
  while (phase > 2 * PI) phase -= 2 * PI;

  for (uint8_t x = 0; x < CUBE_SIZE; x++) {
    renderInterrupt();
    // convert cube x to floating point coordinate between x_min and x_max
    float xprime = mapf(x, 0, CUBE_SIZE - 1, x_min, x_max);
    for (uint8_t z = 0; z < CUBE_SIZE; z++) {
      // convert cube z to floating point coordinate between z_min and z_max
      float zprime = mapf(z, 0, CUBE_SIZE - 1, z_min, z_max);
      // determine y floating point coordinate
      float yprime = sinf(phase + sqrtf(xprime * xprime + zprime * zprime));
      // convert floating point y back to cube y
      float y = mapf(yprime, -1, 1, 0, CUBE_SIZE - 1);
      // display voxel on the cube
      //Color c = Color((hue16 >> 8) + y * 10, &RainbowGradientPalette[0]);
      int fy = floor(y);
      int cy = ceil(y);
      for (int _y = fy; _y < cy + 1; _y++) {
        float bri = 1 - abs(y - _y);
        Color col = Color(hueOffset + y * hueScale, 100, 20 * bri, HSB_MODE);
        ledManager.setVoxel(x, _y, z, col);
      }
    }
  }
}

SineManager sineManager;