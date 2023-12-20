#pragma once

#include "BoxState.h"
#include "Config.h"

class BoxRun : public BoxState {
public:
  BoxRun(unsigned long);
  virtual ~BoxRun() = default;

  BoxState *tick() final;

private:
  ACTIVE_TEAM winner = ACTIVE_TEAM::Neutral;
  unsigned long limit;
};
