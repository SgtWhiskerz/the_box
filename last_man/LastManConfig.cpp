#include "LastManConfig.h"
#include "Arduino.h"
#include "LastManGrace.h"
#include "Config.h"
#include "Helpers.h"

const uint8_t all_on[] = {0xff, 0xff, 0xff, 0xff};

BoxState *LastManConfig::tick() {
  long time = millis();
  if (time - last_blink > 1000) {
    if (on) {
      timer.setSegments(all_on);
    } else {
      timer.clear();
    }
    on = !on;
    last_blink = time;
  }
  if (digitalRead(MIN_5) == HIGH) {
    time_limit = 5_min;
    time_set = true;
  }
  if (digitalRead(MIN_10) == HIGH) {
    time_limit = 10_min;
    time_set = true;
  }
  if (digitalRead(MIN_15) == HIGH) {
    time_limit = 15_min;
    time_set = true;
  }
  if (time_set) {
    Serial.println("[INFO] Transitioning from CONFIG to GRACE");
    Serial.print("[INFO]\tSelected time limit: ");
    Serial.println(time_limit);
    return new LastManGrace(time_limit);
  }
  return this;
}
