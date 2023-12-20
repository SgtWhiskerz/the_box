#pragma once

#include "BoxState.h"

class BoxGrace : public BoxState {
public:
  BoxGrace(unsigned long);
  virtual ~BoxGrace() = default;

  BoxState *tick() final;

private:
  bool cnt_dwn = false;
  unsigned long limit;
};
