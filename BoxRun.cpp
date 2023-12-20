#pragma once

#include "BoxRun.h"
#include "Arduino.h"
#include "BoxConfig.h"
#include "Config.h"
#include "FastLED.h"
#include "Helpers.h"

CRGB TEAM_COLORS[] = {CRGB::White, CRGB::Blue, CRGB::Red};

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

BoxRun::BoxRun(unsigned long r_limit) : limit(r_limit) {}

BoxState *BoxRun::tick() {
  unsigned long time = millis();
  unsigned long elapsed = time - change;
  long remain = limit - elapsed;
  if (elapsed < RING_START) {
    digitalWrite(HEADACHE, HIGH);
  } else {
    digitalWrite(HEADACHE, LOW);
  }

  if (remain < 0) { // TODO: test this for game satisfaction
                    // should the game end at 00:00:00 or 00:00:??
    if (remain * -1 < RING_END) {
      digitalWrite(HEADACHE, HIGH);
    } else {
      digitalWrite(HEADACHE, LOW);
    }

    if (digitalRead(READY) == HIGH) {
      Serial.println("[INFO] Transitioning from RUNNING to CONFIG");
      return new BoxConfig();
    }
  } else {
    teamButtons(winner);
    const int indx = static_cast<int>(winner);
    displayColor(TEAM_COLORS[indx]);
    displayMillis(remain);
  }
  return this;
}
