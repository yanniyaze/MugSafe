#include <Arduino.h>

Servo myservo;

#define DEFAULT_PULSE_WIDTH 0;

void moveUp(int SERVO_PIN){

delay(1000);
myservo.attach(SERVO_PIN);

myservo.write(180);
delay(700);

myservo.write(90);
myservo.detach();
}

void moveDown(int SERVO_PIN){ 

delay(1000);
myservo.attach(SERVO_PIN);

myservo.write(0);
delay(700);

myservo.write(90);
myservo.detach();
} 
