#pragma once

#include "BoxState.h"
#include "Config.h"

class LastManRun : public BoxState {
public:
  LastManRun(unsigned long);
  virtual ~LastManRun() = default;

  BoxState *tick() final;

private:
  ACTIVE_TEAM winner = ACTIVE_TEAM::Neutral;
  unsigned long limit;
};
