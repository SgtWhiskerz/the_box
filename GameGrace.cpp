#pragma once

#include "GameGrace.h"
#include "Config.h"
#include "DisplayManager.h"
#include "dominate/DominateRun.h"
#include "hold/HoldRun.h"
#include "last_man/LastManRun.h"

GameGrace::GameGrace(GameGrace::Games sel_game, unsigned long r_limit)
    : act_game(sel_game), limit(r_limit) {}

BoxState *GameGrace::tick() {
  unsigned long time = millis();
  unsigned long elapsed = time - change;
  DisplayManager dm = DisplayManager::get();
  if (!cnt_dwn) {
    dm.dispMillis(DisplayManager::Timers::Center, limit);
    if (time - change > LIMIT_SHOWN) {
      cnt_dwn = true;
    } else {
      dm.dispMillis(DisplayManager::Timers::Center, GRACE_PERIOD - elapsed);
    }
  }

  if (time - change > GRACE_PERIOD) {
    Serial.println("[INFO] Transitioning from GRACE to RUNNING");
    switch (act_game) {
    case GameGrace::Games::LastMan:

      return new LastManRun(limit);
    case GameGrace::Games::Hold:
      return new HoldRun(limit);
    case GameGrace::Games::Dominate:
      return new DominateRun(limit);
    }
  }
  return this;
}
