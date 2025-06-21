#ifndef BATTERY_H
#define BATTERY_H

#include <TFT_eSPI.h>

int getBatteryPercentage();
void drawBattery(TFT_eSPI* gfx, int percentage);

#endif // BATTERY_H 