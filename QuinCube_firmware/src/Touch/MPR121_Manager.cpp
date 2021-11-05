#include "MPR121_Manager.h"

#include <i2c_t3.h>

void MPR121_Manager::init() {
  pinMode(18, OUTPUT);
  digitalWrite(18, HIGH);
  Wire.begin(I2C_MASTER, 0, 0, 19, 18, I2C_PULLUP_EXT, 100000, I2C_OP_MODE_ISR);

  for (int i = 0; i < SENSOR_AM; i++) {
    writeRegister(sensorAddr[i], MPR121_SOFTRESET, 0x63);
    writeRegister(sensorAddr[i], MPR121_CONFIG1, 0xFF);
    writeRegister(sensorAddr[i], MPR121_CONFIG2, 0x38);
    writeRegister(sensorAddr[i], MPR121_ECR, 0x4C);
  }
}

void MPR121_Manager::update() {
  if (Wire.done()) {
    readSensData(sensorAddr[currSensor]);

    currSensor++;
    if (currSensor >= SENSOR_AM) currSensor = 0;

    requestSensData(sensorAddr[currSensor]);

    if (currSensor == 0) {
      processSensData();
    }
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

  if (sincePrint > 1000) {
    sincePrint = 0;
    // for (uint8_t i = 0; i < ELEC_AM; i++) {
    //   Serial.print(touchVals[i]);
    //   Serial.print('\t');
    // }
    Serial.println(loopCounter);
    Serial.println();
    loopCounter = 0;
  }
}

bool MPR121_Manager::readSensData(byte addr) {
  if (Wire.available() < 24)
    return false;
  for (int i = 0; i < 12; i++) {
    uint16_t v = Wire.read();
    v |= ((uint16_t)Wire.read()) << 8;
    touchRaw[i + currSensor * 12] = v;
  }
  return true;
}

void MPR121_Manager::requestSensData(byte addr) {
  Wire.beginTransmission(addr);
  Wire.write(MPR121_FILTDATA_0L);
  Wire.endTransmission(false);
  Wire.sendRequest(addr, 24, I2C_STOP);
}

byte MPR121_Manager::writeRegister(uint8_t addr, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission();
}

MPR121_Manager mpr121Manager;
