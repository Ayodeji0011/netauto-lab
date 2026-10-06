#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

struct DisplayData {
  String line1;
  String line2;
};

extern volatile bool newMessageFlag;
extern DisplayData sharedDisplay;
extern SemaphoreHandle_t displayMutex;
