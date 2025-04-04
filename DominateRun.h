#ifndef DOMINATE_RUN_H
#define DOMINATE_RUN_H

#include "BoxState.h"
#include "Config.h"

/**
 * Teams compete to hold control for the longest
 */
class DominateRun : public BoxState {
public:
  DominateRun(unsigned long);
  DominateRun(const DominateRun &) = default;
  DominateRun(DominateRun &&) = default;
  DominateRun &operator=(const DominateRun &) = default;
  DominateRun &operator=(DominateRun &&) = default;
  ~DominateRun() override;

  BoxState *tick() final;

private:
  ACTIVE_TEAM active_team = ACTIVE_TEAM::Neutral;
  ACTIVE_TEAM last_team = ACTIVE_TEAM::Neutral;
  unsigned long limit;
  unsigned long lst_loop = millis();
  unsigned long blu_time = 0;
  unsigned long red_time = 0;
  unsigned long swap_time = 0;
};

#endif
