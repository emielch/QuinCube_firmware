#include "TimeManager.h"

#include "DemoManager.h"
#include "Touch/TouchbarManager.h"

elapsedMillis sinceAlarmCheck = 0;

void TimeManager::update() {
  if (sinceAlarmCheck < 1000) return;
  sinceAlarmCheck = 0;

  if (touchbarManager.getSinceTouched() < 10000) {
    demoManager.enableDemo();
  } else {
    int weekday = getWeekDay(now());
    if (weekday == 6 || weekday == 0) {  // if it's saturday or sunday
      demoManager.disableDemo();
      return;
    }

    unsigned long currT = now() % (60 * 60 * 24);
    if (onTime < offTime) {
      if (currT > onTime && currT < offTime)
        demoManager.enableDemo();
      else
        demoManager.disableDemo();
    } else {
      if (currT > offTime && currT < onTime)
        demoManager.disableDemo();
      else
        demoManager.enableDemo();
    }
  }

  if (sincePrint > 1000) {
    sincePrint = 0;
    printCurrTime();
  }
}

void TimeManager::printCurrTime() {
  Serial.print("Curr: \t");
  printTime(now());
  Serial.print("Weekday: \t");
  Serial.println(getWeekDay(now()));
  Serial.print("ON: \t");
  printTime(onTime);
  Serial.print("OFF: \t");
  printTime(offTime);
  Serial.println();
}

unsigned long TimeManager::now() {
  return Teensy3Clock.get() + offset;
}
void TimeManager::setTime(unsigned long t) {
  Teensy3Clock.set(t);
}

void TimeManager::printTime(long t) {
  Serial.print(getHours(t));
  printDigits(getMinutes(t));
  printDigits(getSeconds(t));
  Serial.println();
}

void TimeManager::printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

int TimeManager::getWeekDay(unsigned long val) {  // 0 is sunday, 1 is monday, etc...
  return (val / 60 / 60 / 24 + 4) % 7;
}
int TimeManager::getHours(unsigned long val) {
  return val / 60 / 60 - (val / 60 / 60 / 24 * 24);
}
int TimeManager::getMinutes(unsigned long val) {
  return val / 60 - (val / 60 / 60 * 60);
}
int TimeManager::getSeconds(unsigned long val) {
  return val - (val / 60 * 60);
}

TimeManager timeManager;