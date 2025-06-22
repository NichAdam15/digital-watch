#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "globals.h"

void button_init();
bool button_handle_press(ScreenState& currentScreen);

#endif // BUTTON_H 