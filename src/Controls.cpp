#include "Controls.h"

namespace CONTROLS {

bool state = false;
bool warm_on = false;
bool cold_on = false;

void (*onBrChangeCallback)(uint8_t val);
void (*onLongPressCallback)(void);
void (*onShortPressCallback)(void);

void IRAM_ATTR _isr_btn() {
  static uint32_t press_time = 0;
  static bool last_state = HIGH;

  if (millis() - press_time < BTN_PRESS_DEBOUNCE) {  // debounce
    return;
  }

  // prevent reentrancy:
  bool state = digitalRead(TOGGLE_SW);

  // if it is a button press we go from high to low
  if (last_state && !state) {
    press_time = millis();
    last_state = state;
  } else if (!last_state && state) {  // if it is a release then it goes from low to high
    last_state = state;
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
}

volatile uint8_t counter = MIN_ENCODER_VALUE;

/* Encoder reading and debouncing
see https://www.best-microcontroller-projects.com/rotary-encoder.html
and https://garrysblog.com/2021/03/20/reliably-debouncing-rotary-encoders-with-arduino-and-esp32/
*/

void IRAM_ATTR _isr_enc() {
  static volatile uint8_t prev_state = 3;                                                   // Lookup table index
  static volatile int8_t encval = 0;                                                        // Encoder value
  static const int8_t enc_states[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};  // Lookup table

  prev_state <<= 2;                                              // Remember previous state
  prev_state |= (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);  // Add current state of pin A and B

  if (digitalRead(ENC_A)) prev_state |= 0x02;  // Add current state of pin A
  if (digitalRead(ENC_B)) prev_state |= 0x01;  // Add current state of pin B

  encval += enc_states[(prev_state & 0x0f)];

  // Update counter if encoder has rotated a full indent, that is at least 4 steps
  if (encval > 3) {  // Four steps forward
    counter = (counter > 0) ? --counter : counter;
    encval = 0;
  } else if (encval < -3) {  // Four steps backwards
    counter = (counter < MAX_ENCODER_VALUE) ? ++counter : counter;
    encval = 0;
  }
}

void begin() {
  pinMode(TOGGLE_SW, INPUT_PULLUP);
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(TOGGLE_SW), _isr_btn, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_A), _isr_enc, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), _isr_enc, CHANGE);
}

void update() {
  static uint8_t last_br_change = 0;
  if (last_br_change != counter) {
    last_br_change = counter;
    if (onBrChangeCallback != nullptr) {
      onBrChangeCallback(counter);
    }
  }
}

void onLongPress(void (*callback)(void)) {
  onLongPressCallback = callback;
}

void onShortPress(void (*callback)(void)) {
  onShortPressCallback = callback;
}

void onBrChange(void (*callback)(uint8_t)) {
  onBrChangeCallback = callback;
}

}  // namespace CONTROLS