#include "Helpers.h"
#include "Config.h"
#include "FastLED.h"

constexpr int TOP_LED_PORT = 5;
constexpr int BTM_LED_PORT = 6;

constexpr int TOP_NUM_LEDS = 42;
constexpr int BTM_NUM_LEDS = 32;

CRGB top_leds[TOP_NUM_LEDS];
CRGB btm_leds[BTM_LED_PORT];

void displayColor(const CRGB color) {
  for (int i = 0; i < TOP_NUM_LEDS; i++) {
    top_leds[i] = color;
  }
  for (int i = 0; i < BTM_NUM_LEDS; i++) {
    btm_leds[i] = color;
  }
  FastLED.show();
}

void splitDispColor(CRGB color, bool side) {
  const int split = TOP_NUM_LEDS / 2;
  for (int i = 0; side && i < split; i++) {
    top_leds[i] = color;
  }
  for (int i = split; !side && i < TOP_NUM_LEDS; i++) {
    top_leds[i] = color;
  }
  FastLED.show();
}

void initLED() {
  CFastLED::addLeds<WS2812B, TOP_LED_PORT, GRB>(static_cast<CRGB *>(top_leds),
                                                TOP_NUM_LEDS);
  CFastLED::addLeds<WS2812B, BTM_LED_PORT, GRB>(static_cast<CRGB *>(btm_leds),
                                                BTM_NUM_LEDS);
}

void playHorn(unsigned long dur) {
  digitalWrite(HORN_PIN, LOW);
  delay(dur);
  digitalWrite(HORN_PIN, HIGH);
}
