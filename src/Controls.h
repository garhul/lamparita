#pragma once
#include <Arduino.h>

#define CHANGE_THRESHOLD 5  // how much we need to move the pot to trigger a change
#define THRESHOLD_MIN 10
#define THJRESHOLD_MAX 100

#define TOGGLE_SW D5
#define WARM_CTRL D6
#define COLD_CTRL D7
#define POT_PIN A0

#define LONG_PRESS_DURATION 500

namespace CONTROLS {
void begin();
void poll();

void IRAM_ATTR _isr_btn();

void onBrChange(void (*callback)(uint16_t));

void onLongPress(void (*callback)(void));
void onShortPress(void (*callback)(void));

}  // namespace CONTROLS