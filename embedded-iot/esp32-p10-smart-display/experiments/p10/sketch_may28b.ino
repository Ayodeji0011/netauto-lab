#include <Arduino.h>

// Direct pin definitions (LAT on GPIO 2)
#define PIN_A    19
#define PIN_B    21
#define PIN_LAT  2   // Latch on GPIO 2
#define PIN_OE   22  // Output Enable on GPIO 22
#define PIN_CLK  18  // Clock on GPIO 18
#define PIN_DATA 23  // Data on GPIO 23

// 6 panels total = 384 shift bits per scan line
#define TOTAL_BITS 384

void setup() {
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_LAT, OUTPUT);
  pinMode(PIN_OE, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);

  // Turn Output Enable ON (Active LOW for HUB12 P10)
  digitalWrite(PIN_OE, LOW);
  digitalWrite(PIN_LAT, LOW);
  digitalWrite(PIN_CLK, LOW);
}

void loop() {
  // Cycle through all 4 multiplexed row groups
  for (int scan = 0; scan < 4; scan++) {
    
    // Select 1 of 4 scan lines (A and B pins)
    digitalWrite(PIN_A, (scan & 0x01) ? HIGH : LOW);
    digitalWrite(PIN_B, (scan & 0x02) ? HIGH : LOW);

    // Clock in HIGH data for all shift registers across the board
    digitalWrite(PIN_DATA, HIGH);
    for (int i = 0; i < TOTAL_BITS; i++) {
      digitalWrite(PIN_CLK, HIGH);
      delayMicroseconds(2);
      digitalWrite(PIN_CLK, LOW);
      delayMicroseconds(2);
    }

    // Pulse Latch HIGH to push shifted data to display outputs
    digitalWrite(PIN_LAT, HIGH);
    delayMicroseconds(5);
    digitalWrite(PIN_LAT, LOW);

    // Hold line illuminated briefly
    delayMicroseconds(800);
  }
}
