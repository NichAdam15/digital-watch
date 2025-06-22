#include "weather.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

// --- Live Weather Data ---
const char* city = "Union,KY,US";
const char* api_key = "27eadd3cdeda61121d8e1c3ff798031d";
// -------------------------

static int temperature = 75; // Default fallback temperature
static String weatherCondition = "Clear";
static bool weatherDataValid = false;
static int temp_high = 0;
static int temp_low = 0;
static int lastHttpCode = 0;

void updateWeatherData() {
  // Check if WiFi is connected
  if (WiFi.status() != WL_CONNECTED) {
    weatherDataValid = false;
    return;
  }

  String url = String("http://api.openweathermap.org/data/2.5/weather?q=") + city + "&units=imperial&APPID=" + api_key;

  HTTPClient http;
  http.begin(url);
  http.setTimeout(10000); // 10 second timeout
  int httpCode = http.GET();
  lastHttpCode = httpCode;

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      
      JSONVar myObject = JSON.parse(payload);

      if (JSON.typeof(myObject) != "undefined") {
        // Extract weather data
        if (myObject.hasOwnProperty("main") && myObject["main"].hasOwnProperty("temp")) {
          temperature = (int)round((double)myObject["main"]["temp"]);
          temp_high = (int)round((double)myObject["main"]["temp_max"]);
          temp_low = (int)round((double)myObject["main"]["temp_min"]);
          weatherDataValid = true;
        } else {
          weatherDataValid = false;
        }
        
        if (myObject.hasOwnProperty("weather") && myObject["weather"].hasOwnProperty(0)) {
          String mainCondition = (const char*)myObject["weather"][0]["main"];
          
          // Map API condition to our icons
          if (mainCondition == "Clouds" || mainCondition == "Mist" || mainCondition == "Fog") {
            weatherCondition = "Clouds";
          } else if (mainCondition == "Rain" || mainCondition == "Drizzle" || mainCondition == "Thunderstorm") {
            weatherCondition = "Rain";
          } else {
            weatherCondition = "Clear"; // Default for Clear, etc.
          }
        }
      } else {
        weatherDataValid = false;
      }
    } else {
      weatherDataValid = false;
    }
  } else {
    weatherDataValid = false;
  }
  
  http.end();
}

int getLastWeatherErrorCode() {
  return lastHttpCode;
}

bool isWeatherDataValid() {
  return weatherDataValid;
}

int getTempHigh() {
  return temp_high;
}

int getTempLow() {
  return temp_low;
}

String getWeatherCondition() {
  return weatherCondition;
}

int getTemperature() {
  return temperature;
}

String getWeatherIcon() {
  return weatherCondition;
}

uint16_t getBackgroundColor() {
  if (weatherCondition == "Rain") {
    return TFT_BLUE;
  } else if (weatherCondition == "Clouds") {
    return TFT_DARKGREY;
  } else { // Clear
    return TFT_ORANGE;
  }
}

void drawWeatherIcon(TFT_eSPI* gfx, String icon, int x, int y, int size) {
  if (icon == "Clear") {
    uint16_t iconColor = TFT_YELLOW;
    gfx->fillCircle(x, y, size / 3, iconColor); // Sun center
    // Rays
    for (int i = 0; i < 360; i += 45) {
      float a = i * DEG_TO_RAD;
      int x1 = x + (int)((size / 2.5) * cos(a));
      int y1 = y + (int)((size / 2.5) * sin(a));
      int x2 = x + (int)((size / 2.0) * cos(a));
      int y2 = y + (int)((size / 2.0) * sin(a));
      gfx->drawLine(x1, y1, x2, y2, iconColor);
      gfx->drawLine(x1+1, y1, x2+1, y2, iconColor);
    }
  } else if (icon == "Clouds") {
    uint16_t iconColor = TFT_WHITE;
    gfx->fillCircle(x - size/4, y, size/4, iconColor);
    gfx->fillCircle(x + size/5, y, size/3, iconColor);
    gfx->fillCircle(x, y - size/6, size/4, iconColor);
    gfx->fillCircle(x, y + size/6, size/5, iconColor);
    gfx->fillRect(x - size/4, y-size/6, size/2, size/3, iconColor);
  } else if (icon == "Rain") {
    uint16_t cloudColor = TFT_WHITE;
    int cloudY = y - size/8;
    // Cloud
    gfx->fillCircle(x - size/4, cloudY, size/4, cloudColor);
    gfx->fillCircle(x + size/5, cloudY, size/3, cloudColor);
    gfx->fillCircle(x, cloudY - size/6, size/4, cloudColor);
    gfx->fillCircle(x, cloudY + size/6, size/5, cloudColor);
    gfx->fillRect(x - size/4, cloudY - size/6, size/2, size/3, cloudColor);

    // Rain
    uint16_t rainColor = TFT_WHITE;
    int rainY = y + size/4;
    for (int i=0; i < 4; i++) {
        int rx = x - size/4 + i * (size/6) + 5;
        gfx->drawFastVLine(rx, rainY, 15, rainColor);
        gfx->drawFastVLine(rx+1, rainY, 15, rainColor);
    }
  }
}

