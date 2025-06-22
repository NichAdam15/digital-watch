#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include <TFT_eSPI.h>
#include <time.h>

void drawMainScreen(TFT_eSprite* spr, struct tm* timeinfo, bool timeInitialized);

#endif // MAIN_SCREEN_H 