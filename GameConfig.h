#pragma once

#include "BoxState.h"

class GameConfig : public BoxState {
public:
  virtual ~GameConfig() = default;
  BoxState *tick() final;

private:
  bool on = true;
  bool time_set = false;
  long time_limit = 0;
  unsigned long last_blink = 0;
};
