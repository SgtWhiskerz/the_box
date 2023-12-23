#pragma once

#include "../BoxState.h"

class LastManGrace : public BoxState {
public:
  LastManGrace(unsigned long);
  virtual ~LastManGrace() = default;

  BoxState *tick() final;

private:
  bool cnt_dwn = false;
  unsigned long limit;
};
