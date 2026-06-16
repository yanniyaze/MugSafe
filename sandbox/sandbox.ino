#include <Servo.h>

Servo myservo;

#define DEFAULT_PULSE_WIDTH 0;

void setup() {
  delay(1000);
  myservo.attach(9); 
  myservo.write(90);    // Stop/Neutral zuerst setze
}

void loop() {
  moveUp();

  delay(2000);

  //moveDown();
  myservo.detach();
  while (1) {}
}

void moveUp() {
  myservo.write(180);
  delay(250);
  myservo.write(90);
}

void moveDown() {
  myservo.write(0);
  delay(250);
  myservo.write(90);
}