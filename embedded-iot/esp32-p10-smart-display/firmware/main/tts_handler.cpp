#include "tts_handler.h"

#define DFPLAYER_RX   34
#define DFPLAYER_TX   13
#define DFPLAYER_VOL  27  // 0-30

HardwareSerial dfSerial(1);
DFRobotDFPlayerMini dfPlayer;

SemaphoreHandle_t dfPlayerMutex = NULL;

volatile bool audioRepeating = false;
int currentRepeatFile = -1;
long repeatIntervalMs = 5000; // gap between repeats in ms
long lastPlayTime = 0;

// =====================
// Audio name to file number mapping
// =====================
struct AudioMap {
  const char* name;
  int fileNumber;
};

AudioMap audioLibrary[] = {
  {"attention",    1},
  {"welcome",      2},
  {"classes",      3},
  {"assembly",     4},
  {"break",        5},
  {"endbreak",     6},
  {"closing",      7},
  {"announcement", 8},
  {"silence",      9},
  {"reminder",     10},
  {"staff",        11},
  {"visitors",     12},
  {"hall",         13},
  {"alert",        14},
  {"morning",      15},
  {"afternoon",    16},
  {"evening",      17},
  {"thankyou",     18},
  {"standby",      19},
  {"greatday",     20},
};
int audioLibrarySize = sizeof(audioLibrary) / sizeof(audioLibrary[0]);

// =====================
// Init DFPlayer
// =====================
void initDFPlayer() {
  dfPlayerMutex = xSemaphoreCreateMutex();

  dfSerial.begin(9600, SERIAL_8N1, DFPLAYER_RX, DFPLAYER_TX);
  delay(1000);

  Serial.println("Initializing DFPlayer...");

  if (!dfPlayer.begin(dfSerial, false, false)) {
    Serial.println("DFPlayer failed! Check:");
    Serial.println("- Wiring correct?");
    Serial.println("- SD card inserted?");
    Serial.println("- SD card formatted FAT32?");
    Serial.println("- MP3 folder exists?");
    return;
  }

  dfPlayer.volume(DFPLAYER_VOL);
  dfPlayer.EQ(DFPLAYER_EQ_NORMAL);
  Serial.println("DFPlayer initialized!");
  Serial.println("Files on SD: " + String(dfPlayer.readFileCounts()));
}

// =====================
// Get file number from name
// =====================
int getFileNumber(String audioName) {
  audioName.trim();
  audioName.toLowerCase();
  for (int i = 0; i < audioLibrarySize; i++) {
    if (audioName == String(audioLibrary[i].name)) {
      return audioLibrary[i].fileNumber;
    }
  }
  Serial.println("Audio not found: " + audioName);
  return -1;
}

// =====================
// Play audio by name
// =====================
void playAudio(String audioName, bool repeat) {
  Serial.println("playAudio: " + audioName + " repeat=" + String(repeat));

  if (dfPlayerMutex == NULL) return;
  if (audioName == "" || audioName == "none" || audioName == "null") {
    Serial.println("No audio specified");
    return;
  }

  int fileNumber = getFileNumber(audioName);
  if (fileNumber == -1) return;

  if (xSemaphoreTake(dfPlayerMutex, pdMS_TO_TICKS(1000))) {
    audioRepeating   = repeat;
    currentRepeatFile = repeat ? fileNumber : -1;
    lastPlayTime     = millis();

    dfPlayer.disableLoop();
    dfPlayer.playMp3Folder(fileNumber);
    xSemaphoreGive(dfPlayerMutex);
  }
}

// =====================
// Stop audio
// =====================
void stopAudio() {
  if (dfPlayerMutex == NULL) return;

  if (xSemaphoreTake(dfPlayerMutex, pdMS_TO_TICKS(1000))) {
    audioRepeating    = false;
    currentRepeatFile = -1;
    dfPlayer.stop();
    xSemaphoreGive(dfPlayerMutex);
  }
  Serial.println("Audio stopped");
}

// =====================
// Check and handle repeat — call from dedicated task
// =====================
void checkAudioRepeat() {
  if (dfPlayerMutex == NULL) return;
  if (!audioRepeating || currentRepeatFile == -1) return;

  if (millis() - lastPlayTime >= repeatIntervalMs) {
    if (xSemaphoreTake(dfPlayerMutex, pdMS_TO_TICKS(50))) {
      lastPlayTime = millis();
      dfPlayer.playMp3Folder(currentRepeatFile);
      xSemaphoreGive(dfPlayerMutex);
    }
    Serial.println("Audio repeated");
  }
}