#include <Arduino.h>

#include "date.h"

String addZero(int val);

DATE *now = new DATE;

void updateDateNow(String dateString) {
  int year = dateString.substring(1, 5).toInt();
  int month = dateString.substring(6, 8).toInt();
  int day = dateString.substring(9, 11).toInt();
  int hour = dateString.substring(12, 14).toInt();
  int min = dateString.substring(15, 17).toInt();
  int timezone = dateString.substring(28, 30).toInt();
  now->year = year;
  now->month = month;
  now->day = day;
  now->hour = hour;
  now->min = min;
  now->timezone = timezone;
}

String getStringNow() {
  return now->getString();
}

String getStringTomorrow() {
  DATE tommorow = { .year = now->year, .month = now->month, .day = now->day - 1, .hour = 0, .min = 0, .timezone = now->timezone };
  if (tommorow.day == 0) {
    tommorow.day = 30;
    tommorow.month = tommorow.month - 1;
    if (tommorow.month == 0) {
      tommorow.month = 12;
      tommorow.year = tommorow.year - 1;
    }
  }
  return tommorow.getString();
}

String DATE::getString() {
  String date = String(this->year) + "-" + addZero(this->month) + "-" + addZero(this->day);
  String time = addZero(this->hour) + ":" + addZero(this->min) + ":00+" + addZero(this->timezone) + ":00";
  return date + "T" + time;
}

String addZero(int val) {
  if (val < 10) {
    return "0" + String(val);
  }
  return String(val);
}