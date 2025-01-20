#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Strip.h>
#include <espnow.h>

#define __DEBUG
namespace ESPNOW_HANDLER {

enum CMD {
  CMD_OFF,
  CMD_SET_FX,
  CMD_SET_FX_BR,
  CMD_SET_FX_SPEED,
  CMD_PAUSE,
  CMD_RESUME,
  CMD_SET_HSB,
  CMD_SET_RGB,
};

typedef struct auroraMsg {
  uint8_t cmd;
  uint8_t payload[32];
} auroraMsg;

void init();

void handleMessage(uint8_t *mac, uint8_t *incomingData, uint8_t len);
void processCommand(auroraMsg *msg);

void onCmdOff(void (*callback)(void));

void onCmdSetFx(void (*callback)(uint8_t));

void onCmdSetFxBr(void (*callback)(uint8_t));

void onCmdSetFxSpeed(void (*callback)(uint8_t));

void onCmdSetHsb(void (*callback)(uint8_t, uint8_t, uint8_t));

void onCmdSetRgb(void (*callback)(uint8_t, uint8_t, uint8_t));

void onCmdPause(void (*callback)(void));

void onCmdResume(void (*callback)(void));

}  // namespace ESPNOW_HANDLER
