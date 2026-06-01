#include "libraries/piezo/buzzer.cpp"

#define UP_BUTTON_PIN 6
#define DOWN_BUTTON_PIN 4
#define CONFIRM_BUTTON_PIN 2
#define PIEZO 11

void setup() {
  pinMode(PIEZO, OUTPUT);
  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  byte upButtonState = digitalRead(UP_BUTTON_PIN);
  byte downButtonState = digitalRead(DOWN_BUTTON_PIN);
  byte confirmButtonState = digitalRead(CONFIRM_BUTTON_PIN);


  if (upButtonState == LOW) {
    playPositive(PIEZO);
  }

  if (downButtonState == LOW) {
    playNegative(PIEZO);
  }

  if (confirmButtonState == LOW) {
    playConfirm(PIEZO);
  }

  
}
