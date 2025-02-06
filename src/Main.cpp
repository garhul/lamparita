#include "Main.h"

bool white_light_on = false;
uint8_t br_setting = 0;
uint8_t br_target = 0;

Animator* animator;
NeoPixelBusType* strip;

#define DIM_DELAY 24;  // 255 3seconds ~  6s

void attachHandlers() {
  ESPNOW_HANDLER::onCmdOff([]() {
    animator->setMode(ANIMATOR_MODE::OFF);
    white_light_on = false;
  });

  ESPNOW_HANDLER::onCmdSetFx([](uint8_t fx) {
    animator->setMode(ANIMATOR_MODE::PLAYING);
    animator->setFX(fx);
  });

  ESPNOW_HANDLER::onCmdSetFxBr([](uint8_t br) {
    animator->setMaxBrightness(br);
  });

  ESPNOW_HANDLER::onCmdSetFxSpeed([](uint8_t spd) {
    animator->setSpeedScale(spd);
  });

  ESPNOW_HANDLER::onCmdPause([]() {
    animator->setMode(ANIMATOR_MODE::PAUSED);
  });

  ESPNOW_HANDLER::onCmdResume([]() {
    animator->setMode(ANIMATOR_MODE::PLAYING);
  });

  ESPNOW_HANDLER::onCmdSetHsb([](uint8_t h, uint8_t s, uint8_t b) {
    strip->ClearTo(HslColor(h, s, b));
    strip->Show();
    animator->setMode(ANIMATOR_MODE::PAUSED);
  });
}

inline void dimToTarget() {
  if (br_setting < br_target) br_setting++;
}

#ifdef __USE_CONTROLS
void attachControls() {
  CONTROLS::onShortPress([]() {
    white_light_on = !white_light_on;
    if (white_light_on) {
      br_target = br_setting;
      br_setting = 0;  // reset brightness
    }

    Serial.printf("Lights %d\n", white_light_on);
  });

  CONTROLS::onLongPress([]() {
    // Turn off all lights

    Serial.println("Long press detected");
    animator->setMode(ANIMATOR_MODE::OFF);
    white_light_on = false;
  });

  CONTROLS::onBrChange([](uint8_t br) {
    br_setting = map(br, 0, 15, 0, 255);
    animator->setMaxBrightness(br_setting);
    Serial.printf("Change detected %d -- %d\n", br, br_setting);
  });
}
#endif

void setup() {
  Serial.begin(115200);
  delay(100);

  strip = new NeoPixelBusType(STRIP_LENGTH);

  strip->Begin();
  strip->ClearTo(RgbColor(20, 0, 0));
  strip->Show();
  delay(1000);

  animator = new Animator(strip);

#ifdef __USE_CONTROLS
  pinMode(WARM_CTRL, OUTPUT);
  pinMode(COLD_CTRL, OUTPUT);
  CONTROLS::begin();
  attachControls();
#endif

  ESPNOW_HANDLER::init();
  attachHandlers();

  Serial.println("----------------------- Setup complete -----------------------");
}

void loop() {
  static uint32_t dimm_t = 0;

  animator->update();

#ifdef __USE_CONTROLS
  CONTROLS::update();

  if (dimm_t < millis()) {
    dimToTarget();
    dimm_t = millis() + DIM_DELAY;
  }
  // update ligths state
  if (!white_light_on) {
    digitalWrite(COLD_CTRL, 0);
    digitalWrite(WARM_CTRL, 0);
  } else {
    analogWrite(COLD_CTRL, br_setting / 2);
    analogWrite(WARM_CTRL, br_setting);
  }

#endif
}
