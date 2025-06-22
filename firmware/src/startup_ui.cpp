#include "startup_ui.h"
#include "weather.h"
#include "network.h"
#include <WiFi.h>

void drawStartupSequence(TFT_eSprite* spr) {
  spr->fillSprite(TFT_BLACK);
  
  spr->setTextColor(TFT_WHITE);
  spr->setTextSize(2);
  spr->drawString("Digital Watch", 120, 80);
  spr->pushSprite(0, 0);
  delay(1000);
  
  // Step 1: Connect to WiFi
  spr->fillRect(0, 110, 240, 40, TFT_BLACK);
  spr->setTextColor(TFT_WHITE);
  spr->setTextSize(1);
  spr->drawString("Connecting to WiFi...", 120, 120);
  spr->pushSprite(0, 0);
  
  bool wifiConnected = connectToWiFi(spr);
  
  if (wifiConnected) {
    spr->fillRect(0, 110, 240, 40, TFT_BLACK);
    spr->setTextColor(TFT_GREEN);
    spr->drawString("WiFi Connected!", 120, 120);
    spr->pushSprite(0, 0);
    delay(1500);

    // Step 2: Sync Time
    struct tm timeinfo;
    spr->fillRect(0, 110, 240, 40, TFT_BLACK);
    spr->setTextColor(TFT_WHITE);
    spr->drawString("Syncing Time...", 120, 120);
    spr->pushSprite(0, 0);
    
    bool timeSynced = syncTime(spr, &timeinfo);
    
    if (timeSynced) {
      spr->fillRect(0, 110, 240, 40, TFT_BLACK);
      spr->setTextColor(TFT_GREEN);
      spr->drawString("Time Synced!", 120, 120);
      spr->pushSprite(0, 0);
      delay(1500);

      // Step 3: Fetch Weather
      spr->fillRect(0, 110, 240, 40, TFT_BLACK);
      spr->setTextColor(TFT_WHITE);
      spr->drawString("Fetching Weather...", 120, 120);
      spr->pushSprite(0, 0);
      
      updateWeatherData();
      
      // Wait a moment for the weather data to be processed
      delay(500);
      
      if (isWeatherDataValid()) {
        spr->fillRect(0, 110, 240, 40, TFT_BLACK);
        spr->setTextColor(TFT_GREEN);
        spr->drawString("Weather Fetched!", 120, 120);
      } else {
        spr->fillRect(0, 110, 240, 60, TFT_BLACK);
        spr->setTextColor(TFT_RED);
        spr->drawString("Weather Failed", 120, 120);
        char errStr[20];
        sprintf(errStr, "Error Code: %d", getLastWeatherErrorCode());
        spr->drawString(errStr, 120, 140);
      }
      spr->pushSprite(0, 0);
      delay(1500);

    } else {
      spr->fillRect(0, 110, 240, 40, TFT_BLACK);
      spr->setTextColor(TFT_RED);
      spr->drawString("NTP Failed - Using Fallback", 120, 120);
      spr->pushSprite(0, 0);
      delay(2000);
    }
  } else {
    spr->fillRect(0, 110, 240, 60, TFT_BLACK);
    spr->setTextColor(TFT_RED);
    spr->drawString("WiFi Failed", 120, 120);
    spr->drawString("Check network", 120, 135);
    spr->pushSprite(0, 0);
    delay(3000);
  }

  spr->fillSprite(TFT_BLACK);
  spr->pushSprite(0, 0);
} 