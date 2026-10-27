
#include <FreqCount.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long baseLine = 0;

const int LED = A2;
const int buzzer = A3;

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Metal Detector");
  lcd.setCursor(0, 1);
  lcd.print("Calibrating...");

  FreqCount.begin(100);

  while (!FreqCount.available()) {
    delay(10);
  }

  baseLine = FreqCount.read();

  Serial.print("Baseline frequency: ");
  Serial.println(baseLine);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");

  delay(1000);
  silence();
}

void loop() {
  if (!FreqCount.available()) {
    return;
  }

  unsigned long count = FreqCount.read();
  long difference = (long)count - (long)baseLine;

  Serial.print("Freq: ");
  Serial.print(count);
  Serial.print("  Diff: ");
  Serial.println(difference);

  lcd.setCursor(0, 0);

  if (difference < -10) {
    lcd.print("Ferrous");
    digitalWrite(LED, HIGH);
    digitalWrite(buzzer, HIGH);
  }
  else if (difference > 10) {
    lcd.print("Non-Ferrous");
    digitalWrite(LED, HIGH);
    digitalWrite(buzzer, HIGH);
  }
  else {
    lcd.print("No Metal");
    silence();
  }

  // Gradually adjust the baseline
  if (count > baseLine) {
    baseLine++;
  }
  else if (count < baseLine) {
    baseLine--;
  }
}

void silence() {
  digitalWrite(LED, LOW);
  digitalWrite(buzzer, LOW);
}
