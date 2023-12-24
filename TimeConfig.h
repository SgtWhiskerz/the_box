#pragma once

#include "BoxState.h"
#include "GameGrace.h"

class TimeConfig : public BoxState {
public:
  TimeConfig(GameGrace::Games);
  virtual ~TimeConfig() = default;
  BoxState *tick() final;

private:
  GameGrace::Games act_game;
  bool on = true;
  bool time_set = false;
  long time_limit = 0;
  unsigned long last_blink = 0;
};
