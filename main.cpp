// Programme principal
// Il est le premier lancé et contient toutes les fonctions principales

// Importation d'une bibliothèque permettant d'interagir avec des périphriques SPI
#include <Arduino/SPI.h>
// Importation d'une bibliothèque permettant l'écriture sur la carte SD
#include <Arduino/SD.h>
// Importation de l'ensmeble de nos fonctions secondaires dans le fichier fonctions.cpp
#include <fonctions.cpp>
//
#include <stdlib.h>
//
#include <SoftwareSerial.h>
//
#include <iarduino_RTC.h>


// 
SoftwareSerial ss(4, 3);

// Définition des variables globales
// Délai entre deux mesures en mode standard (à doubler dans le mode éco)
const int delaiMesure = 10000;
// Pin pour lier la carte SD
const int pinCS = 11;
// Pins pour les boutons
const int boutonRouge = 5;
const int boutonVert = 6;
// Pins pour les LEDs
const int ledOrange = 7;
const int ledVert = 8;
const int ledBleue = 9;
const int ledJaune = 10;
// Pins pour les capteurs
const int pinLux = A1;
enum Mode {Standard = 0, Eco, Maintenance};
enum couleur {Rouge = 0, Vert, Jaune, Bleu, Orange, Blanc};

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  Serial.println("Demarrage du programme");
  SD.begin(pinCS);
  pinMode(ledVert, OUTPUT);
  pinMode(ledBleue, OUTPUT);
  pinMode(ledJaune, OUTPUT);
  pinMode(ledOrange, OUTPUT);
  pinMode(boutonRouge, INPUT);
  pinMode(boutonVert, INPUT);
  time.begin();
}

void modeStandard(){
  couleurLed(Orange)
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
  digitalWrite(ledBleue, LOW);
  digitalWrite(ledOrange, HIGH);
  digitalWrite(ledVert, lOW);
  digitalWrite(ledJaune, LOW);  
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
  // Mise à jour du clignottement des LEDs
  couleurLed(Orange)
  // Affichage du mode
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