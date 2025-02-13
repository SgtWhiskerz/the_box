#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include "BoxState.h"
#include "GameGrace.h"

class GameConfig : public BoxState {
public:
  GameConfig() = default;
  GameConfig(const GameConfig &) = default;
  GameConfig(GameConfig &&) = default;
  GameConfig &operator=(const GameConfig &) = default;
  GameConfig &operator=(GameConfig &&) = default;
  ~GameConfig() override = default;
  BoxState *tick() final;

private:
  GameGrace::Games act_game = GameGrace::Games::None;
  bool game_set = false;
  long t_select = 0;
};

#endif
