#include <i2c_t3.h>

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

byte i2caddr_init = 90;
byte currMPR121 = 0;
byte i2caddr = 90;



float touchVals[SENS_AM];
float touchBaseline[SENS_AM];
int touchRaw[SENS_AM];

#define TOUCH_UPDATE_INTERVAL 333333/134
elapsedMicros sinceTouchUpdate;


void sensSetup() {
  //while (!Serial);
  pinMode(18, OUTPUT);
  digitalWrite(18, HIGH);
  Wire.begin();
  for (currMPR121 = 0; currMPR121 < 3; currMPR121++) {
    i2caddr = i2caddr_init + currMPR121;
    writeRegister(MPR121_SOFTRESET, 0x63);
    writeRegister(MPR121_CONFIG1, 0x3F);
    writeRegister(MPR121_CONFIG2, 0x38);

    writeRegister(MPR121_ECR, 0x8C);
  }

}


void sensUpdate() {
  if (sinceTouchUpdate < TOUCH_UPDATE_INTERVAL) return;
  sinceTouchUpdate = 0;

  if (Wire.done()) {
    readSensData();

    currMPR121++;
    if (currMPR121 == 3) currMPR121 = 0;
    i2caddr = i2caddr_init + currMPR121;

    requestSensData();

    if (currMPR121 == 0) {
        processSensData();
        updateTouch();
    }

  }
}

elapsedMillis sinceTouchPrint = 0;

void processSensData() {
  touchSampleCounter++;
  for (uint8_t i = 0; i < SENS_AM; i++) {

    if(touchBaseline[i] < touchRaw[i]-400)
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

  if (sinceTouchPrint < 100000) return;
  sinceTouchPrint = 0;
  for (uint8_t i = 0; i < 36; i++) {
    Serial.print(touchVals[i]);
    Serial.print('\t');
  }
  Serial.println();
  Serial.println();
}


void requestSensData() {
  Wire.beginTransmission(i2caddr);
  Wire.write(MPR121_FILTDATA_0L);
  Wire.endTransmission(false);
  Wire.sendRequest(i2caddr, 24, I2C_STOP);
}

bool readSensData() {
  if (Wire.available() < 24)
    return false;
  for (int i = 0; i < 12; i++) {
    uint16_t v = Wire.read();
    v |= ((uint16_t)Wire.read()) << 8;
    touchRaw[i + currMPR121 * 12] = v;
  }
  return true;
}

void printAllRegs() {
  for (uint8_t i = 0; i < 0x7F; i++) {
    Serial.print("$"); Serial.print(i, HEX);
    Serial.print(": 0x"); Serial.println(readRegister8(i), HEX);
  }
}

byte writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(i2caddr);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission();
}


uint8_t readRegister8(uint8_t reg) {
  Wire.beginTransmission(i2caddr);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(i2caddr, 1);
  if (Wire.available() < 1)
    return 0;
  return Wire.read();
}
