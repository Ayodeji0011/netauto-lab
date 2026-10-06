#include "audio_tts.h"

#include <Arduino.h>
#include <sam_arduino.h>

static SAM sam;

void initAudioTTS() {

    Serial.println();
    Serial.println("========================================");
    Serial.println("INITIALIZING ESP32 OFFLINE TTS");
    Serial.println("========================================");

    /*
       SAM configuration.

       GPIO25 is the classic ESP32 internal DAC channel 1.
       The PAM amplifier receives the analog audio signal.
    */

    sam.setVolume(80);
    sam.setPitch(64);

    Serial.println("TTS engine ready");
    Serial.println("Audio output: GPIO25 DAC");
    Serial.println("========================================");
}

void speakText(const String &text) {

    if (text.length() == 0) {
        return;
    }

    String cleanText = text;

    cleanText.trim();

    if (cleanText.length() == 0) {
        return;
    }

    Serial.print("TTS: ");
    Serial.println(cleanText);

    sam.say(cleanText.c_str());
}
