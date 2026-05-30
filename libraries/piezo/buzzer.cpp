#include <Arduino.h>

const int NOTE_A4 = 880;     
const int NOTE_B5 = 988;     
const int NOTE_B4 = 494;  
const int NOTE_G4 = 392;  
const int NOTE_C6 = 1047;
const int NOTE_G5 = 784;
const int NOTE_A5 = 880;

const int NOTE_F4 = 349;
const int NOTE_E4 = 330;
const int NOTE_D4 = 294;
const int NOTE_C4 = 262;
const int NOTE_B3 = 247;

const int shortDur = 120;
const int gap = 40;

void playPositive(int piezoPin) {
  tone(piezoPin, NOTE_A4, shortDur);
  delay(shortDur + gap);
  tone(piezoPin, NOTE_B5, shortDur);
  delay(shortDur + gap);
  noTone(piezoPin);
}

void playNegative(int piezoPin) {
  tone(piezoPin, NOTE_B4, shortDur);
  delay(shortDur + gap);
  tone(piezoPin, NOTE_G4, shortDur);
  delay(shortDur + gap);
  noTone(piezoPin);
}

void playConfirm(int piezoPin) {
  tone(piezoPin, NOTE_C6, 90);
  delay(120);
  noTone(piezoPin);
}

void playEndMelody(int piezoPin) {
  tone(piezoPin, NOTE_G5, 180);
  delay(220);
  tone(piezoPin, NOTE_B5, 180);
  delay(220);
  tone(piezoPin, NOTE_C6, 180);
  delay(220);
  tone(piezoPin, NOTE_B5, 180);
  delay(220);
  tone(piezoPin, NOTE_A5, 260);
  delay(320);
  noTone(piezoPin);

  delay(180);

  tone(piezoPin, NOTE_B5, 180);
  delay(220);
  tone(piezoPin, NOTE_C6, 180);
  delay(320);
  tone(piezoPin, NOTE_C6, 180);
  delay(320);
  tone(piezoPin, NOTE_C6, 180);
  delay(320);
  noTone(piezoPin);
}


void playSadMelody(int piezoPin) {
  tone(piezoPin, NOTE_G4, 220);
  delay(260);
  tone(piezoPin, NOTE_F4, 220);
  delay(260);
  tone(piezoPin, NOTE_E4, 260);
  delay(300);
  tone(piezoPin, NOTE_D4, 260); 
  delay(300);
  tone(piezoPin, NOTE_C4, 320); 
  delay(360);
  tone(piezoPin, NOTE_D4, 220); 
  delay(260);
  tone(piezoPin, NOTE_B3, 420);  
  delay(480);

  noTone(piezoPin);
}