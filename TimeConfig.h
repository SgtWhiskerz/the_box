#ifndef TIME_CONFIG_H
#define TIME_CONFIG_H

#include "BoxState.h"
#include "GameGrace.h"

class TimeConfig : public BoxState {
public:
  TimeConfig(GameGrace::Games);
  TimeConfig(const TimeConfig &) = default;
  TimeConfig(TimeConfig &&) = default;
  TimeConfig &operator=(const TimeConfig &) = default;
  TimeConfig &operator=(TimeConfig &&) = default;
  ~TimeConfig() override = default;
  BoxState *tick() final;

private:
  GameGrace::Games act_game;
  bool on = true;
  bool time_set = false;
  long time_limit = 0;
  unsigned long last_blink = 0;
};

#endif
