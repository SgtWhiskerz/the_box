#include "DominateRun.h"
#include "Config.h"
#include "DisplayManager.h"
#include "GameConfig.h"
#include "Helpers.h"

DominateRun::DominateRun(unsigned long r_limit) : limit(r_limit) {
  displayColor(CRGB::White);
  playHorn(RING_START);
}

DominateRun::~DominateRun() {
  DisplayManager dm = DisplayManager::get();
  if (blu_time > red_time) {
    displayColor(CRGB::Blue);
  } else if (red_time > blu_time) {
    displayColor(CRGB::Red);
  } else {
    displayColor(CRGB::White);
  }
  playHorn(RING_END);
}

BoxState *DominateRun::tick() {
  DisplayManager dm = DisplayManager::get();
  const bool blue_override = digitalRead(REM_1) == HIGH;
  const bool red_override = digitalRead(REM_2) == HIGH;
  const bool neutral_override = digitalRead(REM_3) == HIGH;
  static bool lst_boverride = blue_override;
  static bool lst_roverride = red_override;
  static bool lst_noverride = neutral_override;
  const bool blue = digitalRead(B_PIN) == HIGH;
  const bool red = digitalRead(R_PIN) == HIGH;
  const unsigned long time = millis();
  const unsigned long elapsed = time - getChangePoint();
  const unsigned long t_diff = time - lst_loop;
  if ((blue_override && blue_override != lst_boverride) ||
      (red_override && red_override != lst_roverride) ||
      (neutral_override && neutral_override != lst_noverride)) {
    const unsigned long negation = time - swap_time;
    switch (active_team) {
    case ACTIVE_TEAM::Blue:
      blu_time -= negation;
      break;
    case ACTIVE_TEAM::Red:
      red_time -= negation;
      break;
    default:
      // neutral team doesn't track time
      // could try to add back to remaining game time
      // effectively reseting the entire game state
      break;
    }
  }
  if ((blue && red) || neutral_override) {
    last_team = active_team;
    active_team = ACTIVE_TEAM::Neutral;
  } else if (blue || blue_override) {
    last_team = active_team;
    active_team = ACTIVE_TEAM::Blue;
  } else if (red || red_override) {
    last_team = active_team;
    active_team = ACTIVE_TEAM::Red;
  }
  if (active_team != last_team) {
    swap_time = time;
  }
  switch (active_team) {
  case ACTIVE_TEAM::Blue:
    blu_time += t_diff;
    displayColor(CRGB::Blue);
    break;
  case ACTIVE_TEAM::Red:
    red_time += t_diff;
    displayColor(CRGB::Red);
    break;
  default:
    displayColor(CRGB::White);
  }
  dm.dispMillis(DisplayManager::Timers::Blue, blu_time);
  dm.dispMillis(DisplayManager::Timers::Red, red_time);
  dm.dispMillis(DisplayManager::Timers::Center, limit - elapsed);
  if (elapsed > limit) {
    return new GameConfig();
  }
  lst_loop = time;
  lst_boverride = blue_override;
  lst_roverride = red_override;
  lst_noverride = neutral_override;
  last_team = active_team;
  return this;
}
