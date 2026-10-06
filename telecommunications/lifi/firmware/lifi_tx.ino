#include <LiquidCrystal.h>

#define TX_LED 3
// LCD Pin allocation: RS=12, E=11, D4=5, D5=4, D6=6, D7=7
LiquidCrystal lcd(12, 11, 5, 4, 6, 7);

const int BIT_DELAY = 60; // MUST match RX's BIT_DELAY exactly

void sendChar(char c)
{
  // Start bit
  digitalWrite(TX_LED, HIGH);
  delay(BIT_DELAY);

  for (int i = 0; i < 8; i++)
  {
    digitalWrite(TX_LED, bitRead(c, i));
    delay(BIT_DELAY);
  }

  // Stop bit
  digitalWrite(TX_LED, LOW);
  delay(BIT_DELAY);
}

void sendPreamble()
{
  digitalWrite(TX_LED, HIGH);
  delay(300);
  digitalWrite(TX_LED, LOW);
  delay(BIT_DELAY * 2);
}

void sendMessage(String message)
{
  sendPreamble();

  for (unsigned int i = 0; i < message.length(); i++)
  {
    sendChar(message[i]);
  }

  sendChar((char)0x00); // terminator
  digitalWrite(TX_LED, LOW);
}

void setup()
{
  Serial.begin(9600);
  pinMode(TX_LED, OUTPUT);
  digitalWrite(TX_LED, LOW);
  lcd.begin(16, 2);
  lcd.print("LiFi Ready...");
  delay(1000);

  lcd.clear();
  lcd.print("Waiting for Web");
  lcd.setCursor(0, 1);
  lcd.print("Input...");
}

void loop()
{
  if (Serial.available() > 0)
  {
    String inputMessage = Serial.readStringUntil('\n');
    inputMessage.trim();
    if (inputMessage.length() > 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sending:");
      lcd.setCursor(0, 1);
      lcd.print(inputMessage.length() > 16 ? inputMessage.substring(0, 16) : inputMessage);

      sendMessage(inputMessage);

      Serial.print("Transmitted: ");
      Serial.println(inputMessage);

      delay(1500);

      lcd.clear();
      lcd.print("Waiting for Web");
      lcd.setCursor(0, 1);
      lcd.print("Input...");
    }
  }
}
