#include "GameConfig.h"
#include "Config.h"
#include "DisplayManager.h"
#include "Helpers.h"
#include "Arduino.h"
#include "GameGrace.h"

BoxState *GameConfig::tick() {
  static const uint8_t all_on[] = {0xff, 0xff, 0xff, 0xff};
  DisplayManager dm = DisplayManager::get();
  long time = millis();
  if (time - last_blink > 1000) {
    if (on) {
      dm.dispSegments(DisplayManager::Timers::Center, all_on);
    } else {
      dm.dispClear(DisplayManager::Timers::Center);
    }
    on = !on;
    last_blink = time;
  }
  if (digitalRead(MIN_5) == HIGH) {
    time_limit = 5_min;
    time_set = true;
  }
  if (digitalRead(MIN_10) == HIGH) {
    time_limit = 10_min;
    time_set = true;
  }
  if (digitalRead(MIN_15) == HIGH) {
    time_limit = 15_min;
    time_set = true;
  }
  if (time_set) {
    Serial.println("[INFO] Transitioning from CONFIG to GRACE");
    Serial.print("[INFO]\tSelected time limit: ");
    Serial.println(time_limit);
    return new GameGrace(time_limit);
  }
  return this;
}
