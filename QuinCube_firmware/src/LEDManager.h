#pragma once

#include <Arduino.h>
#include <Color.h>
#include <MultiWS2811.h>

#include "config.h"

extern DMAMEM int displayMemory[LEDS_PER_CHANNEL * 96];
extern int drawingMemory[LEDS_PER_CHANNEL * 96];

class LEDManager {
 public:
  void init();
  void update();
  void show();
  void setVoxel(byte x, byte y, byte z, byte r, byte g, byte b);
  void setVoxel(byte x, byte y, byte z, Color c);
  Color getVoxel(byte x, byte y, byte z);

  void setPixelTB(int pos, Color c);  // set a pixel on the touchbar
  Color getPixelTB(int pos);          // get a pixel on the touchbar

  void setPixel(int id, byte r, byte g, byte b);
  void setPixel(int id, Color c);
  Color getPixel(int id);

 private:
  MultiWS2811 *leds;
  int getVoxelLedId(byte x, byte y, byte z);
  int getTbLedId(int pos);
  uint32_t bufsize = LEDS_PER_CHANNEL * 384;
  void resetLEDs();
};

extern LEDManager ledManager;
