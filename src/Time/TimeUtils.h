#pragma once

#include <Arduino.h>

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

int getWeekDay(unsigned long val) {  // 0 is sunday, 1 is monday, etc...
  return (val / 60 / 60 / 24 + 4) % 7;
}
int getHours(unsigned long val) {
  return val / 60 / 60 - (val / 60 / 60 / 24 * 24);
}
int getMinutes(unsigned long val) {
  return val / 60 - (val / 60 / 60 * 60);
}
int getSeconds(unsigned long val) {
  return val - (val / 60 * 60);
}

void printTime(long t) {
  Serial.print(getHours(t));
  printDigits(getMinutes(t));
  printDigits(getSeconds(t));
  Serial.println();
}