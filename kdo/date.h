#include <Arduino.h>

#ifndef DATES_H
#define DATES_H

typedef struct {
  int year;
  int month;
  int day;
  int hour;
  int min;
  int timezone;

  String getString();
  void display();
} DATE;

void updateDateNow(String dateString);

String getStringStartDate();
String getStringMidnigth();
String getStringForNextDay();

#endif