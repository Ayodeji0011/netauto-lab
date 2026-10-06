#pragma once
#include "shared_data.h"
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

extern SemaphoreHandle_t dfPlayerMutex;

void initDFPlayer();
void playAudio(String audioName, bool repeat);
void stopAudio();
void checkAudioRepeat();