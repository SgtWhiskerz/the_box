#include "DominateRun.h"
#include "DisplayManager.h"
#include "GameConfig.h"

DominateRun::DominateRun(unsigned long r_limit) : limit(r_limit) {}

BoxState *DominateRun::tick() {
  /*
   * Handle team buttons
   * Increment team held time
   */
  DisplayManager dm = DisplayManager::get();
  const bool blue = digitalRead(B_PIN) == HIGH;
  const bool red = digitalRead(R_PIN) == HIGH;
  const long t_diff = static_cast<long>(millis() - getChangePoint());

  if (blue && red) {
    active_team = ACTIVE_TEAM::Neutral;
  } else if (blue) {
    active_team = ACTIVE_TEAM::Blue;
  } else if (red) {
    active_team = ACTIVE_TEAM::Red;
  }
  // TODO: disp team color
  if (active_team == ACTIVE_TEAM::Blue) {
    blu_time = t_diff;
  } else if (active_team == ACTIVE_TEAM::Red) {
    red_time = t_diff;
  }

  dm.dispMillis(DisplayManager::Timers::Blue, blu_time);
  dm.dispMillis(DisplayManager::Timers::Red, red_time);

  if (blu_time > limit || red_time > limit) {
    return new GameConfig();
  }
  return this;
}
