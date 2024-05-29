#pragma once

#include <Arduino.h>

#define ELEC_AM 36   // amount of electrodes
#define SENSOR_AM 3  // amount of MPR121 sensors

class MPR121_Manager {
 public:
  void init();
  void update();
  float getTouchVal(int i) { return touchVals[i]; }

 private:
  elapsedMillis sincePrint = 0;

  byte sensorAddr[SENSOR_AM] = {0x5A, 0x5B, 0x5C};
  byte currSensor = 0;  // the sensor that is currently being read. (we will read each sensor sequentially)

  float touchVals[ELEC_AM];
  float touchBaseline[ELEC_AM];
  int touchRaw[ELEC_AM];

  void processSensData();
  bool readSensData(byte addr);

  byte writeRegister(uint8_t addr, uint8_t reg, uint8_t value);
  int loopCounter = 0;
};

extern MPR121_Manager mpr121Manager;
