#include <DS18B20.h>
#include <Servo.h>

#include "libraries/temperaturesensor/tempSens.cpp"
#include "libraries/piezo/buzzer.cpp"
#include "libraries/led/led.cpp"
#include "libraries/ultrasonic/ultrasonic.cpp"
#include "libraries/servo/servo.cpp"

#define UP_BUTTON_PIN       12
#define DOWN_BUTTON_PIN     10
#define CONFIRM_BUTTON_PIN  13
#define PIEZO_PIN           8
#define TEMP_SENSOR_PIN     9
#define SERVO_PIN           3
#define US_TRIG_PIN         7
#define US_ECHO_PIN         5

#define RED_LED_PIN         2
#define GREEN_LED_PIN       4
#define BLUE_LED_PIN        6


// Temperatursensor
DS18B20 ds(TEMP_SENSOR_PIN);

// Die Zustaende unserer State-Machine
enum STATE {
  // ZUSTAND ZUM ZURÜCKSETZEN
  PRE_INIT,

  // ULTRASCHALLSENSOR
  ULTRA_SOUND_CHECK,

  // TEMPERATURSTEUERUNG
  TEMP_PREF,

  // DECKELSTEUERUNG
  CAP_HEIGHT_MGMT,

  // TEMPERATURMESSUNG
  TEMP_WAIT,

  // WARTE-ENDZUSTAND
  SUCCESS_OR_NOT
};

// Unser Initial-Zustand
STATE currentState = PRE_INIT;

// Unsere Flag, ob eine Tasse hineingestellt wurde, oder nicht.
bool isCupThere = false;

// Unser Array mit allen erlaubten Temperaturwünschen, mit Index
const float possibleTemperatures[6] = {50, 55, 60, 65, 70, 75};
int possibleTemperatureIndex = 0;

// Unsere Vergleichstemperatur im State CAP_HEIGHT_MGMT
const float indexTemperature = 30.0;

// Die Wunschtemperatur, die vom User im State TEMP_PREF eingestellt wird, initial ist der erste/kleinste Wert
float preferredTemperature = possibleTemperatures[0]; 

// Initiale Startzeitpunkte für den Endzustand
unsigned long startTime = 0; 
unsigned long lastMinuteAction = 0;

// Flag, ob die Tasse nach Ablauf herausgenommen wurde, oder nicht
bool hasTakenCupOut = false;

void setup() {
  Serial.begin(9600);

  // Buzzer
  pinMode(PIEZO_PIN, OUTPUT);

  // Buttons
  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_BUTTON_PIN, INPUT_PULLUP);

  // LEDs
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  // UltraschallSensor
  pinMode(US_TRIG_PIN, OUTPUT);
  pinMode(US_ECHO_PIN, INPUT);

}

