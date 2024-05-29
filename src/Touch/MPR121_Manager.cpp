#include "MPR121_Manager.h"

#include <Wire.h>

enum {
  MPR121_TOUCHSTATUS_L = 0x00,
  MPR121_TOUCHSTATUS_H = 0x01,
  MPR121_FILTDATA_0L = 0x04,
  MPR121_FILTDATA_0H = 0x05,
  MPR121_BASELINE_0 = 0x1E,
  MPR121_MHDR = 0x2B,
  MPR121_NHDR = 0x2C,
  MPR121_NCLR = 0x2D,
  MPR121_FDLR = 0x2E,
  MPR121_MHDF = 0x2F,
  MPR121_NHDF = 0x30,
  MPR121_NCLF = 0x31,
  MPR121_FDLF = 0x32,
  MPR121_NHDT = 0x33,
  MPR121_NCLT = 0x34,
  MPR121_FDLT = 0x35,

  MPR121_TOUCHTH_0 = 0x41,
  MPR121_RELEASETH_0 = 0x42,
  MPR121_DEBOUNCE = 0x5B,
  MPR121_CONFIG1 = 0x5C,
  MPR121_CONFIG2 = 0x5D,
  MPR121_CHARGECURR_0 = 0x5F,
  MPR121_CHARGETIME_1 = 0x6C,
  MPR121_ECR = 0x5E,
  MPR121_AUTOCONFIG0 = 0x7B,
  MPR121_AUTOCONFIG1 = 0x7C,
  MPR121_UPLIMIT = 0x7D,
  MPR121_LOWLIMIT = 0x7E,
  MPR121_TARGETLIMIT = 0x7F,

  MPR121_GPIODIR = 0x76,
  MPR121_GPIOEN = 0x77,
  MPR121_GPIOSET = 0x78,
  MPR121_GPIOCLR = 0x79,
  MPR121_GPIOTOGGLE = 0x7A,

  MPR121_SOFTRESET = 0x80,
};

void MPR121_Manager::init() {
  Wire.begin();
  Wire.setClock(1e6);

  for (int i = 0; i < SENSOR_AM; i++) {
    writeRegister(sensorAddr[i], MPR121_SOFTRESET, 0x63);
    writeRegister(sensorAddr[i], MPR121_CONFIG1, 0xFF);
    writeRegister(sensorAddr[i], MPR121_CONFIG2, 0x38);
    writeRegister(sensorAddr[i], MPR121_ECR, 0x4C);
  }
}

void MPR121_Manager::update() {
  readSensData(sensorAddr[currSensor]);

  currSensor++;
  if (currSensor >= SENSOR_AM) currSensor = 0;

  if (currSensor == 0) {
    processSensData();
  }
}

void MPR121_Manager::processSensData() {
  loopCounter++;

  for (uint8_t i = 0; i < ELEC_AM; i++) {
    if (touchBaseline[i] < touchRaw[i] - 400)
      touchBaseline[i] = touchRaw[i];
    else if (touchRaw[i] > touchBaseline[i] - 5)
      touchBaseline[i] = touchBaseline[i] * 0.995 + touchRaw[i] * 0.005;
    else
      touchBaseline[i] = touchBaseline[i] * 0.9999 + touchRaw[i] * 0.0001;

    if (touchVals[i] < 1)
      touchVals[i] = touchVals[i] * 0.9 + (touchBaseline[i] - touchRaw[i]) * 0.1;
    else
      touchVals[i] = touchBaseline[i] - touchRaw[i];
  }

  // if (sincePrint > 1000) {
  //   sincePrint = 0;
  //   for (uint8_t i = 0; i < ELEC_AM; i++) {
  //     Serial.print(touchVals[i]);
  //     Serial.print('\t');
  //   }
  //   Serial.println();
  //   Serial.println(loopCounter);
  //   loopCounter = 0;
  // }
}

bool MPR121_Manager::readSensData(byte addr) {
  Wire.beginTransmission(addr);
  Wire.write(MPR121_FILTDATA_0L);
  Wire.endTransmission(false);
  Wire.requestFrom(addr, (byte)24);

  if (Wire.available() < 24) {
    Serial.println("wire not available");
    return false;
  }
  for (int i = 0; i < 12; i++) {
    uint16_t v = Wire.read();
    v |= ((uint16_t)Wire.read()) << 8;
    touchRaw[i + currSensor * 12] = v;
  }
  return true;
}

byte MPR121_Manager::writeRegister(uint8_t addr, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission();
}

MPR121_Manager mpr121Manager;
