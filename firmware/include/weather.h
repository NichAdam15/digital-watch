#ifndef WEATHER_H
#define WEATHER_H

#include <TFT_eSPI.h>

void updateWeatherData();
void drawWeather(TFT_eSPI* gfx);
bool isWeatherDataValid();
int getTempHigh();
int getTempLow();
String getWeatherCondition();
int getTemperature();
String getWeatherIcon();
void drawWeatherIcon(TFT_eSPI* gfx, String icon, int x, int y, int size);
String getLocation();
uint16_t getBackgroundColor();
int getLastWeatherErrorCode();
extern const char* city;

#endif // WEATHER_H 