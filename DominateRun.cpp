#include "DominateRun.h"
#include "Config.h"
#include "DisplayManager.h"
#include "GameConfig.h"
#include "Helpers.h"

DominateRun::DominateRun(unsigned long r_limit) : limit(r_limit) {
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
  /*
   * Handle team buttons
   * Increment team held time
   */
  DisplayManager dm = DisplayManager::get();
  const bool blue = digitalRead(B_PIN) == HIGH;
  const bool red = digitalRead(R_PIN) == HIGH;
  const unsigned long time = millis();
  const unsigned long elapsed = time - getChangePoint();
  const unsigned long t_diff = time - lst_loop;

  if (blue && red) {
    active_team = ACTIVE_TEAM::Neutral;
  } else if (blue) {
    active_team = ACTIVE_TEAM::Blue;
  } else if (red) {
    active_team = ACTIVE_TEAM::Red;
  }

  if (active_team == ACTIVE_TEAM::Blue) {
    blu_time += t_diff;
    displayColor(CRGB::Blue);
  } else if (active_team == ACTIVE_TEAM::Red) {
    red_time += t_diff;
    displayColor(CRGB::Red);
  } else {
    displayColor(CRGB::White);
  }

  dm.dispMillis(DisplayManager::Timers::Blue, blu_time);
  dm.dispMillis(DisplayManager::Timers::Red, red_time);
  dm.dispMillis(DisplayManager::Timers::Center, limit - elapsed);

  if (elapsed > limit) {
    return new GameConfig();
  }
  lst_loop = time;
  return this;
}
