#include "weather.h"
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

// --- Live Weather Data ---
const char* city = "Union,KY,US";
const char* api_key = "27eadd3cdeda61121d8e1c3ff798031d";
// -------------------------

static int temperature = 75; // Default fallback temperature
static String weatherCondition = "Clear";
static bool weatherDataValid = false;

void updateWeatherData() {
  // Check if WiFi is connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, skipping weather update");
    weatherDataValid = false;
    return;
  }

  String url = String("http://api.openweathermap.org/data/2.5/weather?q=") + city + "&units=imperial&APPID=" + api_key;
  
  Serial.print("Fetching weather from: ");
  Serial.println(url);

  HTTPClient http;
  http.begin(url);
  http.setTimeout(10000); // 10 second timeout
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpCode);
    
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Successfully fetched weather data.");
      String payload = http.getString();
      Serial.print("Payload: ");
      Serial.println(payload);
      
      JSONVar myObject = JSON.parse(payload);

      if (JSON.typeof(myObject) != "undefined") {
        // Extract weather data
        if (myObject.hasOwnProperty("main") && myObject["main"].hasOwnProperty("temp")) {
          temperature = (int)round((double)myObject["main"]["temp"]);
          Serial.print("Temperature: ");
          Serial.println(temperature);
          weatherDataValid = true;
        }
        
        if (myObject.hasOwnProperty("weather") && myObject["weather"].hasOwnProperty(0)) {
          String mainCondition = (const char*)myObject["weather"][0]["main"];
          Serial.print("Weather condition: ");
          Serial.println(mainCondition);
          
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
        Serial.println("Failed to parse JSON.");
        weatherDataValid = false;
      }
    } else {
      Serial.print("HTTP Error: ");
      Serial.println(httpCode);
      weatherDataValid = false;
    }
  } else {
    Serial.print("HTTP GET request failed. Error: ");
    Serial.println(http.errorToString(httpCode).c_str());
    weatherDataValid = false;
  }
  
  http.end();
}

bool isWeatherDataValid() {
  return weatherDataValid;
}

void drawWeather(TFT_eSPI* gfx) {
  int icon_x = 120;
  int icon_y = 45;

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
  if (weatherDataValid) {
    gfx->setTextColor(TFT_WHITE);
  } else {
    gfx->setTextColor(TFT_DARKGREY); // Grey out when data is not valid
  }
  gfx->setTextSize(2);
  
  char tempStr[4];
  sprintf(tempStr, "%d", temperature);
  
  int tempWidth = gfx->textWidth(tempStr);
  gfx->drawString(tempStr, 120 - (tempWidth/2) - 5, icon_y + 35);
  gfx->drawCircle(120 + (tempWidth/2), icon_y + 28, 2, weatherDataValid ? TFT_WHITE : TFT_DARKGREY); // Moved degree symbol up
  gfx->drawString("F", 120 + (tempWidth/2) + 10, icon_y + 35);
  
  // Show connection status indicator
  if (!weatherDataValid) {
    gfx->setTextColor(TFT_RED);
    gfx->setTextSize(1);
    gfx->drawString("No WiFi", icon_x - 15, icon_y + 55);
  }
} 