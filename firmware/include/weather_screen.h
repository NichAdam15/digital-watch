#ifndef WEATHER_SCREEN_H
#define WEATHER_SCREEN_H

#include <TFT_eSPI.h>
#include <time.h>

void drawWeatherScreen(TFT_eSprite* spr, struct tm* timeinfo);

#endif // WEATHER_SCREEN_H 