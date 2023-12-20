#include "BoxConfig.h"
#include "BoxGrace.h"
#include "BoxRun.h"
#include "BoxState.h"
#include "Config.h"
#include "FastLED.h"
#include "Helpers.h"
#include "TM1637Display.h"

void setup() {
  Serial.begin(9600);
  initLED();
  pinMode(B_PIN, INPUT);
  pinMode(R_PIN, INPUT);
  pinMode(MIN_5, INPUT);
  pinMode(MIN_10, INPUT);
  pinMode(MIN_15, INPUT);
  pinMode(READY, INPUT);
  pinMode(RESET, INPUT);
  pinMode(HEADACHE, OUTPUT);
  timer.setBrightness(3);
  displayColor(CRGB::White);
}

void loop() {
  static BoxState *state = new BoxConfig();
  static BoxState *next = nullptr;
  unsigned long tick_start = millis();

  next = state->tick();

  if (digitalRead(RESET) == HIGH) {
    Serial.println("[INFO] Transitioning to CONFIG");
    delete next;
    next = new BoxConfig();
  }
  if (next != state) {
    delete state;
    state = next;
    next = nullptr;
  }

  Serial.print("[DEBUG] Loop complete. Duration in milliseconds: ");
  Serial.println(millis() - tick_start);
}
