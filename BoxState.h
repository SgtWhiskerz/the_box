#ifndef BOXSTATE_H
#define BOXSTATE_H

#include "Arduino.h"

class BoxState {
public:
  BoxState() = default;
  BoxState(BoxState &) = default;
  BoxState(BoxState &&) = default;
  BoxState &operator=(const BoxState &) = default;
  BoxState &operator=(BoxState &&) = default;
  virtual ~BoxState() = default;
  virtual BoxState *tick() = 0;

private:
  unsigned long change = millis();

protected:
  unsigned long getChangePoint() const { return change; }
};

#endif
