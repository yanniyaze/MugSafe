#include <Arduino.h>

float duration_us, distance_cm;
const float cupMinDistance_cm = 5.0;

bool checkUltrasonicSensorState(int US_TRIG_PIN, int US_ECHO_PIN) {
  digitalWrite(US_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(US_ECHO_PIN, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  return distance_cm <= cupMinDistance_cm;
}