#ifndef CONFIG_H
#define CONFIG_H

// Team Buttons--------------------------------------------

constexpr int B_PIN = 8;
constexpr int R_PIN = 7;

// Remote Relay--------------------------------------------

constexpr int REM_1 = 9;
constexpr int REM_2 = 10;
constexpr int REM_3 = 11;
// constexpr int REM_4 = ;
constexpr int REM_5 = 12;
constexpr int REM_RESET = 13;

constexpr int HORN_PIN = 4;

// Timings-------------------------------------------------

constexpr unsigned long GRACE_PERIOD = 10000;
constexpr unsigned long LIMIT_SHOWN = 5000;
constexpr unsigned long RING_START = 1000;
constexpr unsigned long RING_END = 1000;

enum class ACTIVE_TEAM : int { Neutral = 0, Blue, Red };

#endif
