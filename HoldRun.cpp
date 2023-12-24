#include "HoldRun.h"
#include "DisplayManager.h"
#include "GameConfig.h"
#include "Helpers.h"

HoldRun::HoldRun(unsigned long r_limit)
    : limit(r_limit), blu_time(static_cast<long>(limit) / 2),
      red_time(static_cast<long>(limit) / 2) {
  DisplayManager::get().dispClear(DisplayManager::Timers::Center);
}

HoldRun::~HoldRun() {
  if (blu_time < 0 && red_time < 0) {
    displayColor(CRGB::White);
  } else if (blu_time < 0) {
    displayColor(CRGB::Blue);
  } else {
    displayColor(CRGB::Red);
  }
}

BoxState *HoldRun::tick() {
  /*
   * Handle button toggle
   * set stored value to the start time - current
   * Check win condition (one team exceeds the limit)
   */
  DisplayManager dm = DisplayManager::get();
  const bool blue = digitalRead(B_PIN) == HIGH;
  const bool red = digitalRead(R_PIN) == HIGH;
  const unsigned long time = millis();
  const unsigned long t_diff = time - lst_loop;

  if (blue && red) {
    holders = ACTIVE_TEAM::Neutral;
  } else if (blue) {
    holders = ACTIVE_TEAM::Blue;
  } else if (red) {
    holders = ACTIVE_TEAM::Red;
  }

  if (holders == ACTIVE_TEAM::Blue) {
    blu_time -= t_diff;
    displayColor(CRGB::Blue);
  } else if (holders == ACTIVE_TEAM::Red) {
    red_time -= t_diff;
    displayColor(CRGB::Red);
  } else {
    displayColor(CRGB::White);
  }

  dm.dispMillis(DisplayManager::Timers::Blue, blu_time);
  dm.dispMillis(DisplayManager::Timers::Red, red_time);

  if (blu_time < 0 || red_time < 0) {
    return new GameConfig();
  }
  lst_loop = time;
  return this;
}
