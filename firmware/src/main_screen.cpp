#include "main_screen.h"
#include "weather.h"
#include "battery.h"
#include "time_utils.h"

void drawMainScreen(TFT_eSprite* spr, struct tm* timeinfo, bool timeInitialized) {
  drawWeather(spr);

  if (timeInitialized) {
    if (getLocalTime(timeinfo)) {
      int hour12 = timeinfo->tm_hour % 12;
      if (hour12 == 0) hour12 = 12;

      char timeStr[6];
      sprintf(timeStr, "%d:%02d", hour12, timeinfo->tm_min);
      const char* ampm = (timeinfo->tm_hour < 12) ? "AM" : "PM";

      spr->setTextDatum(MC_DATUM);
      spr->setTextSize(7);
      spr->setTextColor(TFT_WHITE);
      spr->drawString(timeStr, 120, 133);

      spr->setTextSize(2);
      spr->setTextColor(TFT_SILVER);
      spr->drawString(ampm, 120, 170);

      spr->setTextDatum(MC_DATUM);
      char dateStr[16];
      sprintf(dateStr, "%s, %s %d", 
              getDayString(timeinfo->tm_wday), 
              getMonthString(timeinfo->tm_mon + 1), 
              timeinfo->tm_mday);
      spr->drawString(dateStr, 120, 200);
    }
  } else {
    static unsigned long startTime = millis();
    unsigned long elapsedSeconds = (millis() - startTime) / 1000;
    int currentHour = (elapsedSeconds / 3600) % 24;
    int currentMinute = (elapsedSeconds / 60) % 60;

    int hour12 = currentHour % 12;
    if (hour12 == 0) hour12 = 12;
    char timeStr[6];
    sprintf(timeStr, "%d:%02d", hour12, currentMinute);
    const char* ampm = (currentHour < 12) ? "AM" : "PM";

    spr->setTextDatum(MC_DATUM);
    spr->setTextSize(7);
    spr->setTextColor(TFT_WHITE);
    spr->drawString(timeStr, 120, 133);

    spr->setTextSize(2);
    spr->setTextColor(TFT_SILVER);
    spr->drawString(ampm, 120, 170);
    
    spr->drawString("Mon, Jan 1", 120, 200);
  }

  int batt = getBatteryPercentage();
  drawBattery(spr, batt);
} 