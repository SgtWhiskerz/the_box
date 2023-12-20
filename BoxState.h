#pragma once

#include "Arduino.h"

class BoxState {
public:
  virtual ~BoxState() = default;
  virtual BoxState *tick() = 0;

protected:
  unsigned long change = millis();
};
