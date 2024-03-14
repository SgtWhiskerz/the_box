#include "LastManRun.h"
#include "Arduino.h"
#include "Config.h"
#include "DisplayManager.h"
#include "GameConfig.h"
#include "Helpers.h"

LastManRun::LastManRun(unsigned long r_limit) : limit(r_limit) {
  DisplayManager dm = DisplayManager::get();
  dm.dispClear(DisplayManager::Timers::Blue);
  dm.dispClear(DisplayManager::Timers::Red);
  displayColor(CRGB::White);
  playHorn(RING_START);
}

LastManRun::~LastManRun() { playHorn(RING_END); }

BoxState *LastManRun::tick() {
  const bool blue = digitalRead(B_PIN) == HIGH;
  const bool red = digitalRead(R_PIN) == HIGH;
  const unsigned long time = millis();
  const unsigned long elapsed = time - getChangePoint();
  const long remain = static_cast<long>(limit - elapsed);
  DisplayManager dispMan = DisplayManager::get();

  if (blue && red) {
    active_team = ACTIVE_TEAM::Neutral;
  } else if (blue) {
    active_team = ACTIVE_TEAM::Blue;
  } else {
    active_team = ACTIVE_TEAM::Red;
  }

  switch (active_team) {
  case ACTIVE_TEAM::Blue:
    displayColor(CRGB::Blue);
    break;
  case ACTIVE_TEAM::Red:
    displayColor(CRGB::Red);
    break;
  default:
    displayColor(CRGB::White);
  }

  dispMan.dispMillis(DisplayManager::Timers::Center, remain);

  if (remain < 0) { // TODO: test this for game satisfaction
                    // should the game end at 00:00:00 or 00:00:??
                    // ! careful, signed compared with unsigned
    return new GameConfig();
  }
  return this;
}
