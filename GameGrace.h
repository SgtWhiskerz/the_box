#ifndef GAME_GRACE_H
#define GAME_GRACE_H

#include "BoxState.h"

class GameGrace : public BoxState {
public:
  enum class Games { LastMan, Dominate, Hold, None };

  GameGrace(Games, unsigned long);
  GameGrace(GameGrace &) = default;
  GameGrace(GameGrace &&) = default;
  GameGrace &operator=(const GameGrace &) = default;
  GameGrace &operator=(GameGrace &&) = default;
  ~GameGrace() override = default;

  BoxState *tick() final;

private:
  Games act_game;
  bool cnt_dwn = false;
  unsigned long limit;
};

#endif
