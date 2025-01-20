#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

// NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma400KbpsMethod>* bus;
// NeoPixelBus<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod>* bus;
// NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* bus;
// NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* bus;
// NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart0800KbpsMethod>* bus;
#define STRIP_LENGTH 30
// #define WEMOS_D1
#ifdef WEMOS_D1
typedef NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart1800KbpsMethod> NeoPixelBusType;  // USES D4
#else
typedef NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> NeoPixelBusType;  // USES TX
#endif