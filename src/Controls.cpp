#include "Controls.h"

namespace CONTROLS {

bool state = false;
bool warm_on = false;
bool cold_on = false;
uint16_t pot_state = 0;

void (*onBrChangeCallback)(uint16_t pot_state);
void (*onLongPressCallback)(void);
void (*onShortPressCallback)(void);

void IRAM_ATTR _isr_btn() {
  static uint32_t press_time = 0;

  // prevent reentrancy:
  noInterrupts();
  bool state = digitalRead(TOGGLE_SW);

  if (!state) {
    press_time = millis();
  } else {
    if (millis() - press_time > LONG_PRESS_DURATION) {
      if (onLongPressCallback != nullptr) {
        onLongPressCallback();
      }
    } else {
      if (onShortPressCallback != nullptr) {
        onShortPressCallback();
      }
    }
  }
  interrupts();
}

void begin() {
  pinMode(TOGGLE_SW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TOGGLE_SW), _isr_btn, CHANGE);
}

void poll() {
  static uint32_t last_poll = 0;
  if (millis() - last_poll < 50) {
    return;
  }
  last_poll = millis();

  static uint16_t last_readout = 0;
  uint16_t readout = analogRead(POT_PIN);

  if (abs(last_readout - readout) < CHANGE_THRESHOLD) {
    return;
  }

  last_readout = readout;
  if (onLongPressCallback != nullptr) onBrChangeCallback(readout);
}

void onLongPress(void (*callback)(void)) {
  onLongPressCallback = callback;
}

void onShortPress(void (*callback)(void)) {
  onShortPressCallback = callback;
}

void onBrChange(void (*callback)(uint16_t)) {
  onBrChangeCallback = callback;
}

}  // namespace CONTROLS