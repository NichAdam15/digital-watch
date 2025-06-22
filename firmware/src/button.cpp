#include "button.h"
#include <Arduino.h>

#define BUTTON_PIN 6

void button_init() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  delay(100); // Let pull-up stabilize
}

bool button_handle_press(ScreenState &currentScreen) {
  static bool lastButtonState = HIGH;
  static bool lastStableState = HIGH;
  static unsigned long lastDebounceTime = 0;
  static bool firstRun = true;
  const unsigned long debounceDelay = 15;
  
  // On first run, read the button state to initialize properly
  if (firstRun) {
    lastButtonState = digitalRead(BUTTON_PIN);
    lastStableState = lastButtonState;
    firstRun = false;
    return false; // Don't process on first run
  }
  
  bool reading = digitalRead(BUTTON_PIN);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (lastStableState == HIGH && reading == LOW) {
      // Button was just pressed
      lastStableState = LOW;
      lastButtonState = reading;
      // Toggle screen
      if (currentScreen == SCREEN_MAIN) {
        currentScreen = SCREEN_WEATHER;
      } else {
        currentScreen = SCREEN_MAIN;
      }
      return true;
    } else if (reading == HIGH) {
      lastStableState = HIGH;
    }
  }
  
  lastButtonState = reading;
  return false;
} 