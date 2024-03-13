#include "BoxState.h"
#include "Config.h"
#include "GameConfig.h"
#include "Helpers.h"

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  initLED();
  pinMode(B_PIN, INPUT);
  pinMode(R_PIN, INPUT);
  pinMode(MIN_5, INPUT);
  pinMode(MIN_10, INPUT);
  pinMode(MIN_15, INPUT);
  pinMode(READY, INPUT);
  pinMode(RESET, INPUT);
  pinMode(HEADACHE, OUTPUT);
  displayColor(CRGB::White);
  Serial1.write("awu2400s");
  Serial1.write("awi1200s");
  Serial1.write("awo0000s");
  Serial1.flush();
}

void loop() {
  static BoxState *state = new GameConfig();
  static BoxState *next = nullptr;
  static int r_btn_state = 0;
  static int lr_btn_state = 0;
  unsigned long tick_start = millis();

  next = state->tick();

  r_btn_state = digitalRead(RESET);
  if (r_btn_state != lr_btn_state && r_btn_state == HIGH) {
    Serial.println("[INFO] Transitioning to CONFIG");
    if (next != nullptr && next != state) {
      delete next;
    }
    next = new GameConfig();
  }
  if (next != state) {
    delete state;
    state = next;
    next = nullptr;
  }

  Serial.print("[DEBUG] Loop complete. Duration in milliseconds: ");
  Serial.println(millis() - tick_start);
  lr_btn_state = r_btn_state;
}
