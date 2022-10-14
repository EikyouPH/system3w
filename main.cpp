// Programme principal
// Il est le premier lancé et contient toutes les fonctions principales
#include <Arduino/SPI.h>
#include <Arduino/SD.h>
#include <fonctions.cpp>

SoftwareSerial ss(4, 3);
const int delaiMesure = 10000;
const int delaiMesureEco = 20000;
const int pinCS = 11;
const int boutonRouge = 5;
const int boutonVert = 6;
const int pinLux = A1;
enum Mode {Standard = 0, Eco, Maintenance};

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  Serial.println("Demarrage du programme");
  SD.begin(pinCS);
  pinMode(boutonRouge, INPUT);
  pinMode(boutonVert, INPUT);
  time.begin();
}

void modeStandard(){   
  mesureCapteurs();
  delay(1000);
  sauvMesure();
  delay(delaiMesure);
  checkErreur();
  if (boutonVert == HIGH){
    Mode = Eco;
  }
  else if (boutonRouge == HIGH){
    Mode = Maintenance;
  }
  else {
    Mode = Standard;
  }
  return mode;
}

void modeEco(){ 
  mesureCapteurs();
  delay(1000);
  sauvMesure();
  checkErreur();
  delay(delaiMesureEco);
  if (boutonVert == HIGH){
      Mode = Standard;
  }
  else if (boutonRouge == HIGH){
      Mode = Maintenance;
  }
  else {
      Mode = Eco;
  }
  return Mode;
}

void modeMaintenance() {
  Serial.println("Mode Maintenance");
  delay(1000);
  stopMesure();
  accesSD();
  affSerie();
  if (boutonRouge == HIGH){
    Mode = Standard;
  }
  else {
    Mode = Maintenance;
  }
  return Mode;
}
void loop() {
  
}