#include "HX711.h"
#include <SPI.h>
#include <SD.h>

// HX711 circuit wiring
const int8_t pins[] PROGMEM = {2, 3, 4, 5, 6, 7, 8, 9, 10}; // {LC1-dOut, LC1-SCK,...LC4-Sck,MicroSD Pin}

//HX711 scale;
HX711 LC1;
HX711 LC2;
HX711 LC3;
HX711 LC4;

//Start time
unsigned long t = 0;

File dataFile;

void setup() {
  Serial.begin(9600);
  LC1.begin(pins[0], pins[1]);
  LC2.begin(pins[2], pins[3]);
  LC3.begin(pins[4], pins[5]);
  LC4.begin(pins[6], pins[7]);
  LC1.tare();LC2.tare();LC3.tare();LC4.tare();
  LC1.set_scale(454);LC2.set_scale(454);LC3.set_scale(454);LC4.set_scale(459);
  if (!SD.begin(pins[8])) {
  Serial.println("Card failed, or not present");
  // don't do anything more:
  while (1);
  }
  Serial.println("card initialized.");
  SD.remove("LoadCell.csv"); //Remove old file
}

void loop() {
  if (LC1.is_ready()) {
   
    Serial.print(t); Serial.print(",");
    Serial.print(LC1.get_units(), 1); Serial.print(",");
    Serial.print(LC2.get_units(), 1); Serial.print(",");
    Serial.print(LC3.get_units(), 1); Serial.print(",");
    Serial.println(LC4.get_units(), 1);

    dataFile = SD.open("LoadCell.csv", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
    dataFile.print(t); dataFile.print(",");
    dataFile.print(LC1.get_units(), 1); dataFile.print(",");
    dataFile.print(LC2.get_units(), 1); dataFile.print(",");
    dataFile.print(LC3.get_units(), 1); dataFile.print(",");
    dataFile.println(LC4.get_units(), 1);
    dataFile.close();

    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("LoadCell.csv");
    }
    t = millis();
  } else {
    Serial.println("HX711 not found.");
  }
//
  delay(200);
  
}
