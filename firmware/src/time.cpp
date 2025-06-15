#include "time.h"

static int hour = 12, minute = 0;
static const char* weekday = "Tue";
static int day = 18, month = 6, year = 2025;

void initTime() {
  // future: sync with RTC or NTP
}

void updateTime() {
  minute++;
  if (minute >= 60) {
    minute = 0;
    hour = (hour + 1) % 24;
  }
}

int getHour()    { return hour; }
int getMinute()  { return minute; }
const char* getWeekday() { return weekday; }

Date getDate() {
  return { day, month, year, weekday };
}
