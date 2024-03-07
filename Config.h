#ifndef CONFIG_H
#define CONFIG_H

constexpr int NUM_LEDS = 4;

// Team Buttons--------------------------------------------

constexpr int B_PIN = 8;
constexpr int R_PIN = 9;

// Game Time Buttons---------------------------------------

constexpr int MIN_5 = 7;
constexpr int MIN_10 = 6;
constexpr int MIN_15 = 5;

// Flow Control--------------------------------------------

constexpr int READY = 4;
constexpr int RESET = 3;

constexpr int HEADACHE = 52;

// Timings-------------------------------------------------

constexpr unsigned long GRACE_PERIOD = 10000;
constexpr unsigned long LIMIT_SHOWN = 5000;
constexpr unsigned long RING_START = 750;
constexpr unsigned long RING_END = 1000;

enum class ACTIVE_TEAM : int { Neutral = 0, Blue, Red };

#endif
