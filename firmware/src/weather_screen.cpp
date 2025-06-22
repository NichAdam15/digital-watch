#include "weather_screen.h"
#include "weather.h"
#include "time_utils.h"

void drawWeatherScreen(TFT_eSprite* spr, struct tm* timeinfo) {
  if (!isWeatherDataValid()) {
    spr->fillSprite(TFT_BLACK); // Default background if no data
    spr->setTextDatum(MC_DATUM);
    spr->setTextColor(TFT_WHITE);
    spr->setTextSize(2);
    spr->drawString("Weather data not available", 120, 120);
    return;
  }

  // Set background color based on weather
  uint16_t bgColor = getBackgroundColor();
  spr->fillSprite(bgColor);

  // --- Draw Weather Info ---

  // Current Temperature (Top Left)
  spr->setTextDatum(TL_DATUM); // Top-Left datum
  spr->setTextSize(8);
  spr->setTextColor(TFT_WHITE);
  char tempStr[8];
  sprintf(tempStr, "%d", getTemperature());
  spr->drawString(tempStr, 25, 30);
  spr->drawCircle(spr->getCursorX() + 10, 42, 5, TFT_WHITE); // Manual degree circle

  // High / Low Temperature (Below Current Temp)
  spr->setTextSize(3);
  spr->setTextColor(TFT_WHITE, bgColor);
  char highLowStr[12];
  sprintf(highLowStr, "%d / %d", getTempHigh(), getTempLow());
  spr->drawString(highLowStr, 30, 95);
  spr->drawCircle(spr->getCursorX() - 55, 99, 3, TFT_WHITE); // Manual degree for high
  spr->drawCircle(spr->getCursorX() + 5, 99, 3, TFT_WHITE); // Manual degree for low

  // Weather Icon (Top Right)
  drawWeatherIcon(spr, getWeatherIcon(), 175, 70, 80);

  // --- Draw Location and Date (Bottom) ---
  spr->setTextDatum(MC_DATUM); // Middle-Center datum

  // Location
  spr->setTextSize(3);
  spr->setTextColor(TFT_WHITE);
  spr->drawString(getLocation(), 120, 185);
  
  // Date
  spr->setTextSize(2);
  char dateStr[20];
  sprintf(dateStr, "%s, %s %d", 
          getDayString(timeinfo->tm_wday), 
          getMonthString(timeinfo->tm_mon + 1), 
          timeinfo->tm_mday);
  spr->drawString(dateStr, 120, 210);
} 