void loop() {

  // State-Machine
  switch (currentState) {
      case PRE_INIT:
        /*
          Der State, der alle globalen Variablen zurücksetzt.
        */
        preferredTemperature = possibleTemperatures[0]; 

        startTime = 0; 
        lastMinuteAction = 0;

        hasTakenCupOut = false;

        currentState = ULTRA_SOUND_CHECK;
        break;
      
      case ULTRA_SOUND_CHECK:
        /*
          Hier wird mittels Ultraschallsensor geprüft, ob eine Tasse abgestellt wurde, oder nicht. Die Logik ist in ultrasonic.cpp implementiert
        */

        while (true) {
          byte confirmButtonState = digitalRead(CONFIRM_BUTTON_PIN);
          if (confirmButtonState == LOW) {
            isCupThere = checkUltrasonicSensorState(US_TRIG_PIN, US_ECHO_PIN);
            break;
          }
        }

        if (isCupThere) {
          playPositive(PIEZO_PIN);
          currentState = TEMP_PREF;
        } else {
          playNegative(PIEZO_PIN);
          currentState = PRE_INIT;
        }

        break;
      case TEMP_PREF:
        Serial.println("State: TEMP_PREF");

        /* 
          Hier kommt die Buttonlogik zur Temperaturregelung rein (preferredTemperature nutzen). Die Logik ist in led.cpp implementiert.
        */

        while (true) {
          byte upButtonState = digitalRead(UP_BUTTON_PIN);
          byte downButtonState = digitalRead(DOWN_BUTTON_PIN);
          byte confirmButtonState = digitalRead(CONFIRM_BUTTON_PIN);

          if (upButtonState == LOW && preferredTemperature != possibleTemperatures[5]) {
            playPositive(PIEZO_PIN);
            preferredTemperature = possibleTemperatures[possibleTemperatureIndex + 1];
            possibleTemperatureIndex++;
          } else if (upButtonState == LOW && preferredTemperature == possibleTemperatures[5]) {
            playError(PIEZO_PIN);
          }

          if (downButtonState == LOW && preferredTemperature != possibleTemperatures[0]) {
            playNegative(PIEZO_PIN);
            preferredTemperature = possibleTemperatures[possibleTemperatureIndex - 1];
            possibleTemperatureIndex--;
          } else if (downButtonState == LOW && preferredTemperature == possibleTemperatures[0]) {
            playError(PIEZO_PIN);
          }

          if (confirmButtonState == LOW) {
            playConfirm(PIEZO_PIN);

            currentState = CAP_HEIGHT_MGMT; // der naechste State
            break;
          }
          chooseLight(possibleTemperatureIndex);
        }

        break;
      case CAP_HEIGHT_MGMT:

        //hier wird nach dem confirm die Funktion aufgerufen welche den Deckel inkl. Thermometer nach unten zur Tasse fährt 
        moveDown(SERVO_PIN);

        currentState = TEMP_WAIT;
        break;

      case TEMP_WAIT:
        /*
          Hier wird mittels Polling gemessen, ob unsere Wunschtemperatur erreicht wurde. Die Logik ist in tempSens.cpp implementiert.
          BeispielCode siehe unten
        */

        delay(35000);

        while (true) {
        
          if (reachedPreferredTemperature(ds, preferredTemperature)) {
            // positiven Jingle spielen
            playEndMelody(PIEZO_PIN);
            currentState = SUCCESS_OR_NOT;
            break;
            }
        
          //else {
            //Dieser Fall kann nur eintreten, wenn der Sensor keine Temperatur mehr misst. Daher Fehlerzustand
            //playSadMelody(PIEZO_PIN);
            //currentState = PRE_INIT;
           //break;
        //}
        }
      case SUCCESS_OR_NOT:
        /*
          Hier wird auf den User Input gewartet, mittels Interrupt. Dabei wird fuer 3 Minuten jede 15s ein Jingle gespielt,
          sodass der User auf das Entfernen der Tasse aufmerksam gemacht wird. Wenn dieser nicht den Button drueckt, machen wir nen traurigen Jingle.
        */
        Serial.println("State: SUCCESS_OR_NOT");

        startTime = millis(); // aktueller Zeitpunkt, wenn fertig gemessen wurde
        lastMinuteAction = startTime;
        playEndMelody(PIEZO_PIN);

        while (millis() - startTime < 180000UL) {  // 3 Minuten
          if (digitalRead(CONFIRM_BUTTON_PIN) == LOW) {
            playPositive(PIEZO_PIN);
            delay(100);
            hasTakenCupOut = true;
            break;
          }

          if (millis() - lastMinuteAction >= 15000UL) {  // jede 15 Sekunden
            lastMinuteAction += 15000UL;  // stabiler als = millis()
            playEndMelody(PIEZO_PIN);
          }
        }

        if (hasTakenCupOut) {
          /*
          Hier wurde der Button gedrueckt und es wird ein positiver Jingle gespielt
          */
          moveUp(SERVO_PIN);
          delay(500);
          playEndMelody3(PIEZO_PIN);
        } else {
          /*
          Hier sind die drei Minuten abgelaufen und es wird ein negativer Jingle
          */
          moveUp(SERVO_PIN);
          delay(5000);
          playSadMelody(PIEZO_PIN);
        }

        currentState = PRE_INIT;
        break;
    }

  delay(1000);
}
