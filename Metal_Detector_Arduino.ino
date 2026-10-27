#include <FreqCount.h>

// Baseline frequency of the search coil
unsigned long baseLine = 0;

// LED pins
const int bluePin  = A2;
const int greenPin = A4;
const int redPin   = A5;
const int buzzer   = A1;


void setup() {
  Serial.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Startup LED test
  digitalWrite(redPin, HIGH);   delay(200); silence();
  digitalWrite(greenPin, HIGH); delay(200); silence();
  digitalWrite(bluePin, HIGH);  delay(200); silence();

  // Start frequency counting
  // IMPORTANT: Uses Arduino D5 internally
  FreqCount.begin(100); // 100 ms gate time

  while (!FreqCount.available()) {
    delay(10);
  }

  baseLine = FreqCount.read();

  Serial.print("Baseline frequency: ");
  Serial.println(baseLine);

  if (baseLine > 10000) {
    digitalWrite(greenPin, HIGH); // OK
  } else {
    digitalWrite(redPin, HIGH);   // No oscillation / too low
  }

  delay(1000);
  silence();
}

void loop() {
  if (!FreqCount.available()) {
    return;
  }

  unsigned long count = FreqCount.read();
  long difference = abs((long)baseLine - (long)count);

  Serial.print("Freq: ");
  Serial.print(count);
  Serial.print("  Diff: ");
  Serial.println(difference);

  if (difference > 30) {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    digitalWrite(buzzer, HIGH);
  }
  else if (difference > 20) {
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, LOW);
  }
  else if (difference > 10) {
    digitalWrite(bluePin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
  }
  else {
    silence();
  }

  // Slow baseline tracking (prevents drift)
  if (count > baseLine) {
    baseLine++;
  } else if (count < baseLine) {
    baseLine--;
  }
}

void silence() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
  digitalWrite(buzzer, LOW);
}