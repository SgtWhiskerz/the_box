#include "FastLED.h"
#include "TM1637Display.h"

constexpr int NUM_LEDS = 4;
CRGB top_leds[NUM_LEDS];
CRGB rhs_leds[NUM_LEDS];
CRGB lhs_leds[NUM_LEDS];

// Team Buttons--------------------------------------------

constexpr int B_PRT = 8;
constexpr int R_PRT = 9;

// Game Time Buttons---------------------------------------

constexpr int MIN_5 = 7;
constexpr int MIN_10 = 6;
constexpr int MIN_15 = 5;

// Flow Control--------------------------------------------

constexpr int READY = 4;
constexpr int RESET = 3;

constexpr int HEADACHE = 52;

constexpr int D_CLK = 22;
constexpr int D_DIO = 23;
TM1637Display timer(D_CLK, D_DIO);

const uint8_t all_on[] = {0xff, 0xff, 0xff, 0xff};

// Timings-------------------------------------------------

constexpr unsigned long GRACE_PERIOD = 30000;
constexpr unsigned long LIMIT_SHOWN = 5000;
constexpr unsigned long RING_START = 750;
constexpr unsigned long RING_END = 1000;

enum class ACTIVE_TEAM { NEUTRAL, BLUE, RED };

constexpr int getSec(unsigned long millis) { return (millis / 1000) % 60; }

constexpr int getMin(unsigned long int millis) { return millis / 60000; }

inline void displayTime(const int min, const int sec) {
  timer.showNumberDecEx(sec, 0b11100000, true, 2, 2);
  timer.showNumberDecEx(min, 0b11100000, true, 2, 0);
}

inline void displayMillis(unsigned long milli) {
  int sec = getSec(milli);
  int min = getMin(milli);
  displayTime(min, sec);
}

inline void teamButtons(ACTIVE_TEAM &team) {
  const bool blue = digitalRead(B_PRT) == HIGH;
  const bool red = digitalRead(R_PRT) == HIGH;
  if (blue && red) {
    team = ACTIVE_TEAM::NEUTRAL;
  } else if (blue) {
    team = ACTIVE_TEAM::BLUE;
  } else if (red) {
    team = ACTIVE_TEAM::RED;
  }

  CRGB color = CRGB::White;
  switch (team) {
  case ACTIVE_TEAM::BLUE:
    color = CRGB::Blue;
    break;
  case ACTIVE_TEAM::RED:
    color = CRGB::Red;
    break;
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    top_leds[i] = color;
    rhs_leds[i] = color;
    lhs_leds[i] = color;
  }
  FastLED.show();
}

class BoxState {
public:
  virtual ~BoxState() = default;

  virtual BoxState *tick() = 0;

protected:
  unsigned long change = millis();
};

class BoxConfig : public BoxState {
public:
  BoxState *tick() final;

private:
  bool on = true;
};

class BoxRun : public BoxState {
public:
  BoxRun(unsigned long);

  BoxState *tick() final;

private:
  ACTIVE_TEAM winning = ACTIVE_TEAM::NEUTRAL;
  unsigned long limit;
};

BoxRun::BoxRun(unsigned long r_limit) : limit(r_limit) {}

BoxState *BoxRun::tick() {
  unsigned long time = millis();
  unsigned long elapsed = time - change;
  long remain = limit - elapsed;
  if (elapsed < RING_START) {
    digitalWrite(HEADACHE, HIGH);
  } else {
    digitalWrite(HEADACHE, LOW);
  }

  if (remain < 0) { // TODO: test this for game satisfaction
                    // should the game end at 00:00:00 or 00:00:??
    if (remain * -1 < RING_END) {
      digitalWrite(HEADACHE, HIGH);
    } else {
      digitalWrite(HEADACHE, LOW);
    }

    if (digitalRead(READY) == HIGH) {
      Serial.println("[INFO] Transitioning from RUNNING to CONFIG");
      return new BoxConfig();
    }
  } else {
    teamButtons(winning);
    displayMillis(remain);
  }
  return this;
}

class BoxGrace : public BoxState {
public:
  BoxGrace(unsigned long);

  BoxState *tick() final;

private:
  bool cnt_dwn = false;
  unsigned long limit;
};

BoxGrace::BoxGrace(unsigned long r_limit) : limit(r_limit) {}

BoxState *BoxGrace::tick() {
  unsigned long time = millis();
  unsigned long elapsed = time - change;
  if (!cnt_dwn) {
    displayMillis(limit);
    if (time - change > LIMIT_SHOWN) {
      cnt_dwn = true;
    }
  } else {
    displayMillis(GRACE_PERIOD - elapsed);
  }

  if (time - change > GRACE_PERIOD) {
    Serial.println("[INFO] Transitioning from GRACE to RUNNING");
    return new BoxRun(limit);
  }
  return this;
}

BoxState *BoxConfig::tick() {
  static unsigned long time = 0;
  bool time_set = false;
  long time_limit = 0;
  if (time - change > 1000) {
    if (on) {
      timer.setSegments(all_on);
    } else {
      timer.clear();
    }
    on = !on;
    time = millis();
  }
  if (digitalRead(MIN_5) == HIGH) {
    time_limit = 5 * 60000;
    time_set = true;
  }
  if (digitalRead(MIN_10) == HIGH) {
    time_limit = 10 * 60000;
    time_set = true;
  }
  if (digitalRead(MIN_15) == HIGH) {
    time_limit = 15 * 60000;
    time_set = true;
  }
  if (time_set) {
    Serial.println("[INFO] Transitioning from CONFIG to GRACE");
    Serial.print("[INFO]\tSelected time limit: ");
    Serial.println(time_limit);
    return new BoxGrace(time_limit);
  }
  return this;
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, 2>(top_leds, NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, _>(rhs_leds, NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, _>(lhs_leds, NUM_LEDS);
  pinMode(B_PRT, INPUT);
  pinMode(R_PRT, INPUT);
  pinMode(MIN_5, INPUT);
  pinMode(MIN_10, INPUT);
  pinMode(MIN_15, INPUT);
  pinMode(READY, INPUT);
  pinMode(RESET, INPUT);
  pinMode(HEADACHE, OUTPUT);
  timer.setBrightness(5);
}

void loop() {
  static BoxState *state = new BoxConfig();
  static BoxState *next = nullptr;
  unsigned long tick_start = millis();

  next = state->tick();

  if (digitalRead(RESET)) {
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
