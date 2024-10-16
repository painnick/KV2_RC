#include <Arduino.h>
#include "esp_log.h"

#include "common.h"
#include "controllers/Mp3Controller.h"

#define MAIN_TAG "Main"

void setup() {
  ESP_LOGI(MAIN_TAG, "Setup...");

  setupSound();
  delay(1000);

  playOpening();
  delay(4000);
}

void loop() {
  dfmp3.loop();
}
