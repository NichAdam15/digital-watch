#pragma once

struct Date {
  int day;
  int month;
  int year;
  const char* weekday;
};

void initTime();
void updateTime();
int getHour();
int getMinute();
const char* getWeekday();
Date getDate();
