#include <DS18B20.h>

#include "libraries/led/led.cpp"
#include "libraries/temperatursensor/simpleMessung.cpp"

DS18B20 ds(2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  messung(ds);
}
