#include <Arduino.h>
#include "display.h"
#include "time.h"
#include "battery.h"

void setup() {
  initDisplay();
  bootAnimation();
  initTime();
}

void loop() {
  updateTime();
  int battery = readBatteryPercent();
  drawHomeScreen(getHour(), getMinute(), getDate(), battery);
  delay(1000);
}
