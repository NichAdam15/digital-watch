#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include "globals.h"
#include "startup_ui.h"
#include "main_screen.h"
#include "weather_screen.h"
#include "button.h"
#include "network.h"
#include "weather.h"
#include "time_utils.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

ScreenState currentScreen = SCREEN_MAIN;

struct tm timeinfo;
bool timeInitialized = false;

unsigned long lastWeatherUpdate = 0;
const long weatherUpdateInterval = 900000; // 15 minutes

void setup() {
  button_init();
  
  tft.init();
  tft.setRotation(0);
  
  spr.createSprite(240, 240);
  spr.setTextDatum(MC_DATUM);

  // Force WiFi to station mode and disconnect
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  drawStartupSequence(&spr);

  if (WiFi.status() == WL_CONNECTED && getLocalTime(&timeinfo)) {
    timeInitialized = true;
  }
  
  delay(1000); // Longer delay to prevent spurious button press on boot
}

void loop() {
  spr.fillSprite(TFT_BLACK);

  bool screenChanged = button_handle_press(currentScreen);
  if (screenChanged) {
      spr.fillSprite(TFT_BLACK);
      spr.pushSprite(0, 0);
  }

  switch (currentScreen) {
    case SCREEN_MAIN:
      drawMainScreen(&spr, &timeinfo, timeInitialized);
      break;
    case SCREEN_WEATHER:
      drawWeatherScreen(&spr, &timeinfo);
      break;
  }
  
  spr.pushSprite(0, 0);

  static unsigned long lastUpdate = 0;

  if (millis() - lastWeatherUpdate > weatherUpdateInterval) {
    lastWeatherUpdate = millis();
    updateWeatherData();
  }

  delay(100);
} 