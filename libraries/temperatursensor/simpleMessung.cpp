#include <Arduino.h>

void messung(DS18B20 ds) {
  while (ds.selectNext()) {
    Serial.println(ds.getTempC());
  }
}