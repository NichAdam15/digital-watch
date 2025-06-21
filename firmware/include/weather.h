#ifndef WEATHER_H
#define WEATHER_H

#include <TFT_eSPI.h>

void updateWeatherData();
void drawWeather(TFT_eSPI* gfx);
bool isWeatherDataValid();

#endif // WEATHER_H 