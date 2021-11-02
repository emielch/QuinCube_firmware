#pragma once

#include <Arduino.h>
#include <Color.h>
#include <MultiWS2811.h>

#include "config.h"

class LEDManager {
 public:
  void init();
  void update();
  void setVoxel(int x, int y, int z, byte r, byte g, byte b);
  void setVoxel(int x, int y, int z, Color c);
  Color getVoxel(int x, int y, int z);
  void setPixel(int id, byte r, byte g, byte b);
  void setPixel(int id, Color c);
  Color getPixel(int id);

 private:
  MultiWS2811 *leds;
  int getVoxelId(int x, int y, int z);
  uint32_t bufsize = LEDS_PER_CHANNEL * 384;
  void resetLEDs();
};

extern LEDManager ledManager;
