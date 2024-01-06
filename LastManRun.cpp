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

LastManRun::LastManRun(unsigned long r_limit) : limit(r_limit) {}

BoxState *LastManRun::tick() {
  const unsigned long time = millis();
  const unsigned long elapsed = time - getChangePoint();
  const long remain = static_cast<long>(limit - elapsed);
  DisplayManager dispMan = DisplayManager::get();
  if (elapsed < RING_START) {
    digitalWrite(HEADACHE, HIGH);
  } else {
    digitalWrite(HEADACHE, LOW);
  }

  if (remain < 0) {               // TODO: test this for game satisfaction
                                  // should the game end at 00:00:00 or 00:00:??
    if (remain * -1 < RING_END) { // ! careful, signed compared with unsigned
      digitalWrite(HEADACHE, HIGH);
    } else {
      digitalWrite(HEADACHE, LOW);
    }

    if (digitalRead(READY) == HIGH) {
      Serial.println("[INFO] Transitioning from RUNNING to CONFIG");
      return new GameConfig();
    }
  } else {
    teamButtons(winner);
    const int indx = static_cast<int>(winner);
    displayColor(TEAM_COLORS[indx]);
    dispMan.dispMillis(DisplayManager::Timers::Center, remain);
  }
  return this;
}
