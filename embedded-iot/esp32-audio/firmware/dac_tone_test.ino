#include <Arduino.h>

#define AUDIO_DAC 25

void setup() {
  Serial.begin(115200);

  Serial.println("ESP32 DAC AUDIO TEST");
  Serial.println("You should hear a tone...");
}

void loop() {

  // Generate approximately 1 kHz tone
  const int sampleRate = 8000;
  const int frequency = 1000;

  const int samplesPerCycle = sampleRate / frequency;

  for (int i = 0; i < samplesPerCycle; i++) {

    int sample;

    if (i < samplesPerCycle / 2) {
      sample = 220;
    } else {
      sample = 35;
    }

    dacWrite(AUDIO_DAC, sample);

    delayMicroseconds(1000000 / sampleRate);
  }
}
