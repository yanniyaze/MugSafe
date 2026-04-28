#include <Arduino.h>

const int BLUE = 12;
const int GREEN = 10;
const int RED = 8;

void led_rot_gruen_blau() {
  int state = RED;
    while (true) {
      switch (state) {
        case RED:
          digitalWrite(RED, LOW);
          digitalWrite(GREEN, HIGH);
          delay(500);
          state = GREEN;

          break;
        case BLUE:
          digitalWrite(BLUE, LOW);
          digitalWrite(RED, HIGH);
          delay(500);
          state = RED;
          break;
        case GREEN:
          digitalWrite(GREEN, LOW);
          digitalWrite(BLUE, HIGH);
          delay(500);
          state = BLUE;
          break;
    }
  }
}