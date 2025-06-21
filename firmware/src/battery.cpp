#include "battery.h"
#include <TFT_eSPI.h>

extern TFT_eSPI tft; // Use the global tft object from main.cpp

int getBatteryPercentage() {
  // This is a placeholder.
  // You would typically read a voltage from an ADC pin and map it to a percentage.
  // For example: analogRead(ADC_PIN) -> map to 0-100
  // Let's cycle the battery percentage for demonstration
  static int percentage = 100;
  static long last_update = 0;
  if (millis() - last_update > 30000) { // Drop 1% every 30 seconds
    last_update = millis();
    percentage--;
    if (percentage < 0) percentage = 100;
  }
  return percentage;
}

void drawBattery(TFT_eSPI* gfx, int percentage) {
  // Position battery in top-right area, moved to be safely inside the circle
  int x = 170;
  int y = 25;
  int width = 25;
  int height = 12;
  
  gfx->drawRect(x, y, width, height, TFT_WHITE);
  gfx->fillRect(x + 2, y + 2, (width - 4) * percentage / 100, height - 4, TFT_WHITE);
  gfx->fillRect(x + width, y + 3, 3, height - 6, TFT_WHITE);
} 