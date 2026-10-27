#include <Servo.h>

const int TOUCH_SENSOR_PIN = 10;
const int PIR_SENSOR_PIN = 8;
const int SERVO_PIN = 9;

Servo servo;

int lastTouchState;
int currentTouchState;

 

void setup() {
  Serial.begin(9600);
  pinMode(TOUCH_SENSOR_PIN, INPUT);
  pinMode(PIR_SENSOR_PIN, INPUT);
  servo.attach(SERVO_PIN);

  currentTouchState = digitalRead(TOUCH_SENSOR_PIN);
}

void loop() {

  if (digitalRead(PIR_SENSOR_PIN) == HIGH) {
    Serial.println("Motion detected! Pausing for 5 seconds...");
    delay(0);
    ;
  }

  lastTouchState = currentTouchState;
  currentTouchState = digitalRead(TOUCH_SENSOR_PIN);

  if (lastTouchState == LOW && currentTouchState == HIGH) {
    Serial.println("Touch detected");

 
    servo.write(0);
    delay(1000);    

    servo.write(90);
    delay(200);      

    servo.write(180);
    delay(1100);       

    servo.write(90);

    delay(4000);
  }

  servo.write(90);
}
