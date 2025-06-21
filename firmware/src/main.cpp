#include <TFT_eSPI.h>
#include <WiFi.h>
#include <time.h>
#include "battery.h"
#include "weather.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft); // Create a sprite object

// Button pin definition
#define BUTTON_PIN 3

// Screen states
enum ScreenState { SCREEN_MAIN, SCREEN_SECOND };
ScreenState currentScreen = SCREEN_MAIN;

// WiFi credentials - you'll need to update these
const char* ssid = "Wildcat";
const char* password = "purdue0810121416";

// NTP Server settings
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -18000; // GMT offset in seconds (adjust for your timezone)
const int daylightOffset_sec = 3600; // Daylight saving time offset

// Time variables
struct tm timeinfo;
bool timeInitialized = false;

// Weather update timer
unsigned long lastWeatherUpdate = 0;
const long weatherUpdateInterval = 900000; // 15 minutes

// Helper to get short day string
const char* getDayString(uint8_t dayOfWeek) {
  switch(dayOfWeek) {
    case 0: return "Sun";
    case 1: return "Mon";
    case 2: return "Tue";
    case 3: return "Wed";
    case 4: return "Thu";
    case 5: return "Fri";
    case 6: return "Sat";
  }
  return "";
}

// Helper to get short month string
const char* getMonthString(uint8_t month) {
  switch(month) {
    case 1: return "Jan";
    case 2: return "Feb";
    case 3: return "Mar";
    case 4: return "Apr";
    case 5: return "May";
    case 6: return "Jun";
    case 7: return "Jul";
    case 8: return "Aug";
    case 9: return "Sep";
    case 10: return "Oct";
    case 11: return "Nov";
    case 12: return "Dec";
  }
  return "";
}

void setup() {
  Serial.begin(115200);
  
  // Initialize button pin
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  tft.init();
  tft.setRotation(0);
  
  // Create the sprite
  spr.createSprite(240, 240);
  spr.setTextDatum(MC_DATUM); // Set text alignment to Middle-Center

  // Show connecting message
  spr.setTextColor(TFT_YELLOW);
  spr.setTextSize(2);
  spr.setCursor(20, 100);
  spr.println("Connecting to WiFi...");
  spr.pushSprite(0, 0); // Push sprite to screen
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    spr.print(".");
    spr.pushSprite(0, 0); // Push sprite to screen
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    spr.fillSprite(TFT_BLACK);
    spr.setCursor(20, 100);
    spr.setTextColor(TFT_GREEN);
    spr.println("WiFi Connected!");
    spr.println("Getting time...");
    spr.pushSprite(0, 0);
    
    // Configure NTP
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
    // Wait for time to be set
    int timeAttempts = 0;
    while (!getLocalTime(&timeinfo) && timeAttempts < 10) {
      delay(1000);
      timeAttempts++;
    }
    
    if (getLocalTime(&timeinfo)) {
      timeInitialized = true;
      updateWeatherData(); // Get initial weather data
      spr.fillSprite(TFT_BLACK);
      spr.setCursor(20, 100);
      spr.setTextColor(TFT_GREEN);
      spr.println("Time synced!");
      spr.pushSprite(0, 0);
      delay(1000);
    } else {
      spr.fillSprite(TFT_BLACK);
      spr.setCursor(20, 100);
      spr.setTextColor(TFT_RED);
      spr.println("Failed to get time");
      spr.pushSprite(0, 0);
      delay(2000);
    }
  } else {
    spr.fillSprite(TFT_BLACK);
    spr.setCursor(20, 100);
    spr.setTextColor(TFT_RED);
    spr.println("WiFi failed!");
    spr.pushSprite(0, 0);
    delay(2000);
  }
  
  spr.fillSprite(TFT_BLACK);
  spr.pushSprite(0, 0);
}

void loop() {
  // Handle button press
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && buttonState == LOW) {
    // Button was just pressed
    Serial.println("Button pressed - switching screens");
    if (currentScreen == SCREEN_MAIN) {
      currentScreen = SCREEN_SECOND;
    } else {
      currentScreen = SCREEN_MAIN;
    }
    delay(200); // Simple debounce
  }
  lastButtonState = buttonState;

  spr.fillSprite(TFT_BLACK); // Clear the sprite

  // Update weather data periodically
  if (millis() - lastWeatherUpdate > weatherUpdateInterval) {
    lastWeatherUpdate = millis();
    Serial.println("Updating weather data...");
    updateWeatherData();
  }

  // Draw content based on current screen
  if (currentScreen == SCREEN_MAIN) {
    // Draw main watch face
    drawWeather(&spr);

    if (timeInitialized) {
      // Get current time
      if (getLocalTime(&timeinfo)) {
        // Draw Time
        spr.setTextColor(TFT_WHITE);
        spr.setTextSize(7);
        char timeStr[6];
        sprintf(timeStr, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
        spr.drawString(timeStr, 120, 125); // Adjusted for new layout

        // Draw Date
        spr.setTextColor(TFT_SILVER);
        spr.setTextSize(2);
        char dateStr[16];
        sprintf(dateStr, "%s, %s %d", 
                getDayString(timeinfo.tm_wday), 
                getMonthString(timeinfo.tm_mon + 1), 
                timeinfo.tm_mday);
        spr.drawString(dateStr, 120, 175); // Adjusted for new layout
        
        // Draw Battery
        int batt = getBatteryPercentage();
        drawBattery(&spr, batt);
      }
    } else {
      // Fallback to simple timer if WiFi/time sync failed
      static unsigned long startTime = millis();
      unsigned long elapsedSeconds = (millis() - startTime) / 1000;
      
      int currentHour = (elapsedSeconds / 3600) % 24;
      int currentMinute = (elapsedSeconds / 60) % 60;
      
      // Draw Time
      spr.setTextColor(TFT_WHITE);
      spr.setTextSize(7);
      char timeStr[6];
      sprintf(timeStr, "%02d:%02d", currentHour, currentMinute);
      spr.drawString(timeStr, 120, 125);

      // Draw Date (static)
      spr.setTextColor(TFT_SILVER);
      spr.setTextSize(2);
      spr.drawString("Mon, Jan 1", 120, 175);
      
      // Draw Battery
      int batt = getBatteryPercentage();
      drawBattery(&spr, batt);
    }
  } else if (currentScreen == SCREEN_SECOND) {
    // Draw second screen
    spr.setTextColor(TFT_CYAN);
    spr.setTextSize(3);
    spr.drawString("Settings", 120, 80);
    
    spr.setTextColor(TFT_WHITE);
    spr.setTextSize(2);
    spr.drawString("Press button to", 120, 120);
    spr.drawString("return to main", 120, 145);
    
    // Show some system info
    spr.setTextColor(TFT_YELLOW);
    spr.setTextSize(1);
    char infoStr[32];
    sprintf(infoStr, "WiFi: %s", WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
    spr.drawString(infoStr, 120, 180);
    
    sprintf(infoStr, "Free RAM: %d KB", ESP.getFreeHeap() / 1024);
    spr.drawString(infoStr, 120, 195);
    
    sprintf(infoStr, "Screen: %d", currentScreen);
    spr.drawString(infoStr, 120, 210);
  }

  spr.pushSprite(0, 0); // Push the completed sprite to the screen
  delay(100); // Faster refresh rate for better button responsiveness
} 