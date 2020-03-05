#include <EEPROM.h>
#include <Colore.h>
#include "Orb.h"
#include <Colore.h>
#include <MultiWS2811.h>

const int ledsPerStrip = 125;

DMAMEM int displayMemory[ledsPerStrip * 96];
int drawingMemory[ledsPerStrip * 96];

const int config = WS2811_GRB | WS2811_800kHz;

MultiWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);



unsigned int screenSaverDelay = 2000;
unsigned int demoDelay = 10000;
elapsedMillis sinceNewFrame = screenSaverDelay + 1;

bool demoRunning = false;
bool enableDemo = true;
elapsedMillis sinceDemoStart = 0;

int touchSampleCounter = 0;


void setup()
{
  sinceNewFrame = demoDelay + 1;
  touchSetup();
  demoSetup();
  Serial.setTimeout(50);
  leds.begin();
}


void loop() {
  if (sinceNewFrame > demoDelay && enableDemo) {
    if (!demoRunning) {
      resetDemo();
      demoRunning = true;
      elapsedMillis sinceDemoStart = 0;
    }
    demoLoop();
  } else if (sinceNewFrame > screenSaverDelay) {
    //renderInterrupt();
    reset_ledLib();
    leds.show();
  }

  checkUSBSerial();
}

void renderInterrupt() {
  touchUpdate();
}


void checkUSBSerial() {
  int startChar = Serial.read();

  if (startChar == '%') {
    // receive a frame
    int count = Serial.readBytes((char *)drawingMemory, sizeof(drawingMemory));
    if (count == sizeof(drawingMemory)) {
      leds.show();
      sinceNewFrame = 0;
      if (demoRunning) {
        demoRunning = false;
      }
    }

  }
  else if (startChar == '?') {
    // when the video application asks, give it all our info
    // for easy and automatic configuration
    Serial.print("CUBE");
    Serial.write(',');
    Serial.print(cubeSize);
    Serial.write(',');
    Serial.print(ledsPerStrip);
    Serial.println();
  }
  else if (startChar == 'd') {
    enableDemo = true;
  }
  else if (startChar == 'x') {
    enableDemo = false;
    demoRunning = false;
  }
  else if (startChar >= 0) {
    // discard unknown characters
  }
}
