#ifndef CONFIG_H
#define CONFIG_H

// Team Buttons--------------------------------------------

constexpr int B_PIN = 8;
constexpr int R_PIN = 7;

// Game Time Buttons---------------------------------------

constexpr int MIN_5 = 9;
constexpr int MIN_10 = 10;
constexpr int MIN_15 = 11;

// Flow Control--------------------------------------------

constexpr int READY = 12;
constexpr int RESET = 13;

constexpr int HEADACHE = 4;

// Timings-------------------------------------------------

constexpr unsigned long GRACE_PERIOD = 10000;
constexpr unsigned long LIMIT_SHOWN = 5000;
constexpr unsigned long RING_START = 1000;
constexpr unsigned long RING_END = 1000;

enum class ACTIVE_TEAM : int { Neutral = 0, Blue, Red };

#endif
