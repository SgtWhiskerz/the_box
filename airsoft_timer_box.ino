#include "BoxState.h"
#include "Config.h"
#include "GameConfig.h"
#include "Helpers.h"

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
  pinMode(HEADACHE, OUTPUT); // NOTE: relay requires LOW for ON state (HIGH for OFF)
  digitalWrite(HEADACHE, HIGH);
  displayColor(CRGB::White);
  Serial.println("[INFO] Program started");
}

void loop() {
  static BoxState *state = new GameConfig();
  static BoxState *next = nullptr;
  static int lr_btn_state = 0;
  unsigned long tick_start = millis();

  next = state->tick();

  if (digitalRead(READY) == HIGH) {
    digitalWrite(HEADACHE, LOW);
  } else {
    digitalWrite(HEADACHE, HIGH);
  }

  const int r_btn_state = digitalRead(RESET);
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
