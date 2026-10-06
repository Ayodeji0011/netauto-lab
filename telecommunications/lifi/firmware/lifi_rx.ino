#include <LiquidCrystal.h>

#define RX_SENSOR A0
// Receiver LCD Pinout: RS=7, E=6, D4=5, D5=4, D6=3, D7=8
LiquidCrystal lcd(7, 6, 5, 4, 3, 8);

const int BIT_DELAY = 60; // MUST match TX's BIT_DELAY exactly

// FIX: threshold is no longer a hardcoded guess. At startup, RX samples
// ambient/dark A0 level itself (assuming TX_LED is off during boot, which
// it is - TX sets it LOW in setup()) and derives the threshold from that.
// This adapts to YOUR room, YOUR sensor, YOUR distance - no manual tuning.
int darkBaseline = 0;
int onThreshold = 0;
int offThreshold = 0;
const int MARGIN = 150; // how far below dark-baseline counts as "light on"
const int HYSTERESIS = 40; // gap between on/off thresholds, kills chatter at the boundary

// FIX: average a few quick samples instead of trusting a single analogRead.
// Cuts electrical noise without adding meaningful delay (each analogRead
// is ~100us, this whole function takes under 1ms - negligible next to a
// 60ms bit window).
int readSensorAvg()
{
  long sum = 0;
  const int N = 5;
  for (int i = 0; i < N; i++) sum += analogRead(RX_SENSOR);
  return sum / N;
}

// state-holding version with hysteresis so noise near the threshold
// doesn't cause false bit flips
bool lastState = false;
bool isLightOn()
{
  int v = readSensorAvg();
  if (lastState) {
    // currently "on" - only flip to off if it rises above offThreshold
    lastState = (v < offThreshold);
  } else {
    // currently "off" - only flip to on if it drops below onThreshold
    lastState = (v < onThreshold);
  }
  return lastState;
}

void calibrate()
{
  lcd.setCursor(0, 1);
  lcd.print("Calibrating...");

  long sum = 0;
  const int N = 30;
  for (int i = 0; i < N; i++) {
    sum += analogRead(RX_SENSOR);
    delay(10);
  }
  darkBaseline = sum / N;
  onThreshold = darkBaseline - MARGIN;
  offThreshold = onThreshold + HYSTERESIS;

  Serial.println("--- Calibration ---");
  Serial.print("Dark baseline: "); Serial.println(darkBaseline);
  Serial.print("ON threshold:  "); Serial.println(onThreshold);
  Serial.print("OFF threshold: "); Serial.println(offThreshold);
  Serial.println("If TX LED-on readings don't drop clearly below ON threshold,");
  Serial.println("move the LED closer / point it directly at the sensor and reset.");
}

bool detectPreamble()
{
  if (!isLightOn()) return false;
  unsigned long start = millis();
  while (isLightOn()) {
    if (millis() - start > 250) return true;
  }
  return false;
}

void waitForPreambleEnd()
{
  while (isLightOn()) { /* wait for falling edge */ }
}

char receiveChar()
{
  unsigned long timeout = millis();
  while (!isLightOn()) {
    if (millis() - timeout > 1000) return (char)0xFF; // timeout marker
  }

  delay(BIT_DELAY + (BIT_DELAY / 2)); // skip start bit, center in first data bit

  char c = 0;
  for (int i = 0; i < 8; i++) {
    if (isLightOn()) bitSet(c, i);
    delay(BIT_DELAY);
  }

  delay(BIT_DELAY / 2); // ride out stop bit
  return c;
}

void setup()
{
  pinMode(RX_SENSOR, INPUT_PULLUP);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("LiFi Receiver");
  Serial.begin(9600);

  calibrate();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LiFi Receiver");
  lcd.setCursor(0, 1);
  lcd.print("Waiting Web Data");
}

void loop()
{
  // live monitor - useful if you ever need to re-check readings by hand
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 500) {
    Serial.print("A0 avg: ");
    Serial.println(readSensorAvg());
    lastPrint = millis();
  }

  if (detectPreamble())
  {
    waitForPreambleEnd();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Receiving Web:");
    lcd.setCursor(0, 1);

    String receivedMsg = "";

    for (int i = 0; i < 16; i++) {
      char c = receiveChar();
      if (c == 0x00 || c == (char)0xFF) break;

      receivedMsg += c;
      if (receivedMsg.length() <= 16) lcd.print(c);
    }

    if (receivedMsg.length() > 0) {
      Serial.print("Web Message Received: ");
      Serial.println(receivedMsg);
      delay(3000);
    } else {
      Serial.println("Preamble seen but no valid bytes decoded - check alignment/threshold.");
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LiFi Receiver");
    lcd.setCursor(0, 1);
    lcd.print("Waiting Web Data");
  }
}
