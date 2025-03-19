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
  HoldRun(const HoldRun &) = default;
  HoldRun(HoldRun &&) = default;
  HoldRun &operator=(const HoldRun &) = default;
  HoldRun &operator=(HoldRun &&) = default;
  ~HoldRun() override;

  BoxState *tick() final;

private:
  ACTIVE_TEAM holders = ACTIVE_TEAM::Neutral;
  ACTIVE_TEAM last_holders = ACTIVE_TEAM::Neutral;
  unsigned long limit;
  unsigned long lst_loop = getChangePoint();
  long blu_time;
  long red_time;
  unsigned long swap_time =
      getChangePoint(); // init to game start time to avoid funky negations
};

#endif
