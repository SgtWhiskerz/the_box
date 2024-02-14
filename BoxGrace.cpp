#pragma once

#include "BoxGrace.h"
#include "BoxRun.h"
#include "Config.h"
#include "Helpers.h"

BoxGrace::BoxGrace(unsigned long r_limit) : limit(r_limit) {}

BoxState *BoxGrace::tick() {
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
    return new BoxRun(limit);
  }
  return this;
}
