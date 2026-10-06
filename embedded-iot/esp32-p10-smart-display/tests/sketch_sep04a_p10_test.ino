#include <Arduino.h>

#define DATA_PIN 23
#define CLK_PIN  18
#define LAT_PIN  2
#define OE_PIN   22
#define A_PIN    19
#define B_PIN    21

#define TOTAL_BITS 192

int litPos = 0;

void shiftFrame(int onPos)
{
  for (int i = 0; i < TOTAL_BITS; i++) {
    digitalWrite(DATA_PIN, (i == onPos) ? LOW : HIGH); // LOW = ON
    digitalWrite(CLK_PIN, HIGH);
    digitalWrite(CLK_PIN, LOW);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(LAT_PIN, OUTPUT);
  pinMode(OE_PIN, OUTPUT);
  pinMode(A_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  digitalWrite(CLK_PIN, LOW);
  digitalWrite(LAT_PIN, LOW);
  digitalWrite(OE_PIN, HIGH);
}

void loop()
{
  static int phase = 0;

  digitalWrite(OE_PIN, HIGH);
  digitalWrite(A_PIN, phase & 0x01);
  digitalWrite(B_PIN, (phase >> 1) & 0x01);

  shiftFrame(litPos);

  digitalWrite(LAT_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(LAT_PIN, LOW);
  digitalWrite(OE_PIN, LOW);

  delay(2);

  static int counter = 0;
  counter++;
  if (counter > 3000) {  // slowed down — hold each position much longer
    counter = 0;
    Serial.print("bit position: ");
    Serial.println(litPos);
    litPos++;
    if (litPos >= TOTAL_BITS) litPos = 0;
  }

  phase++;
  if (phase >= 4) phase = 0;
} 
