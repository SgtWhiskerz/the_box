#pragma once

#include "BoxState.h"
#include "GameGrace.h"

class GameConfig : public BoxState {
public:
  virtual ~GameConfig() = default;
  BoxState *tick() final;

private:
  GameGrace::Games act_game = GameGrace::Games::None;
  bool game_set = false;
  long t_select = 0;
};
