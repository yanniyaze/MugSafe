#include <DS18B20.h>

#include "libraries/temperatursensor/tempSens.cpp"

DS18B20 ds(2);
const float indexTemperature = 30.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  float messung = indexMessung(ds);
    if (indexTemperature * 1.5 < messung) {
      // TODO maybe einen Jingle einbauen?
      Serial.println(ds.getTempC());
      Serial.println("true");
    } else {
      // Fehlerzustand, mit allen weiteren Sachen...
      Serial.println(ds.getTempC());
      Serial.println("false");
    }
}
