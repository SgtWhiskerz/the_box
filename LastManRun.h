#ifndef LAST_MAN_RUN_H
#define LAST_MAN_RUN_H

#include "BoxState.h"
#include "Config.h"

class LastManRun : public BoxState {
public:
  LastManRun(unsigned long);
  LastManRun(LastManRun &) = default;
  LastManRun(LastManRun &&) = default;
  LastManRun &operator=(const LastManRun &) = default;
  LastManRun &operator=(LastManRun &&) = default;
  ~LastManRun() override;

  BoxState *tick() final;

private:
  ACTIVE_TEAM active_team = ACTIVE_TEAM::Neutral;
  unsigned long limit;
};

#endif
