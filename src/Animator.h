#pragma once
#include <Strip.h>

#define TEST_DELAY 15
#define MAX_LENGTH 500  // size of buffer
#define REL_UNIT_BYTE 0.0039f
// #define BENCHMARK                 // uncomment to enable benchmarking
#define FRAME_DURATION 1000 / 48  // 48  // ~20ms per frame

#define UNIT_BYTE 0.0039f

// lightness max value is 0.5f
#define UNIT_L_BYTE 0.00195f

typedef struct {
  byte h;
  byte s;
  byte l;
} leds;

enum FX {
  NONE,
  RAINBOW,
  WAVEBOW,
  AURORA,
  OPPOSITES,
  HUE_SPLIT,
  CHASER,
  WHITE_AURORA,
  WHITE_CHASER,
  TRIP,
  ALBI,
  COUNT,
};

enum ANIMATOR_MODE {
  OFF,
  PLAYING,
  PAUSED
};

class Animator {
 public:
  Animator(NeoPixelBusType* strip);
  void clear();
  void setMaxBrightness(uint8_t b);
  void setSpeedScale(uint8_t spd);
  void setFX(uint8_t fx);
  void setMode(uint8_t mode);
  void update();
  void test();
  leds* getFrame();
  uint8_t getLength();

 private:
  uint8_t _fx;
  uint8_t _max_bright;
  uint16_t _length;
  uint16_t _frame_index;
  uint16_t _spd_delay;
  uint8_t _spd;
  leds _pixels[MAX_LENGTH];
  uint8_t _mode;
  NeoPixelBusType* _strip;

  void randomize();
  void updateFrameBuffer(uint8_t fx);

  void fx_aurora();
  void fx_rainbow();
  void fx_wavebow();
  void fx_opposites();
  void fx_hue_split();
  void fx_chaser();
  void fx_chaser_double();
  void fx_white_aurora();
  void fx_white_chaser();
  void fx_trip();
  void fx_albiCeleste();
};
