#include <Animator.h>

Animator::Animator(NeoPixelBusType* strip) {
  this->_max_bright = 15;
  this->_frame_index = 0;
  this->_length = strip->PixelCount();
  this->_spd = 100;
  this->_spd_delay = 12;
  this->_mode = OFF;
  this->_strip = strip;
}

uint8_t Animator::getLength() {
  return this->_length;
}

void Animator::setFX(uint8_t fx) {
  this->_fx = fx;
  this->_frame_index = 0;
}

void Animator::setMaxBrightness(uint8_t b) {
  _max_bright = b;
}

void Animator::setSpeedScale(uint8_t spd) {
  _spd = spd;
  _spd_delay = 100 / _spd;
}

void Animator::randomize() {
  uint8_t px = 0;
  for (px = 0; px < this->_length; px++) {
    this->_pixels[px].h = byte(random(255));
    this->_pixels[px].s = 255;
    this->_pixels[px].l = byte(random(this->_max_bright));
  }
}

void Animator::clear() {
  for (uint8_t px = 0; px < this->_length; px++) {
    this->_pixels[px].h = 0;
    this->_pixels[px].s = 0;
    this->_pixels[px].l = 0;
  }
}

void Animator::fx_chaser() {
  static byte hue = 90;
  static byte chaser_len = 7;  // 7 _pixels length
  static byte space = 7;
  static byte idx = 0;
  static byte adv = 0;
  byte n = 0;
  byte br = 0;
  leds tmp;

  if (this->_frame_index == 0) {
    for (n = 0; n < this->_length; n++) {
      idx = ++idx % (chaser_len + space);

      if (idx < chaser_len) {
        br = (_max_bright / chaser_len) * idx;
        _pixels[n].s = 255;
      } else {
        br = 0;
      }

      _pixels[n].l = br;
      _pixels[n].h = hue;
    }
  }

  if (this->_frame_index % this->_spd_delay == 0) {
    adv = ++adv % this->_length;
    if (adv == 0)
      hue += 8;

    tmp = _pixels[0];
    for (n = 0; n < this->_length - 1; n++) {
      _pixels[n] = _pixels[n + 1];
      _pixels[n].h = hue;
    }
    _pixels[this->_length - 1] = tmp;
    _pixels[this->_length - 1].h = hue;
  }
}

void Animator::fx_white_chaser() {
  static byte chaser_len = 7;  // 7 _pixels length
  static byte space = 7;
  static byte idx = 0;
  byte n = 0;
  byte br = 0;
  leds tmp;

  if (this->_frame_index == 0) {
    for (n = 0; n < this->_length; n++) {
      idx = ++idx % (chaser_len + space);

      if (idx < chaser_len) {
        br = (_max_bright / chaser_len) * idx;
        _pixels[n].h = 0;
      } else {
        br = 0;
      }
      _pixels[n].l = br;
    }
  }

  if (this->_frame_index % this->_spd_delay == 0) {
    tmp = _pixels[0];
    for (n = 0; n < this->_length - 1; n++) {
      _pixels[n] = _pixels[n + 1];
    }
    _pixels[this->_length - 1] = tmp;
  }
}

void Animator::fx_trip() {
  static byte chaser_len = 10;  // 7 _pixels length
  static byte space = 20;
  byte idx = 0;
  byte n = 0;
  leds tmp;

  if (this->_frame_index == 0) {
    this->randomize();

    for (n = 0; n < this->_length; n++) {
      idx = ++idx % (chaser_len + space);
      if (idx < chaser_len) {
        _pixels[n].l = (_max_bright / chaser_len) * idx;
      } else {
        _pixels[n].l = _max_bright / chaser_len;
      }
    }
  }

  if (this->_frame_index % this->_spd_delay == 0) {
    tmp = _pixels[0];
    for (n = 0; n < this->_length - 1; n++) {
      _pixels[n].l = _pixels[n + 1].l;
    }
    _pixels[this->_length - 1].l = tmp.l;
  }
}
// simple hue transition
void Animator::fx_rainbow() {
  static byte hue_inc = 0;

  if (this->_frame_index % this->_spd_delay == 0) {
    hue_inc++;
  }

  byte n = 0;
  for (n = 0; n < this->_length; n++) {
    _pixels[n].h = (n * 2) + hue_inc;
    _pixels[n].l = _max_bright;
    _pixels[n].s = 255;
  }
}

