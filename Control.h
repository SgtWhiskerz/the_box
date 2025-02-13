#ifndef CONTROL_H
#define CONTROL_H

#include "BoxState.h"

class Control : public BoxState {
public:
  Control(unsigned long);
  Control(const Control &) = default;
  Control(Control &&) = default;
  Control &operator=(const Control &) = default;
  Control &operator=(Control &&) = default;
  ~Control() override;

  BoxState *tick() final;

private:
  struct TeamState {
    int score = 0;
    unsigned long tap = 0;
  };
  unsigned long limit;
  TeamState blue;
  TeamState red;
};

#endif
