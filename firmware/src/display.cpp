#include <TFT_eSPI.h>
#include "display.h"
#include "time.h"

TFT_eSPI tft = TFT_eSPI();

void initDisplay() {
  tft.init();
  tft.setRotation(0);
}

void bootAnimation() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(40, 100);
  tft.print("digital-watch");
  delay(500);
}

void drawHomeScreen(int hour, int minute, Date date, int battery) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(4);
  tft.setCursor(40, 40);
  tft.printf("%02d:%02d", hour, minute);

  tft.setTextSize(2);
  tft.setCursor(40, 90);
  tft.printf("%s, %02d/%02d/%d", date.weekday, date.month, date.day, date.year);

  tft.setCursor(40, 130);
  tft.setTextColor(TFT_GREEN);
  tft.printf("Battery: %d%%", battery);
}
