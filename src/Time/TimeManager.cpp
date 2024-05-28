#include "TimeManager.h"

#include "TimeUtils.h"

elapsedMillis sinceAlarmCheck = 0;

void TimeManager::init(void (*_enableCB)(), void (*_disableCB)()) {
  enableCB = _enableCB;
  disableCB = _disableCB;
}

void TimeManager::update() {
  if (sinceAlarmCheck < 1000) return;
  sinceAlarmCheck = 0;

  int weekday = getWeekDay(now());
  if (weekday == 6 || weekday == 0) {  // if it's saturday or sunday
    disableCB();
    return;
  }

  unsigned long currT = now() % (60 * 60 * 24);
  if (onTime < offTime) {
    if (currT > onTime && currT < offTime)
      enableCB();
    else
      disableCB();
  } else {
    if (currT > offTime && currT < onTime)
      disableCB();
    else
      enableCB();
  }

  if (sincePrint > 1000) {
    sincePrint = 0;
    printCurrTime();
  }
}

unsigned long TimeManager::now() {
  return Teensy3Clock.get() + offset;
}
void TimeManager::setTime(unsigned long t) {
  Teensy3Clock.set(t);
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

TimeManager timeManager;