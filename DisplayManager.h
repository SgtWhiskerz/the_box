#pragma once

#include "TM1637Display.h"

constexpr int CNTR_CLK = 0;
constexpr int CNTR_DIG = 0;

constexpr int BLU_CLK = 0;
constexpr int BLU_DIG = 0;

constexpr int RED_CLK = 0;
constexpr int RED_DIG = 0;

class DisplayManager {
public:
  DisplayManager();
  enum class Timers : int { Center = 0, Blue, Red };
  static DisplayManager &get();

  void dispMillis(Timers, unsigned long);
  void dispSegments(Timers, const uint8_t *);
  void dispClear(Timers);

private:
  static DisplayManager *instance;
  TM1637Display scrns[3] = {TM1637Display(CNTR_CLK, CNTR_DIG),
                            TM1637Display(BLU_CLK, BLU_DIG),
                            TM1637Display(RED_CLK, RED_DIG)};
};
