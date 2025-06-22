#ifndef NETWORK_H
#define NETWORK_H

#include <TFT_eSPI.h>

// WiFi credentials
extern const char* ssid;
extern const char* password;

bool connectToWiFi(TFT_eSprite* spr);
bool syncTime(TFT_eSprite* spr, struct tm* timeinfo);

#endif // NETWORK_H 