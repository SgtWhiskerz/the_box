#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "TM1637Display.h"

constexpr int CNTR_CLK = 22;
constexpr int CNTR_DIG = 23;

constexpr int BLU_CLK = 48;
constexpr int BLU_DIG = 49;

constexpr int RED_CLK = 50;
constexpr int RED_DIG = 51;

class DisplayManager {
public:
  DisplayManager();
  enum class Timers : int { Center = 0, Blue, Red };
  static DisplayManager &get();

  void dispMillis(Timers, unsigned long);
  void dispSegments(Timers, const uint8_t *);
  void dispNum(Timers, const int);
  void dispClear(Timers);

private:
  TM1637Display scrns[3] = {TM1637Display(CNTR_CLK, CNTR_DIG),
                            TM1637Display(BLU_CLK, BLU_DIG),
                            TM1637Display(RED_CLK, RED_DIG)};
};

#endif
