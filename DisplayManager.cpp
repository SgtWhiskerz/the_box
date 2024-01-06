#include "DisplayManager.h"

constexpr int DOT_MASK = 0b11100000;

DisplayManager::DisplayManager() {
  for (auto &s : scrns) {
    s.setBrightness(3);
  }
}

DisplayManager &DisplayManager::get() {
  static DisplayManager instance;
  return instance;
}

void DisplayManager::dispMillis(const Timers disp, const unsigned long millis) {
  const int indx = static_cast<int>(disp);
  const int sec = static_cast<int>((millis / 1000) % 60);
  const int min = static_cast<int>(millis / 60000);
  TM1637Display timer = scrns[indx];
  timer.showNumberDecEx(sec, DOT_MASK, true, 2, 2);
  timer.showNumberDecEx(min, DOT_MASK, true, 2, 0);
}

void DisplayManager::dispSegments(const Timers disp, const uint8_t *seg) {
  const int indx = static_cast<int>(disp);
  TM1637Display timer = scrns[indx];
  timer.setSegments(seg);
}

void DisplayManager::dispClear(const Timers disp) {
  const int indx = static_cast<int>(disp);
  TM1637Display timer = scrns[indx];
  timer.clear();
}
