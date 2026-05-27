#include <DS18B20.h>

#include "libraries/temperatursensor/tempSens.cpp"

// Temperatursensor
DS18B20 ds(2);

// Bestaetigungsbutton
const byte confirmButton = 3;

// Die Zustaende unserer State-Machine
enum STATES {
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
STATES currentState = TEMP_PREF;
// Unsere Vergleichstemperatur im State CAP_HEIGHT_CHECK_TEMP
const float indexTemperature = 30.0;
// Die Wunschtemperatur, die vom User im State TEMP_PREF eingestellt wird
float preferredTemperature = indexTemperature; 

void setup() {
  Serial.begin(9600);
}

void loop() {
  // State-Machine
  switch (currentState) {
      case TEMP_PREF:
        /* 
        Hier kommt die Buttonlogik zur Temperaturregelung rein (tempPref nutz)
        Zusaetzlich muss hier auch die Matrix-Anzeige rein
        */

        currentState = CAP_HEIGHT_MGMT; // der naechste State
        break;
      
      case CAP_HEIGHT_MGMT:
        /*
        Hier muss ueberprueft werden, ob der Deckel des Geraetes nah genug an der Tasse dran ist, das passiert mit dem Ultraschallsensor
        Wenn das nicht wirklich klappt, muessen wir es ueber den Temperatursensor machen.                                                                                                               
        */

        currentState = CAP_HEIGHT_CHECK_TEMP;
        break;
      
      case CAP_HEIGHT_CHECK_TEMP:
        /*
        Hier wird ermittelt, ob ein grosser Temperaturunterschied zu einer globalen Variable gemessen wurde. Wenn ja, dann erfolgreich weitermachen, wenn nein, dann wieder zum ersten State
        BeispielCode siehe unten
        */
        float messung = indexMessung(ds);
        if (indexTemperature * 1.5 < messung) {
          // TODO maybe einen Jingle einbauen?
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
           currentState = SUCCESS_OR_NOT;
           break;
        } else {
          // Dieser Fall kann nur eintreten, wenn der Sensor keine Temperatur mehr misst. Daher Fehlerzustand
          currentState = TEMP_WAIT;
          break;
        }
      
      case SUCCESS_OR_NOT:
        /*
        Hier wird auf den User Input gewartet, mittels Interrupt. Dabei wird fuer 5 Minuten jede Minute ein Jingle gespielt,
        sodass der User auf das Entfernen der Tasse aufmerksam gemacht wird. Wenn dieser nicht den Button drueckt, machen wir nen traurigen Jingle.
        */

        unsigned long startTime = millis(); // aktueller Zeitpunkt, wenn fertig gemessen wurde
        unsigned long lastMinuteAction = startTime;

        bool positive = false;
        while (millis() - startTime < 300000UL) {  // 5 Minuten
          if (digitalRead(confirmButton) == HIGH) {
            positive = true;
            break;
          }

          if (millis() - lastMinuteAction >= 60000UL) {  // jede Minute
            lastMinuteAction += 60000UL;  // stabiler als = millis()
            // Jingle spielen, damit User informiert wird
          }
        }

        if (positive) {
          /*
          Hier wurde der Button gedrueckt und es wird ein positiver Jingle gespielt + Matrix
          */
        } else {
          /*
          Hier sind die fuenf Minuten abgelaufen und es wird ein negativer Jingle und entsprechend eine Matrix angezeigt
          */
        }

        currentState = TEMP_PREF;
        break;
    }

}

  