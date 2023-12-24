#pragma once

#include "FastLED.h"

constexpr int getSec(unsigned long millis) { return (millis / 1000) % 60; }
constexpr int getMin(unsigned long millis) { return millis / 60000; }

constexpr long operator""_min(unsigned long long min) { return min * 60000; }
constexpr long operator""_min(long double min) { return min * 60000; }
constexpr long operator""_sec(unsigned long long sec) { return sec * 1000; }
constexpr long operator""_sec(long double sec) { return sec * 1000; }

void displayColor(CRGB color);

void initLED();
