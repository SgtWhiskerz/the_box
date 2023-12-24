#include "GameConfig.h"
#include "Arduino.h"
#include "Config.h"
#include "DisplayManager.h"
#include "TimeConfig.h"

BoxState *GameConfig::tick() {
  static const uint8_t ng_ptrn[] = {};
  static const uint8_t lm_ptrn[] = {};
  static const uint8_t dm_ptrn[] = {};
  static const uint8_t hd_ptrn[] = {};

  DisplayManager dm = DisplayManager::get();
  const bool last_man = digitalRead(MIN_5) == HIGH;
  const bool dominate = digitalRead(MIN_10) == HIGH;
  const bool hold = digitalRead(MIN_15) == HIGH;
  unsigned long time = millis();

  if (last_man) {
    act_game = GameGrace::Games::LastMan;
    game_set = true;
    t_select = time;
  } else if (dominate) {
    act_game = GameGrace::Games::Dominate;
    game_set = true;
    t_select = time;
  } else if (hold) {
    act_game = GameGrace::Games::Hold;
    game_set = true;
    t_select = time;
  }

  switch (act_game) {
  case GameGrace::Games::LastMan:
    dm.dispSegments(DisplayManager::Timers::Center, lm_ptrn);
    break;
  case GameGrace::Games::Dominate:
    dm.dispSegments(DisplayManager::Timers::Center, dm_ptrn);
    break;
  case GameGrace::Games::Hold:
    dm.dispSegments(DisplayManager::Timers::Center, hd_ptrn);
    break;
  default:
    dm.dispSegments(DisplayManager::Timers::Center, ng_ptrn);
    break;
  }

  if (game_set && time - t_select > LIMIT_SHOWN) {
    return new TimeConfig(act_game);
  }
  return this;
}
