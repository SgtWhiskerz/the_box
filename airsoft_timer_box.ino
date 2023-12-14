#include "FastLED.h"
#include "TM1637Display.h"

constexpr int NUM_LEDS = 4;
CRGB leds[NUM_LEDS];

constexpr int B_PRT = 8;
constexpr int R_PRT = 9;

constexpr int MIN_5 = 7;
constexpr int MIN_10 = 6;
constexpr int MIN_15 = 5;

constexpr int READY = 4;
constexpr int RESET = 3;

constexpr int HEADACHE = 52;

constexpr int D_CLK = 22;
constexpr int D_DIO = 23;
TM1637Display timer(D_CLK, D_DIO);

const uint8_t all_on[] = {0xff, 0xff, 0xff, 0xff};

constexpr unsigned long GRACE_PERIOD = 30000;
constexpr unsigned long LIMIT_SHOWN = 5000;
constexpr unsigned long RING_START = 750;
constexpr unsigned long RING_END = 1000;

enum class BOX_STATE {
    NONE,
    CONFIG,
    GRACE,
    RUNNING
};

struct BoxStateMachine {
    BOX_STATE state = BOX_STATE::NONE;
    BOX_STATE last = BOX_STATE::NONE;
    unsigned long change = 0;

    BoxStateMachine() {}

    void transitionTo(BOX_STATE new_state) {
        last = state;
        state = new_state;
        change = millis();
    }
};

void setup() {
    Serial.begin(9600);
    FastLED.addLeds<NEOPIXEL, 2>(leds, NUM_LEDS);
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

constexpr int getSec(unsigned long millis) {
    return (millis / 1000) % 60;
}

constexpr int getMin(unsigned long int millis) {
    return millis / 60000;
}

void teamButtons() {
    static bool team{false};
    if(digitalRead(B_PRT) == HIGH) { team = true; }
    else if(digitalRead(R_PRT) == HIGH) { team = false; }

    for(int i = 0; i < NUM_LEDS; i++) {
        if(team) {
            leds[i] = CRGB::Blue;
        }
        else {
            leds[i] = CRGB::Red;
        }
    }
    FastLED.show();
}

void loop() {
    static BOX_STATE state{BOX_STATE::CONFIG};
    static BOX_STATE last{BOX_STATE::NONE};
    static unsigned long time_limit = 0;

    if(digitalRead(RESET)) { state = BOX_STATE::CONFIG; }

    switch(state) {
        case BOX_STATE::CONFIG: {
            static unsigned long disp_change{millis()};
            static bool on{true};
            if(millis() - disp_change > 1000) {
                if(on) { timer.setSegments(all_on); }
                else { timer.clear(); }
                disp_change = millis();
                on = !on;
            }
            if(digitalRead(MIN_5) == HIGH) {
                time_limit = 5 * 60000;
                state = BOX_STATE::GRACE;
            }
            if(digitalRead(MIN_10) == HIGH) {
                time_limit = 10 * 60000;
                state = BOX_STATE::GRACE;
            }
            if(digitalRead(MIN_15) == HIGH) {
                time_limit = 15 * 60000;
                state = BOX_STATE::GRACE;
            }

            last = BOX_STATE::CONFIG;
            break;
        }
        case BOX_STATE::GRACE: {
            static unsigned long grace_begin{0};
            static bool shown_time{false};
            if(last != BOX_STATE::GRACE) {
                grace_begin = millis();
                shown_time = false;
            }
            unsigned long elapsed_time = millis() - grace_begin;
            if(!shown_time) {
                int sec = getSec(time_limit);
                int min = getMin(time_limit);
                timer.showNumberDecEx(sec, 0b11100000, true, 2, 2);
                timer.showNumberDecEx(min, 0b11100000, true, 2, 0);
                if(elapsed_time > LIMIT_SHOWN) {
                    shown_time = true;
                }
            } else {
                unsigned long rem_grace = GRACE_PERIOD - elapsed_time;
                int min = getMin(rem_grace);
                int sec = getSec(rem_grace);
                timer.showNumberDecEx(sec, 0b111000000, true, 2, 2);
                timer.showNumberDecEx(min, 0b111000000, true, 2, 0);
            }

            if(elapsed_time > GRACE_PERIOD) {
                state = BOX_STATE::RUNNING;
            }
            
            last = BOX_STATE::GRACE;
            break;
        }
        case BOX_STATE::RUNNING: {
            static unsigned long match_begin{millis()};
            if(last != BOX_STATE::RUNNING) {
                match_begin = millis();
            }
            if(millis() - match_begin < RING_START) { digitalWrite(HEADACHE, HIGH); }
            else { digitalWrite(HEADACHE, LOW); }

            unsigned long elapsed = millis() - match_begin;
            long remain = time_limit - elapsed;
            
            if(remain <= 0) {
                if(remain * -1 < RING_END) { digitalWrite(HEADACHE, HIGH); }
                else { digitalWrite(HEADACHE, LOW); }

                if(digitalRead(READY)) { state = BOX_STATE::CONFIG; }
            } else {
                teamButtons();
                int sec = getSec(remain);
                int min = getMin(remain);
                timer.showNumberDecEx(sec, 0b111000000, true, 2, 2);
                timer.showNumberDecEx(min, 0b111000000, true, 2, 0);
            }

            last = BOX_STATE::RUNNING;
            break;
        }
    }
}
