#include <SPI.h>
#include <SD.h>

void setup() {
  Serial.begin(9600);
  Serial.println("Demarrage");
  const int delaiMesure = 1000;
  const int pinCS = 10;
  SD.begin(pinCS);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(10000);
}

void loop() {
  Serial.println("Mode standard");
  delay(1000);
  mesureCapteurs();
}