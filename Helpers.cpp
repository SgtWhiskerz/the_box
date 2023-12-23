#include "Helpers.h"
#include "Config.h"
#include "FastLED.h"

CRGB top_leds[NUM_LEDS];
CRGB rhs_leds[NUM_LEDS];
CRGB lhs_leds[NUM_LEDS];

void displayColor(const CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    top_leds[i] = color;
    rhs_leds[i] = color;
    lhs_leds[i] = color;
  }
  FastLED.show();
}

void initLED() {
  FastLED.addLeds<NEOPIXEL, 2>(top_leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 10>(rhs_leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 48>(lhs_leds, NUM_LEDS);
}
