#include "LastManRun.h"
#include "Arduino.h"
#include "Config.h"
#include "DisplayManager.h"
#include "GameConfig.h"
#include "Helpers.h"

const CRGB TEAM_COLORS[] = {CRGB::White, CRGB::Blue, CRGB::Red};

inline void teamButtons(ACTIVE_TEAM &team) {
  const bool blue = digitalRead(B_PIN) == HIGH;
  const bool red = digitalRead(R_PIN) == HIGH;
  if (blue && red) {
    team = ACTIVE_TEAM::Neutral;
  } else if (blue) {
    team = ACTIVE_TEAM::Blue;
  } else if (red) {
    team = ACTIVE_TEAM::Red;
  }
}

LastManRun::LastManRun(unsigned long r_limit) : limit(r_limit) {
  Serial1.write("awo0001s");
  Serial1.flush();
  delay(RING_START);
  Serial1.write("awo0000s");
  Serial1.flush();
}

LastManRun::~LastManRun() {
  Serial1.write("awo0001s");
  Serial1.flush();
  delay(RING_END);
  Serial1.write("awo0000s");
  Serial1.flush();
}

BoxState *LastManRun::tick() {
  const unsigned long time = millis();
  const unsigned long elapsed = time - getChangePoint();
  const long remain = static_cast<long>(limit - elapsed);
  DisplayManager dispMan = DisplayManager::get();

  if (remain < 0) { // TODO: test this for game satisfaction
                    // should the game end at 00:00:00 or 00:00:??
                    // ! careful, signed compared with unsigned
    return new GameConfig();
  } else {
    teamButtons(winner);
    const int indx = static_cast<int>(winner);
    displayColor(TEAM_COLORS[indx]);
    dispMan.dispMillis(DisplayManager::Timers::Center, remain);
  }
  return this;
}
