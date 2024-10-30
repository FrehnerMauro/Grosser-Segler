#include <Wire.h>

#define SENSOR_ADDRESS 0x31  // I²C-Adresse des Sensors

void setup() {
  Serial.begin(9600);    // Starte serielle Kommunikation
  Wire.begin();           // I²C-Bus initialisieren

  delay(100);  // Sensor-Initialisierung abwarten
  Serial.println("TeraRanger Evo 60 verbunden, Messung gestartet...");
}

void loop() {
  int distance = readDistance();  // Distanzmessung durchführen
  if (distance >= 0) {
    Serial.print("Distanz: ");
    Serial.print(distance);
    Serial.println(" mm");
  } else {
    Serial.println("Fehler beim Lesen der Daten.");
  }

  delay(100);  // Pause zwischen Messungen
}

int readDistance() {
  Wire.beginTransmission(SENSOR_ADDRESS);  // Verbindung starten
  Wire.write(0x00);  // Trigger-Befehl senden
  Wire.endTransmission();  // Übertragung beenden

  delayMicroseconds(500);  // Wartezeit gemäß Handbuch

  // Lese 3 Bytes: 2 für den Wert, 1 für die CRC-Prüfsumme
  Wire.requestFrom(SENSOR_ADDRESS, 3);
  if (Wire.available() == 3) {
    int highByte = Wire.read();  // Erstes Datenbyte
    int lowByte = Wire.read();   // Zweites Datenbyte
    byte crc = Wire.read();      // CRC-Prüfsumme

    int distance = (highByte << 8) | lowByte;  // 16-Bit-Distanzwert

    // Optional: CRC-Prüfung (falls notwendig)
    // if (checkCRC(highByte, lowByte, crc)) { ... }

    return distance;
  } else {
    return -1;  // Fehler beim Lesen der Daten
  }
}

// Optionale Funktion zur CRC8-Prüfung (falls erforderlich)
bool checkCRC(int highByte, int lowByte, byte crc) {
  // CRC8-Prüfungslogik hier implementieren (optional)
  return true;  // Beispiel: Rückgabe "true" als Platzhalter
}
