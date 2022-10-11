#include <SPI.h>
#include <SD.h>
#include <fonctions.cpp>

const int delaiMesure = 20000;
const int pinCS = 10;

void setup() {
    Serial.begin(9600);
    Serial.println("Demarrage Eco");
    SD.begin(pinCS);
    pinMode(14, OUTPUT);
    digitalWrite(14, HIGH);
}

