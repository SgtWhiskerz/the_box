#include "Control.h"
#include "Config.h"
#include "DisplayManager.h"
#include "GameConfig.h"
#include "Helpers.h"

constexpr int TEAM_LOCK_DUR = 5_sec;

Control::Control(unsigned long duration) : limit(duration) {
  // ensure both locks are the same value
  // prevents accidental preferential treatment by a millisecond
  unsigned long temp = millis();
  // ensure lock is released as soon as possible
  // prevents player confusion if a button is pressed before
  // game runs for at least TEAM_LOCK milliseconds
  red.tap = temp - TEAM_LOCK_DUR;
  blue.tap = temp - TEAM_LOCK_DUR;
  playHorn(RING_START);
}

Control::~Control() {
  if (blue.score == red.score) {
    displayColor(CRGB::White);
  } else if (red.score > blue.score) {
    displayColor(CRGB::Red);
  } else {
    displayColor(CRGB::Blue);
  }
  playHorn(RING_END);
}

BoxState *Control::tick() {
  DisplayManager dm = DisplayManager::get();
  const bool blue_override = digitalRead(REM_1) == HIGH;
  const bool red_override = digitalRead(REM_2) == HIGH;
  static bool lst_boverride = blue_override;
  static bool lst_roverride = red_override;
  const unsigned long time = millis();
  const unsigned long elapsed = time - getChangePoint();
  const long remain = static_cast<long>(limit - elapsed);
  const long r_lock_dur = static_cast<long>(time - red.tap);
  const long b_lock_dur = static_cast<long>(time - blue.tap);
  if (blue_override && blue_override != lst_boverride) {
    blue.score--;
  }
  if (red_override && red_override != lst_roverride) {
    red.score--;
  }
  if (digitalRead(R_PIN) == HIGH && r_lock_dur > TEAM_LOCK_DUR) {
    red.tap = time;
    red.score++;
  }
  if (digitalRead(B_PIN) == HIGH && b_lock_dur > TEAM_LOCK_DUR) {
    blue.tap = time;
    blue.score++;
  }
  dm.dispMillis(DisplayManager::Timers::Center, remain);
  dm.dispNum(DisplayManager::Timers::Red, red.score);
  dm.dispNum(DisplayManager::Timers::Blue, blue.score);
  if (r_lock_dur < TEAM_LOCK_DUR) {
    if (r_lock_dur % 1000 < 500) {
      splitDispColor(CRGB::Red, true);
    } else {
      splitDispColor(CRGB::Black, true);
    }
  } else {
    splitDispColor(CRGB::White, true);
  }
  if (b_lock_dur < TEAM_LOCK_DUR) {
    if (b_lock_dur % 1000 < 500) {
      splitDispColor(CRGB::Blue);
    } else {
      splitDispColor(CRGB::Black);
    }
  } else {
    splitDispColor(CRGB::White);
  }
  if (elapsed > limit) {
    return new GameConfig();
  }
  lst_boverride = blue_override;
  lst_roverride = red_override;
  return this;
}
