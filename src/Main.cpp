#include "Main.h"

#define WARM_ON 0b10
#define COLD_ON 0b01
#define LIGHTS_OFF 0b00

uint8_t lights_mode = 0b00;
uint8_t br_setting = 0;

Animator* animator;
NeoPixelBusType* strip;

void attachHandlers() {
  ESPNOW_HANDLER::onCmdOff([]() {
    animator->setMode(ANIMATOR_MODE::OFF);
    lights_mode = LIGHTS_OFF;
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

#ifdef __USE_CONTROLS
void attachControls() {
  CONTROLS::onShortPress([]() {
    lights_mode = (lights_mode + 1) % 4;
    Serial.printf("Lights mode: %d\n", lights_mode);
  });

  CONTROLS::onLongPress([]() {
    // Turn off all lights

    Serial.println("Long press detected");
    animator->setMode(ANIMATOR_MODE::OFF);
    digitalWrite(WARM_CTRL, 0);
    digitalWrite(COLD_CTRL, 0);
  });

  CONTROLS::onBrChange([](uint16_t pot_state) {
    br_setting = map(pot_state, 0, 1023, 0, 255);
    animator->setMaxBrightness(br_setting);
    Serial.printf("Change detected %d -- %d\n", pot_state, br_setting);
  });
}
#endif

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(WARM_CTRL, OUTPUT);
  pinMode(COLD_CTRL, OUTPUT);

  strip = new NeoPixelBusType(STRIP_LENGTH);

  strip->Begin();
  strip->ClearTo(RgbColor(20, 0, 0));
  strip->Show();
  delay(1000);

  animator = new Animator(strip);

  Serial.println("testing animator");
  animator->test();
  Serial.println("testing animator end");

#ifdef __USE_CONTROLS
  CONTROLS::begin();
  attachControls();
#endif

  ESPNOW_HANDLER::init();
  attachHandlers();

  Serial.println("----------------------- Setup complete -----------------------");
}

void loop() {
  animator->update();

  // do controls polling (for potentiometer readout)
#ifdef __USE_CONTROLS
  CONTROLS::poll();
#endif

  // update ligths state
  if (lights_mode == LIGHTS_OFF) {
    digitalWrite(COLD_CTRL, 0);
    digitalWrite(WARM_CTRL, 0);
  } else if (lights_mode == COLD_ON) {
    analogWrite(COLD_CTRL, br_setting);
    digitalWrite(WARM_CTRL, 0);
  } else if (lights_mode == WARM_ON) {
    analogWrite(WARM_CTRL, br_setting);
    digitalWrite(COLD_CTRL, 0);
  } else {
    analogWrite(WARM_CTRL, br_setting);
    analogWrite(COLD_CTRL, br_setting);
  }

  animator->test();
}
