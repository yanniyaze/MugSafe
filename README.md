# MugSafe ☕

*Die sichere Variante Heißgetränke zu genießen...*

Dies ist das Projekt der Gruppe 7 bestehend aus:
 - Patrick Radke
 - Vanessa Folte
 - Julius Voigt
 - Yannik Lehnhardt

**MugSafe** ist ein Projekt, welches mithilfe unseres Prototypen sicherstellen soll, dass Personen sich an frisch gebrühten Heißgetränken nicht verbrennen. Dies wird mit einer interaktiven Wunschtemperatureinstellung und einem Temperatursensor umgesetzt, der solange die Temperatur des Heißgetränkes misst, bis die gewählte Temperatur erreicht ist. Ausgestattet mit LEDs und einem Buzzer, soll die **emotionale Komponente des Systems** dem User näher gebracht werden. 

## Struktur
Die Hauptdatei `MugSafe.ino`enthält die wichtigsten Kernelemente unseres Prototypen. Dabei werden aus dem Ordner `libraries`, die wir selbst geschrieben haben, alle nötigen Funktionen zum Ansprechen der jeweiligen Sensoren und Aktoren importiert. Diese sind wie folgt:

 - `/led` - enthält eine Funktion, die mittels der ausgewählten Temperatur bestimmte LEDs leuchten lässt.
 - `/piezo` - enthält Funktionen, die je nach State und Knopfdruck bestimmte Melodien über den Buzzer spielt.
 - `/servo` - enthält zwei Funktionen, die den Servormotor so bewegen, dass sich der Deckel nach oben / unten bewegt.
 - `/temperaturesensor` - enthält Funktionen für das Messen der Flüssigkeit in einer Tasse.
 - `/ultrasonic` - enthält eine Funktion, die erkennt, ob eine Tasse in den Prototyp gestellt wurde, oder nicht.

Im Groben wurde das System als eine State-Machine implementiert, welches sich an das UML-Diagramm in `abgaben/uml_activity_diagram.png`orientiert.

## Funktionen
- Automatische Erkennung, ob eine Tasse vorhanden ist (via Ultraschallsensor)
- Interaktive **Temperaturanzeige** mithilfe von LEDs und Knöpfen
- Einem kontinuierlich messendem **Temperatursensor**
- Einem linear verlaufenden Deckel mittels Servo-Motor
- Einem Buzzer, der entsprechend dem State der Zustands-Maschine **Töne und Melodien** abgibt.

## Testing

Um diesen Code selbstständig auszuführen, bedarf es eine kleine Vorbereitung. Für das Kompilieren in der Arduino-IDE müssen die Bibliotheken:
 - [**DS18B20.h**](https://github.com/matmunk/DS18B20)  (Library für den Temperatursensor) und 
 - [**OneWire.h**](https://www.pjrc.com/teensy/td_libs_OneWire.html) (Dependency für *DS18B20.h*)

im `/Arduino`-Order existieren.
Dafür im *Library Manager der IDE* die jeweiligen Libraries suchen und lokal installieren.


