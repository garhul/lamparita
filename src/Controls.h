#pragma once
#include <Arduino.h>

#define TOGGLE_SW D5
#define WARM_CTRL D6
#define COLD_CTRL D7

#define ENC_A D3
#define ENC_B D2

#define LONG_PRESS_DURATION 500
#define BTN_PRESS_DEBOUNCE 5  // presses shorter than this will be ignored

#define DEBOUNCE_MS 20
#define MAX_ENCODER_VALUE 15
#define MIN_ENCODER_VALUE 0

namespace CONTROLS {
void begin();
void update();

void IRAM_ATTR _isr_btn();
void IRAM_ATTR _isr_enc();

void onBrChange(void (*callback)(uint8_t));

void onLongPress(void (*callback)(void));
void onShortPress(void (*callback)(void));

}  // namespace CONTROLS