String getLocation() {
  return "Union, KY";
}

void drawWeather(TFT_eSPI* gfx) {
  int icon_x = 120;
  int icon_y = 35;

  // Draw weather icon based on condition
  if (weatherCondition == "Clear") {
    // New pixel-art sun with diagonal rays
    gfx->fillRect(icon_x - 10, icon_y - 10, 20, 20, TFT_YELLOW); // Center
    gfx->fillRect(icon_x - 22, icon_y - 2, 10, 4, TFT_YELLOW);   // Left ray
    gfx->fillRect(icon_x + 12, icon_y - 2, 10, 4, TFT_YELLOW);   // Right ray
    gfx->fillRect(icon_x - 2, icon_y - 22, 4, 10, TFT_YELLOW);   // Top ray
    gfx->fillRect(icon_x - 2, icon_y + 12, 4, 10, TFT_YELLOW);   // Bottom ray
    // Diagonal rays
    gfx->fillRect(icon_x - 15, icon_y - 15, 6, 6, TFT_YELLOW);   // Top-left
    gfx->fillRect(icon_x + 9, icon_y - 15, 6, 6, TFT_YELLOW);    // Top-right
    gfx->fillRect(icon_x - 15, icon_y + 9, 6, 6, TFT_YELLOW);    // Bottom-left
    gfx->fillRect(icon_x + 9, icon_y + 9, 6, 6, TFT_YELLOW);     // Bottom-right
  } else if (weatherCondition == "Clouds") {
    // New pixel-art cloud
    gfx->fillRect(icon_x - 20, icon_y, 40, 12, TFT_WHITE);
    gfx->fillRect(icon_x - 10, icon_y - 8, 24, 8, TFT_WHITE);
    gfx->fillRect(icon_x - 15, icon_y + 12, 30, 6, TFT_WHITE);
  } else if (weatherCondition == "Rain") {
    // New pixel-art cloud with rain
    gfx->fillRect(icon_x - 20, icon_y, 40, 12, TFT_LIGHTGREY);
    gfx->fillRect(icon_x - 10, icon_y - 8, 24, 8, TFT_LIGHTGREY);
    // Raindrops
    gfx->fillRect(icon_x - 12, icon_y + 15, 4, 8, TFT_BLUE);
    gfx->fillRect(icon_x, icon_y + 15, 4, 8, TFT_BLUE);
    gfx->fillRect(icon_x + 12, icon_y + 15, 4, 8, TFT_BLUE);
  }

  // Draw temperature with manual degree symbol
  if (weatherDataValid && WiFi.status() == WL_CONNECTED) {
    gfx->setTextColor(TFT_WHITE);
  } else {
    gfx->setTextColor(TFT_DARKGREY); // Grey out when data is not valid
  }
  gfx->setTextSize(2);
  
  char tempStr[4];
  sprintf(tempStr, "%d", temperature);
  
  int tempWidth = gfx->textWidth(tempStr);
  gfx->drawString(tempStr, 120 - (tempWidth/2) - 5, icon_y + 35);
  gfx->drawCircle(120 + (tempWidth/2), icon_y + 28, 2, (weatherDataValid && WiFi.status() == WL_CONNECTED) ? TFT_WHITE : TFT_DARKGREY); // Moved degree symbol up
  gfx->drawString("F", 120 + (tempWidth/2) + 10, icon_y + 35);
  
  // Draw Location
  gfx->setTextSize(1);
  gfx->setTextColor(TFT_SILVER);
  gfx->drawString("Union, KY", 120, icon_y + 55);

  // Show connection status indicator
  if (WiFi.status() != WL_CONNECTED) {
    gfx->setTextColor(TFT_RED);
    gfx->setTextSize(1);
    gfx->drawString("No WiFi", 120, icon_y + 70);
  } else if (!weatherDataValid) {
    gfx->setTextColor(TFT_YELLOW);
    gfx->setTextSize(1);
    gfx->drawString("Weather Err", 120, icon_y + 70);
  }
} 