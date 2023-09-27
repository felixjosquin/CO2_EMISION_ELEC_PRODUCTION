#include <Arduino.h>

#include "date.h"
#include "requestAPI.h"

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

String getStringEndDate() {
  int remainderMin = now->min % 15;
  DATE endDate = { .year = now->year, .month = now->month, .day = now->day, .hour = now->hour, .min = now->min - remainderMin, .timezone = now->timezone };
  // if (remainderMin < 6 && startDate.hour != 0) {
  //   if (startDate.min != 0) {
  //     startDate.min -= 15;
  //   } else {
  //     startDate.min = 45;
  //     startDate.hour -= 1;
  //   }
  // }
  return endDate.getString();
}

String getStringMidnigth() {
  DATE today = { .year = now->year, .month = now->month, .day = now->day, .hour = 0, .min = 0, .timezone = now->timezone };
  return today.getString();
}

String getStringForNextDay() {
  DATE nextDay = { .year = now->year, .month = now->month, .day = now->day + 1, .hour = 0, .min = 0, .timezone = now->timezone };
  if (nextDay.day == 30) {
    nextDay.day = 1;
    nextDay.month = nextDay.month + 1;
    if (nextDay.month == 12) {
      nextDay.month = 1;
      nextDay.year = nextDay.year + 1;
    }
  }
  return nextDay.getString();
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

void addTime(int delta) {
  now->min += delta;
  if (now->min >= 60) {
    now->hour++;
    now->min -= 60;
    if (now->hour % 4 == 0) {
      tryAndRetry(REQUEST_DATE);
    }
  }
}

void printDatetime() {
  Serial.print("!!!!!!! ");
  Serial.print(now->getString());
  Serial.println(" !!!!!!!");
}