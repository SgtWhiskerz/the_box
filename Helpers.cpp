#include "Helpers.h"
#include "Config.h"
#include "FastLED.h"

constexpr int TOP_LED_PORT = 2;
constexpr int RHT_LED_PORT = 10;
constexpr int LFT_LED_PORT = 24;

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
  CFastLED::addLeds<NEOPIXEL, TOP_LED_PORT>(static_cast<CRGB *>(top_leds),
                                            NUM_LEDS);
  CFastLED::addLeds<NEOPIXEL, RHT_LED_PORT>(static_cast<CRGB *>(rhs_leds),
                                            NUM_LEDS);
  CFastLED::addLeds<NEOPIXEL, LFT_LED_PORT>(static_cast<CRGB *>(lhs_leds),
                                            NUM_LEDS);
}

void playHorn(unsigned long dur) {
  Serial1.write("awo0001s");
  Serial1.flush();
  delay(dur);
  Serial1.write("awo0000s");
  Serial1.flush();
}
