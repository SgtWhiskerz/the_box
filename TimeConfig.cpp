#include "TimeConfig.h"
#include "Config.h"
#include "DisplayManager.h"
#include "Helpers.h"

constexpr unsigned long TGGLE_DUR = 1000;

TimeConfig::TimeConfig(GameGrace::Games game) : act_game(game) {}

BoxState *TimeConfig::tick() {
  static const uint8_t all_on[] = {0xff, 0xff, 0xff, 0xff};
  DisplayManager dm = DisplayManager::get();
  unsigned long time = millis();
  if (time - last_blink > TGGLE_DUR) {
    if (on) {
      dm.dispSegments(DisplayManager::Timers::Center, all_on);
    } else {
      dm.dispClear(DisplayManager::Timers::Center);
    }
    on = !on;
    last_blink = time;
  }
  if (digitalRead(REM_1) == HIGH) {
    time_limit = 5_min;
    time_set = true;
  }
  if (digitalRead(REM_2) == HIGH) {
    time_limit = 10_min;
    time_set = true;
  }
  if (digitalRead(REM_3) == HIGH) {
    time_limit = 15_min;
    time_set = true;
  }
  if (time_set) {
    Serial.println("[INFO] Transitioning from CONFIG to GRACE");
    Serial.print("[INFO]\tSelected time limit: ");
    Serial.println(time_limit);
    return new GameGrace(act_game, time_limit);
  }
  return this;
}
