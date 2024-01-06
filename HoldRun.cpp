#include "HoldRun.h"
#include "DisplayManager.h"
#include "GameConfig.h"

HoldRun::HoldRun(unsigned long r_limit) : limit(r_limit) {}

BoxState *HoldRun::tick() {
  /*
   * Handle button toggle
   * set stored value to the start time - current
   * Check win condition (one team exceeds the limit)
   */
  DisplayManager dm = DisplayManager::get();
  const bool blue = digitalRead(B_PIN) == HIGH;
  const bool red = digitalRead(R_PIN) == HIGH;
  const long t_diff = static_cast<long>(millis() - getChangePoint());

  if (blue && red) {
    holders = ACTIVE_TEAM::Neutral;
  } else if (blue) {
    holders = ACTIVE_TEAM::Blue;
  } else if (red) {
    holders = ACTIVE_TEAM::Red;
  }
  // TODO: disp team color
  if (holders == ACTIVE_TEAM::Blue) {
    blu_time = limit - t_diff;
  } else if (holders == ACTIVE_TEAM::Red) {
    red_time = limit - t_diff;
  }

  dm.dispMillis(DisplayManager::Timers::Blue, blu_time);
  dm.dispMillis(DisplayManager::Timers::Red, red_time);

  if (blu_time < 0 || red_time < 0) {
    return new GameConfig();
  }
  return this;
}
