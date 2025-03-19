#include "HoldRun.h"
#include "Config.h"
#include "DisplayManager.h"
#include "GameConfig.h"
#include "Helpers.h"

HoldRun::HoldRun(unsigned long r_limit)
    : limit(r_limit), blu_time(static_cast<long>(limit) / 2),
      red_time(static_cast<long>(limit) / 2) {
  DisplayManager dm = DisplayManager::get();
  dm.dispClear(DisplayManager::Timers::Center);
  displayColor(CRGB::White);
  playHorn(RING_START);
}

HoldRun::~HoldRun() {
  if (blu_time < 0 && red_time < 0 || blu_time == red_time) {
    displayColor(CRGB::White);
  } else if (blu_time < 0) {
    displayColor(CRGB::Blue);
  } else {
    displayColor(CRGB::Red);
  }
  playHorn(RING_END);
}

BoxState *HoldRun::tick() {
  DisplayManager dm = DisplayManager::get();
  const bool blue_override = digitalRead(REM_1) == HIGH;
  const bool red_override = digitalRead(REM_2) == HIGH;
  const bool neutral_override = digitalRead(REM_3) == HIGH;
  const bool blue = digitalRead(B_PIN) == HIGH;
  const bool red = digitalRead(R_PIN) == HIGH;
  const unsigned long time = millis();
  const long t_diff = static_cast<long>(time - lst_loop);
  if (blue_override || red_override || neutral_override) {
    const unsigned long negation = time - swap_time;
    switch (holders) {
    case ACTIVE_TEAM::Blue:
      blu_time += static_cast<long>(negation);
      break;
    case ACTIVE_TEAM::Red:
      red_time += static_cast<long>(negation);
      break;
    default:
      // neutral team doesn't track time
      // could try to add back to remaining game time
      // effectively reseting the entire game state
    }
  }
  if ((blue && red) || neutral_override) {
    holders = ACTIVE_TEAM::Neutral;
  } else if (blue || blue_override) {
    holders = ACTIVE_TEAM::Blue;
  } else if (red || red_override) {
    holders = ACTIVE_TEAM::Red;
  }
  switch (holders) {
  case ACTIVE_TEAM::Blue:
    blu_time -= t_diff;
    displayColor(CRGB::Blue);
    break;
  case ACTIVE_TEAM::Red:
    red_time -= t_diff;
    displayColor(CRGB::Red);
    break;
  default:
    displayColor(CRGB::White);
  }
  if (holders != last_holders) {
    swap_time = time;
  }
  dm.dispMillis(DisplayManager::Timers::Blue, blu_time);
  dm.dispMillis(DisplayManager::Timers::Red, red_time);
  if (blu_time < 0 || red_time < 0) {
    return new GameConfig();
  }
  lst_loop = time;
  return this;
}
