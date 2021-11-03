#include "SerialStreamManager.h"

#include "DemoManager.h"
#include "LedManager.h"
#include "config.h"

void SerialStreamManager::init() {
  Serial.setTimeout(50);
}

void SerialStreamManager::update() {
  int startChar = Serial.read();

  if (startChar == '%') {
    // receive a frame
    int count = Serial.readBytes((char *)drawingMemory, sizeof(drawingMemory));
    if (count == sizeof(drawingMemory)) {
      ledManager.show();
      sinceNewFrame = 0;
    }

  } else if (startChar == '?') {
    // when the video application asks, give it all our info
    // for easy and automatic configuration
    Serial.print("CUBE");
    Serial.write(',');
    Serial.print(CUBE_SIZE);
    Serial.write(',');
    Serial.print(LEDS_PER_CHANNEL);
    Serial.println();
  } else if (startChar == 'd') {
    demoManager.enableDemo();
  } else if (startChar == 'x') {
    demoManager.disableDemo();
  } else if (startChar == 'r') {
    demoManager.switchAnim(Rainbow);
  } else if (startChar == 'o') {
    demoManager.switchAnim(Orbs);
  } else if (startChar == '=') {
    demoManager.adjBri(1);
  } else if (startChar == '-') {
    demoManager.adjBri(-1);
  } else if (startChar >= 0) {
    // discard unknown characters
  }
}

SerialStreamManager serialStreamManager;