#include "Handler.h"

namespace ESPNOW_HANDLER {

void (*cmdOffHandler)(void);
void (*cmdSetFxSpeedHandler)(uint8_t);
void (*cmdSetFxBrHandler)(uint8_t);
void (*cmdSetFxHandler)(uint8_t);
void (*cmdSetHsbHandler)(uint8_t, uint8_t, uint8_t);
void (*cmdResumeHandler)(void);
void (*cmdPauseHandler)(void);
void (*cmdSetRgbHandler)(uint8_t, uint8_t, uint8_t);

void init() {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("ESP-NOW Initialized");
    delay(1000);
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(handleMessage);
  Serial.printf("ESP Board MAC Address:  %s\n", WiFi.macAddress().c_str());
}

void handleMessage(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  auroraMsg data;
  memcpy(&data, incomingData, sizeof(data));

#ifdef __DEBUG
  Serial.println("-----------------Received Data-----------------------------");
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("cmd: ");
  Serial.println(data.cmd);
  Serial.print("payload: ");

  for (uint8_t i = 0; i < sizeof(data.payload); i++) {
    Serial.printf("%02X ", data.payload[i]);
  }

  Serial.println("-----------------------------------------------------------");
#endif

  processCommand(&data);
}

void onCmdOff(void (*callback)(void)) {
  cmdOffHandler = callback;
}

void onCmdSetFx(void (*callback)(uint8_t)) {
  cmdSetFxHandler = callback;
}

void onCmdSetFxBr(void (*callback)(uint8_t)) {
  cmdSetFxBrHandler = callback;
}

void onCmdSetFxSpeed(void (*callback)(uint8_t)) {
  cmdSetFxSpeedHandler = callback;
}

void onCmdSetHsb(void (*callback)(uint8_t, uint8_t, uint8_t)) {
  cmdSetHsbHandler = callback;
}

void onCmdSetRgb(void (*callback)(uint8_t, uint8_t, uint8_t)) {
  cmdSetRgbHandler = callback;
}

void onCmdPause(void (*callback)(void)) {
  cmdPauseHandler = callback;
}

void onCmdResume(void (*callback)(void)) {
  cmdResumeHandler = callback;
}

void processCommand(auroraMsg *msg) {
  switch (msg->cmd) {
    case CMD_OFF:
      cmdOffHandler();
      break;

    case CMD_SET_FX:
      cmdSetFxHandler(msg->payload[0]);
      break;

    case CMD_SET_FX_BR:
      cmdSetFxBrHandler(msg->payload[0]);
      break;

    case CMD_SET_FX_SPEED:
      cmdSetFxSpeedHandler(msg->payload[0]);
      break;

    case CMD_SET_HSB:
      cmdSetHsbHandler(
          msg->payload[0],
          msg->payload[1],
          msg->payload[2]);
      break;

    case CMD_SET_RGB:
      cmdSetRgbHandler(
          msg->payload[0],
          msg->payload[1],
          msg->payload[2]);
      break;

    case CMD_PAUSE:
      cmdPauseHandler();
      break;

    case CMD_RESUME:
      cmdResumeHandler();
      break;

    default:
      Serial.printf("Unknown command: %i\n", msg->cmd);
      break;
  }
}
}  // namespace ESPNOW_HANDLER