// TODO:: check the effect
void Animator::fx_wavebow() {
  static byte h_center = 0;
  static byte br_center = 0;
  static uint8_t br_per_led = 1;
  static int dir = -1;
  byte n = 0;

  if (this->_frame_index == 0) {
    // set the base for the effect
    br_per_led = ceil(_max_bright / this->_length);
    for (n = 0; n < this->_length; n++) {
      _pixels[n].l = _max_bright;
      _pixels[n].s = 255;
      _pixels[n].h = h_center + n;
    }
  }

  for (n = 0; n < this->_length; n++) {
    _pixels[n].h = h_center + n;
    _pixels[n].s = 255;
    _pixels[n].l = (br_center + n) * br_per_led;
  }

  if (this->_frame_index % this->_spd_delay == 0) {
    h_center++;

    if (br_center == this->_length || br_center == 0) {
      dir = dir * -1;
    }
    br_center += dir;
  }
}

/** Split strip in two with a 180 degree in between hues */
void Animator::fx_opposites() {
  static byte hue = 0;

  if (this->_frame_index % this->_spd_delay == 0) {
    hue++;
  }

  byte n = 0;

  for (n = 0; n < this->_length; n++) {
    if (n < this->_length / 2) {
      _pixels[n].h = hue + 128;
      _pixels[n].s = 255;
      _pixels[n].l = _max_bright;
    } else {
      _pixels[n].h = hue;
      _pixels[n].s = 255;
      _pixels[n].l = _max_bright;
    }
  }
}

// area efect (hue from 0 + increment on half of the strip)
void Animator::fx_hue_split() {
  byte n = 0;
  static byte h = 0;

  for (n = 0; n < this->_length; n++) {
    if (n < (this->_length / 2)) {
      _pixels[n].h = n * (255 / this->_length) + h;
    } else {
      _pixels[n].h = (this->_length - n - 1) * (255 / this->_length) + h;
    }

    _pixels[n].l = _max_bright;
    _pixels[n].s = 255;
  }

  if (this->_frame_index % this->_spd_delay == 0) {
    h++;
  }
}

void Animator::fx_aurora() {
  static byte hue_inc = 0;
  static int dirs[MAX_LENGTH];

  if (this->_frame_index == 0) {
    for (int i = 0; i < this->_length; i++) {
      _pixels[i].l = i % 2 * _max_bright;
    }
  }

  if (this->_frame_index % this->_spd_delay == 0) {
    hue_inc++;
  }

  for (int n = 0; n < this->_length; n++) {
    if (this->_frame_index % this->_spd_delay == 0) {
      if (_pixels[n].l == _max_bright) {  // we should start reducing
        dirs[n] = -1;
      } else if (_pixels[n].l <= 0) {
        dirs[n] = 1;
      }
      _pixels[n].l += dirs[n];
    }

    _pixels[n].h = (n * 2) + hue_inc;
    _pixels[n].s = 255;
  }
}

void Animator::fx_white_aurora() {
  static int dirs[MAX_LENGTH];

  if (this->_frame_index == 0) {
    for (int i = 0; i < this->_length; i++) {
      _pixels[i].l = i % 2 * _max_bright;
    }
  }

  for (int n = 0; n < this->_length; n++) {
    if (this->_frame_index % this->_spd_delay == 0) {
      if (_pixels[n].l == _max_bright) {  // we should start reducing
        dirs[n] = -1;
      } else if (_pixels[n].l <= 2) {
        dirs[n] = 1;
      }
      _pixels[n].l += dirs[n];
    }

    _pixels[n].h = 0;
    _pixels[n].s = 0;
  }
}

