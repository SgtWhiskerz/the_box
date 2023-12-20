#pragma once

#include "BoxState.h"

class BoxConfig : public BoxState {
public:
  virtual ~BoxConfig() = default;
  BoxState *tick() final;

private:
  bool on = true;
  bool time_set = false;
  long time_limit = 0;
  unsigned long last_blink = 0;
};
