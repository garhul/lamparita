#pragma once
#define __USE_CONTROLS  // Comment this line to disable hardware controls

#include <Animator.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Handler.h>
#include <NeoPixelBus.h>
#include <Strip.h>

#ifdef __USE_CONTROLS
#include <Controls.h>
#endif