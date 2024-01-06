#ifndef HOLD_RUN_H
#define HOLD_RUN_H

#include "BoxState.h"
#include "Config.h"

/**
 * Teams compete to deplete their timer the quickest
 */
class HoldRun : public BoxState {
public:
  HoldRun(unsigned long);
  HoldRun(HoldRun &) = default;
  HoldRun(HoldRun &&) = default;
  HoldRun &operator=(const HoldRun &) = default;
  HoldRun &operator=(HoldRun &&) = default;
  ~HoldRun() override = default;

  BoxState *tick() final;

private:
  ACTIVE_TEAM holders = ACTIVE_TEAM::Neutral;
  unsigned long limit;
  long blu_time = 0;
  long red_time = 0;
};

#endif
