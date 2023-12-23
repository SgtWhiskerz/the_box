#include "DisplayManager.h"

DisplayManager *DisplayManager::instance = nullptr;

DisplayManager::DisplayManager() {
  for (auto s : scrns) {
    s.setBrightness(3);
  }
}

DisplayManager &DisplayManager::get() {
  if (instance == nullptr) {
    instance = new DisplayManager();
  }
  return *instance;
}

void DisplayManager::dispMillis(const Timers disp, const unsigned long millis) {
  const int indx = static_cast<int>(disp);
  const int sec = (millis / 1000) % 60;
  const int min = millis / 60000;
  TM1637Display timer = scrns[indx];
  timer.showNumberDecEx(sec, 0b11100000, true, 2, 2);
  timer.showNumberDecEx(min, 0b11100000, true, 2, 0);
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
