#include <SPI.h>
#include <DMD2.h>
#include <fonts/SystemFont5x7.h>

// Panel Matrix: 3 panels wide, 2 panels high (96x32 total resolution)
#define DISPLAYS_WIDE 3
#define DISPLAYS_HIGH 2

// ESP32 Hardware Pin Assignments
#define PIN_A    19
#define PIN_B    21
#define PIN_CLK  18
#define PIN_LAT  2
#define PIN_DATA 23
#define PIN_OE   22

// Initialize SoftDMD with your exact pin mappings
SoftDMD dmd(DISPLAYS_WIDE, DISPLAYS_HIGH, PIN_A, PIN_B, PIN_OE, PIN_LAT, PIN_CLK, PIN_DATA);

void setup() {
  dmd.begin();
  dmd.setBrightness(255); // Full brightness
  dmd.selectFont(SystemFont5x7);

  // Clear display
  dmd.clearScreen();

  // Draw "EMERGENCY" centered
  // SystemFont5x7 text width for "EMERGENCY" (9 chars * 6px - 1px = 53px)
  // Horizontal Center on 96px width: (96 - 53) / 2 = 21
  // Vertical Center on 32px height: (32 - 7) / 2 = 12
  dmd.drawString(21, 12, "EMERGENCY");
}

void loop() {
  // DMD2 handles background scan timing reliably inside dmd.begin()
}
