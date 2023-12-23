#pragma once

#include "BoxState.h"

class GameGrace : public BoxState {
public:
  enum class Games { LastMan, Dominate, Hold };

  GameGrace(Games, unsigned long);
  virtual ~GameGrace() = default;

  BoxState *tick() final;

private:
  Games act_game;
  bool cnt_dwn = false;
  unsigned long limit;
};
