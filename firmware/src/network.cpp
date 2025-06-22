#include "network.h"
#include <WiFi.h>
#include <time.h>

// WiFi credentials
const char* ssid = "Wildcat";
const char* password = "purdue0810121416";

// NTP Server settings
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -18000;
const int daylightOffset_sec = 3600;

bool connectToWiFi(TFT_eSprite* spr) {
  spr->setTextSize(1);
  spr->drawString("Connecting WiFi", 120, 120);
  spr->pushSprite(0, 0);
  
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    spr->fillRect(100, 130, 40, 10, TFT_BLACK); // Clear dots area
    for(int i = 0; i <= attempts % 4; i++) {
      spr->drawString(".", 110 + i * 10, 130);
    }
    spr->pushSprite(0, 0);
    attempts++;
  }
  return WiFi.status() == WL_CONNECTED;
}

bool syncTime(TFT_eSprite* spr, struct tm* timeinfo) {
  spr->fillRect(0, 110, 240, 40, TFT_BLACK);
  spr->setTextColor(TFT_WHITE);
  spr->drawString("Syncing Time...", 120, 120);
  spr->pushSprite(0, 0);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  int timeAttempts = 0;
  while (!getLocalTime(timeinfo) && timeAttempts < 10) {
    delay(1000);
    spr->fillRect(100, 130, 40, 10, TFT_BLACK); // Clear dots area
    for(int i = 0; i <= timeAttempts % 4; i++) {
      spr->drawString(".", 110 + i * 10, 130);
    }
    spr->pushSprite(0, 0);
    timeAttempts++;
  }
  return getLocalTime(timeinfo);
} 