#include "BoxState.h"
#include "Config.h"
#include "GameConfig.h"
#include "Helpers.h"

void setup() {
  Serial.begin(9600);
  initLED();
  pinMode(B_PIN, INPUT);
  pinMode(R_PIN, INPUT);
  pinMode(REM_1, INPUT);
  pinMode(REM_2, INPUT);
  pinMode(REM_3, INPUT);
  pinMode(REM_5, INPUT);
  pinMode(REM_RESET, INPUT);
  pinMode(HORN_PIN, OUTPUT); // NOTE: relay requires LOW for ON state (HIGH for OFF)
  digitalWrite(HORN_PIN, HIGH);
  displayColor(CRGB::White);
  Serial.println("[INFO] Program started");
}

void loop() {
  static BoxState *state = new GameConfig();
  static BoxState *next = nullptr;
  static int lr_btn_state = 0;
  unsigned long tick_start = millis();

  next = state->tick();

  if (digitalRead(REM_5) == HIGH) {
    digitalWrite(HORN_PIN, LOW);
  } else {
    digitalWrite(HORN_PIN, HIGH);
  }

  const int r_btn_state = digitalRead(REM_RESET);
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
