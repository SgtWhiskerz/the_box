#include "GameGrace.h"
#include "Config.h"
#include "DisplayManager.h"
#include "DominateRun.h"
#include "HoldRun.h"
#include "LastManRun.h"

GameGrace::GameGrace(GameGrace::Games sel_game, unsigned long r_limit)
    : act_game(sel_game), limit(r_limit) {}

BoxState *GameGrace::tick() {
  const unsigned long time = millis();
  const unsigned long elapsed = time - getChangePoint();
  DisplayManager dispMan = DisplayManager::get();
  if (!cnt_dwn) {
    dispMan.dispMillis(DisplayManager::Timers::Center, limit);
    if (time - getChangePoint() > LIMIT_SHOWN) {
      cnt_dwn = true;
    }
  } else {
    dispMan.dispMillis(DisplayManager::Timers::Center, GRACE_PERIOD - elapsed);
  }

  if (time - getChangePoint() > GRACE_PERIOD) {
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
