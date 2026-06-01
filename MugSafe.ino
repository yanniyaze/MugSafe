#include <DS18B20.h>

#include "libraries/temperaturesensor/tempSens.cpp"
#include "libraries/piezo/buzzer.cpp"

#define UP_BUTTON_PIN       6
#define DOWN_BUTTON_PIN     4
#define CONFIRM_BUTTON_PIN  3
#define PIEZO_PIN           11
#define TEMP_SENSOR_PIN     2

// Temperatursensor
DS18B20 ds(TEMP_SENSOR_PIN);

// Die Zustaende unserer State-Machine
enum STATE {
  // TEMPERATURSTEUERUNG
  TEMP_PREF,

  // DECKELSTEUERUNG
  CAP_HEIGHT_MGMT,
  CAP_HEIGHT_CHECK_TEMP,

  // TEMPERATURMESSUNG
  TEMP_WAIT,

  // WARTE-ENDZUSTAND
  SUCCESS_OR_NOT
};

// Unser Initial-Zustand
STATE currentState = SUCCESS_OR_NOT;
// Unsere Vergleichstemperatur im State CAP_HEIGHT_CHECK_TEMP
const float indexTemperature = 30.0;
// Die Wunschtemperatur, die vom User im State TEMP_PREF eingestellt wird
float preferredTemperature = indexTemperature; 
float messung = 0;

// aktueller Zeitpunkt, wenn fertig gemessen wurde
unsigned long startTime = 0; 
unsigned long lastMinuteAction = 0;

bool positive = false;

void setup() {
  Serial.begin(9600);

  pinMode(PIEZO_PIN, OUTPUT);

  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  Serial.println("Am I Here?");

  // State-Machine
  switch (currentState) {
      case TEMP_PREF:
        Serial.println("State: TEMP_PREF");

        /* 
        Hier kommt die Buttonlogik zur Temperaturregelung rein (preferredTemperature nutzen)
        Zusaetzlich muss hier auch die Matrix-Anzeige rein
        */

        while (true) {
          byte upButtonState = digitalRead(UP_BUTTON_PIN);
          byte downButtonState = digitalRead(DOWN_BUTTON_PIN);
          byte confirmButtonState = digitalRead(CONFIRM_BUTTON_PIN);

          if (upButtonState == LOW) {
            playPositive(PIEZO_PIN);
            preferredTemperature += 5;
            // MATRIX
          }

          if (downButtonState == LOW) {
            playNegative(PIEZO_PIN);
            preferredTemperature -= 5;
            // MATRIX
          }

          if (confirmButtonState == LOW) {
            playConfirm(PIEZO_PIN);
            // MATRIX 

            currentState = CAP_HEIGHT_MGMT; // der naechste State
            break;
          }
        }

        break;
      case CAP_HEIGHT_MGMT:
        /*
        Hier muss ueberprueft werden, ob der Deckel des Geraetes nah genug an der Tasse dran ist, das passiert mit dem Ultraschallsensor
        Wenn das nicht wirklich klappt, muessen wir es ueber den Temperatursensor machen.                                                                                                               
        */
        Serial.println("State: CAP_HEIGHT_MGMT");
        while (true) {
          // nop
        }

        currentState = CAP_HEIGHT_CHECK_TEMP;
        break;
      
      case CAP_HEIGHT_CHECK_TEMP:
        /*
          Hier wird ermittelt, ob ein grosser Temperaturunterschied zu einer globalen Variable gemessen wurde. Wenn ja, dann erfolgreich weitermachen, wenn nein, dann wieder zum ersten State
          BeispielCode siehe unten
        */

        messung = indexMessung(ds);
        if (indexTemperature * 1.5 < messung) {
          playPositive(PIEZO_PIN);
          currentState = TEMP_WAIT;
          break;
        } else {
          // Fehlerzustand, mit allen weiteren Sachen...
          currentState = TEMP_PREF;
          break;
        }
      
      case TEMP_WAIT:
        /*
        Hier wird mittels Polling gemessen, ob unsere Wunschtemperatur erreicht wurde. Die Logik ist in tempSens.cpp implementiert.
        BeispielCode siehe unten
        */
        if (reachedPreferredTemperature(ds, preferredTemperature)) {
          // positiven Jingle spielen und etwas auf Matrix abbilden
          playEndMelody(PIEZO_PIN);
          currentState = SUCCESS_OR_NOT;
          break;
        } else {
          // Dieser Fall kann nur eintreten, wenn der Sensor keine Temperatur mehr misst. Daher Fehlerzustand
          playSadMelody(PIEZO_PIN);
          currentState = TEMP_WAIT;
          break;
        }
      
      case SUCCESS_OR_NOT:
        /*
        Hier wird auf den User Input gewartet, mittels Interrupt. Dabei wird fuer 5 Minuten jede Minute ein Jingle gespielt,
        sodass der User auf das Entfernen der Tasse aufmerksam gemacht wird. Wenn dieser nicht den Button drueckt, machen wir nen traurigen Jingle.
        */
        Serial.println("State: SUCCESS_OR_NOT");

        startTime = millis(); // aktueller Zeitpunkt, wenn fertig gemessen wurde
        lastMinuteAction = startTime;

        while (millis() - startTime < 300000UL) {  // 5 Minuten
          if (digitalRead(CONFIRM_BUTTON_PIN) == LOW) {
            playPositive(PIEZO_PIN);
            delay(100);
            positive = true;
            break;
          }

          if (millis() - lastMinuteAction >= 60000UL) {  // jede Minute
            lastMinuteAction += 60000UL;  // stabiler als = millis()
            playEndMelody(PIEZO_PIN);
          }
        }

        if (positive) {
          /*
          Hier wurde der Button gedrueckt und es wird ein positiver Jingle gespielt + Matrix
          */
          playEndMelody(PIEZO_PIN);
        } else {
          /*
          Hier sind die fuenf Minuten abgelaufen und es wird ein negativer Jingle und entsprechend eine Matrix angezeigt
          */
          playSadMelody(PIEZO_PIN);
        }

        currentState = TEMP_PREF;
        break;
    }

  delay(1000);
}

  