#ifndef HELPERS_H
#define HELPERS_H

#include "FastLED.h"

constexpr int getSec(unsigned long millis) { return (millis / 1000) % 60; }
constexpr int getMin(unsigned long millis) { return millis / 60000; }

constexpr unsigned long operator""_min(unsigned long long min) { return min * 60000; }
constexpr long double operator""_min(long double min) { return min * 60000; }
constexpr unsigned long long operator""_sec(unsigned long long sec) { return sec * 1000; }
constexpr long double operator""_sec(long double sec) { return sec * 1000; }

void displayColor(CRGB color);

void initLED();

#endif
