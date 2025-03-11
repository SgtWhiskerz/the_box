#include "GameConfig.h"
#include "Arduino.h"
#include "Config.h"
#include "DisplayManager.h"
#include "Helpers.h"
#include "TimeConfig.h"

BoxState *GameConfig::tick() {
  // Word 'none'
  static const uint8_t ng_ptrn[] = {SEG_A | SEG_B | SEG_C | SEG_E | SEG_F,
                                    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E |
                                        SEG_F,
                                    SEG_A | SEG_B | SEG_C | SEG_E | SEG_F,
                                    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G};
  // Word "LASN"
  static const uint8_t lm_ptrn[] = {
      SEG_D | SEG_E | SEG_F, SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,
      SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,
      SEG_A | SEG_B | SEG_C | SEG_E | SEG_F};
  // Word "doM"
  static const uint8_t dm_ptrn[] = {
      SEG_B | SEG_C | SEG_D | SEG_E | SEG_G, SEG_C | SEG_D | SEG_E | SEG_G,
      SEG_A | SEG_B | SEG_E | SEG_F, SEG_A | SEG_B | SEG_C};
  // Word "HoLd"
  static const uint8_t hd_ptrn[] = {
      SEG_B | SEG_C | SEG_E | SEG_F | SEG_G, SEG_C | SEG_D | SEG_E | SEG_G,
      SEG_D | SEG_E | SEG_F, SEG_B | SEG_C | SEG_D | SEG_E | SEG_G};
  // Word "bUtt"
  static const uint8_t ctrl_ptrn[] = {SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,
                                      SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
                                      SEG_D | SEG_E | SEG_F | SEG_G,
                                      SEG_D | SEG_E | SEG_F | SEG_G};

  DisplayManager dm = DisplayManager::get();
  const bool last_man = digitalRead(REM_1) == HIGH;
  const bool dominate = digitalRead(REM_2) == HIGH;
  const bool control = digitalRead(REM_3) == HIGH;
  const unsigned long time = millis();

  if (last_man) {
    act_game = GameGrace::Games::LastMan;
    game_set = true;
    t_select = static_cast<long>(time);
  } else if (dominate) {
    act_game = GameGrace::Games::Dominate;
    game_set = true;
    t_select = static_cast<long>(time);
  } else if (control) {
    act_game = GameGrace::Games::Control;
    game_set = true;
    t_select = static_cast<long>(time);
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
  case GameGrace::Games::Control:
    dm.dispSegments(DisplayManager::Timers::Center, ctrl_ptrn);
    break;
  case GameGrace::Games::None:
    dm.dispSegments(DisplayManager::Timers::Center, ng_ptrn);
    break;
  }

  if (game_set && time - t_select > 1_sec) {
    return new TimeConfig(act_game);
  }
  return this;
}
