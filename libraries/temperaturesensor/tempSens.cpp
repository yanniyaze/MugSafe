#include <Arduino.h>

/*
	Hier wird einmalig gemessen, nachdem der Deckel heruntergefahren wurde, welche Temperatur der Sensor, nach einem Delay von 5 Sekunden, gemessen hat.
	Ausgegeben wird der Wert, der bestimmt, ob wir weiter gehen oder zurueck zum ersten Zustand.
*/
float indexMessung(DS18B20 ds) {
  delay(5000);
  return ds.getTempC();
}

/*
	Hier wird dauerhaft gemessen, ob der Temperatursensor die Wunschtemperatur erreicht hat. Das wird mit true ausgegeben, false wird nie erreicht,
	ausser der Temperatursensor misst fehlerhaft
*/
bool reachedPreferredTemperature(DS18B20 ds, float preferredTemperature) {
    while (ds.selectNext()) {
      delay(5000);
      if (ds.getTempC() <= preferredTemperature) {
        return true;
      }
    }
    return false;
  }