// oscilate brightness up and down within 50% of maximum range, (if max bright is 50 go from 40 to 50 and back)
void Animator::fx_albiCeleste() {
  static byte dir = 1;
  static uint8 br = 0;
  uint8 c = br / 3;
  uint8 upper_limit = _max_bright;
  uint8 lower_limit = ceil(_max_bright * .5);
  uint8 inc = ceil((upper_limit) / 24);

  if (this->_frame_index == 0) {
    br = lower_limit;
  }

  if (this->_frame_index % this->_spd_delay == 0) {
    if (br > upper_limit) {
      br = upper_limit;
    } else if (br < lower_limit) {
      br = lower_limit;
    }

    if (br == upper_limit) {
      dir = -1;
    } else if (br == lower_limit) {
      dir = 1;
    }

    br += (inc * dir);
    c = br / 3;
  }

  for (unsigned int n = 0; n < this->_length; n++) {
    if (n < this->_length / 3) {
      _pixels[n].h = c;
      _pixels[n].s = c;
      _pixels[n].l = br;
    } else if (n < ((this->_length / 3) * 2)) {
      _pixels[n].h = br;
      _pixels[n].s = br;
      _pixels[n].l = br;
    } else {
      _pixels[n].h = c;
      _pixels[n].s = c;
      _pixels[n].l = br;
    }
  }
}

void Animator::updateFrameBuffer(uint8_t fx) {
  this->_frame_index++;

  switch (fx) {
    case FX::AURORA:
      this->fx_aurora();
      break;

    case FX::WHITE_AURORA:
      this->fx_white_aurora();
      break;

    case FX::RAINBOW:
      this->fx_rainbow();
      break;

    case FX::WAVEBOW:
      this->fx_wavebow();
      break;

    case FX::OPPOSITES:
      this->fx_opposites();
      break;

    case FX::HUE_SPLIT:
      this->fx_hue_split();
      break;

    case FX::CHASER:
      this->fx_chaser();
      break;

    case FX::WHITE_CHASER:
      this->fx_white_chaser();
      break;

    case FX::TRIP:
      this->fx_trip();
      break;

    case FX::ALBI:
      this->fx_albiCeleste();
      break;
  }
}

// TODO:: allow to recalculate every animation frame with a different brightness (dynamic brightness that works during pause)
leds* Animator::getFrame() {
  // calculate frame

#ifdef BENCHMARK
  unsigned long start = micros();
  updateFrameBuffer(this->_fx);
  unsigned long end = micros();
  Serial.printf("Frame calculation took: %lu for fx %i\n", end - start, this->_fx);
#else
  updateFrameBuffer(this->_fx);
#endif
  return _pixels;
}

void Animator::setMode(uint8_t mode) {
  this->_mode = mode;
}

void Animator::test() {
  // Serial.printf("testing strip of length %d \n", this->_length);
  this->_strip->ClearTo(RgbColor(0, 0, 0), 0, this->_length);
  this->_strip->Show();
  delay(500);

  this->_strip->ClearTo(RgbColor(20, 0, 0), 0, this->_length);
  this->_strip->Show();
  delay(500);

  this->_strip->ClearTo(RgbColor(0, 20, 0), 0, this->_length);
  this->_strip->Show();
  delay(500);

  this->_strip->ClearTo(RgbColor(0, 0, 20), 0, this->_length);
  this->_strip->Show();
  delay(500);

  this->_strip->ClearTo(RgbColor(0, 0, 0), 0, this->_length);
  this->_strip->Show();
}

void Animator::update() {
  static unsigned long next_frame_t = 0;

  if (next_frame_t > millis())
    return;  // not time to update

  next_frame_t = millis() + FRAME_DURATION;

  if (this->_mode == OFF) {
    this->_strip->ClearTo(RgbColor(0, 0, 0));
    this->_strip->Show();
    return;
  }

  if (this->_mode == PLAYING) {
    leds* frame = this->getFrame();

    for (unsigned int n = 0; n < this->_length; n++) {
      this->_strip->SetPixelColor(n, HslColor(frame[n].h * UNIT_BYTE,
                                              frame[n].s * UNIT_BYTE,
                                              frame[n].l * UNIT_L_BYTE));
    }
    this->_strip->Show();
  }
}