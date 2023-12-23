#pragma once

#include "LastManGrace.h"
#include "LastManRun.h"
#include "Config.h"
#include "Helpers.h"

LastManGrace::LastManGrace(unsigned long r_limit) : limit(r_limit) {}

BoxState *LastManGrace::tick() {
  unsigned long time = millis();
  unsigned long elapsed = time - change;
  if (!cnt_dwn) {
    displayMillis(limit);
    if (time - change > LIMIT_SHOWN) {
      cnt_dwn = true;
    }
  } else {
    displayMillis(GRACE_PERIOD - elapsed);
  }

  if (time - change > GRACE_PERIOD) {
    Serial.println("[INFO] Transitioning from GRACE to RUNNING");
    return new LastManRun(limit);
  }
  return this;
}
