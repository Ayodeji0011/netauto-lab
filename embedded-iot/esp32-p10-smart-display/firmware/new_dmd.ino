#include <Arduino.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <DMD32.h>
#include "fonts/SystemFont5x7.h"

// 3 panels wide, 2 panels tall (96x32)
#define DISPLAYS_ACROSS 3
#define DISPLAYS_DOWN   2
#define PIN_DMD_nOE     22

DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
hw_timer_t * timer = NULL;

void IRAM_ATTR triggerScan() {
  dmd.scanDisplayBySPI();
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);

  // Set OE Brightness
  analogWrite(PIN_DMD_nOE, 170);
  delay(500);

  // Hardware Timer Setup (Core 2.0.14)
  uint8_t cpuClock = ESP.getCpuFreqMHz();
  timer = timerBegin(0, cpuClock, true);
  timerAttachInterrupt(timer, &triggerScan, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
  delay(500);

  // Clear display completely
  dmd.clearScreen(true);
  delay(100);

  // Draw a border box around the entire 96x32 frame to verify outer bounds
  // (x1, y1, x2, y2)
  dmd.drawBox(0, 0, 95, 31, GRAPHICS_NORMAL);

  // Draw test text in the middle
  dmd.selectFont(SystemFont5x7);
  dmd.drawString(20, 12, "TESTING", 7, GRAPHICS_NORMAL);
}

void loop() {
  // Timer handles display scanning automatically
}
