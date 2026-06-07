#include <Arduino.h>

#define RED_LED_PIN         2
#define GREEN_LED_PIN       4
#define BLUE_LED_PIN        6

#define DELAY_SHORT         100
#define DELAY_LONG          250


void chooseLight(int currentIndex) {
    switch (currentIndex) {
      case 0:
        digitalWrite(RED_LED_PIN, HIGH);
        delay(DELAY_LONG);
        digitalWrite(RED_LED_PIN, LOW);
        delay(DELAY_LONG);
        break;
      case 1:
        digitalWrite(RED_LED_PIN, HIGH);
        delay(DELAY_SHORT);
        digitalWrite(RED_LED_PIN, LOW);
        delay(DELAY_SHORT);
        break;
      case 2:
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, HIGH);
        delay(DELAY_LONG);
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
        delay(DELAY_LONG);
        break;
      case 3:
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, HIGH);
        delay(DELAY_SHORT);
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
        delay(DELAY_SHORT);
        break;
      case 4:
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(BLUE_LED_PIN, HIGH);
        delay(DELAY_LONG);
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(BLUE_LED_PIN, LOW);
        delay(DELAY_LONG);
        break;
      case 5:
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(BLUE_LED_PIN, HIGH);
        delay(DELAY_SHORT);
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(BLUE_LED_PIN, LOW);
        delay(DELAY_SHORT);
        break;
      default:
        break;
    }
}