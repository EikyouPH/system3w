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

void Standard(){
  mesureCapteurs();
  sauvMesure();
  checkErreur();
  delay(delaiMesure);
  if (boutonVert == HIGH){
    Eco();
  }
  else if (BoutonRouge == HIGH){
    Maintenance();
  }
  else {
    Standard();
  }
}

void loop() {
  Serial.println("Mode standard");
  digitalWrite(ledBleue, LOW);
  digitalWrite(ledOrange, LOW);
  digitalWrite(ledVert, HIGH);
  digitalWrite(ledJaune, LOW);
  delay(1000);
}

void Eco() {
digitalWrite(ledBleue, HIGH);
digitalWrite(ledOrange, LOW);
digitalWrite(ledVert, lOW);
digitalWrite(ledJaune, LOW);
Serial.println("Mode Eco");
delay(1000);
mesureCapteurs();
delay(1000);
sauvMesure();
delay(delaiMesure * 2);
checkErreur();
if (boutonVert == HIGH){
  modeStandard();
}
else if (boutonRouge == HIGH){
  modeMaintenance();
}
else {
  modeEco();
}
    

void Maintenance() {
  // Mise à jour du clignottement des LEDs
  digitalWrite(ledBleue, LOW);
  digitalWrite(ledOrange, HIGH);
  digitalWrite(ledVert, lOW);
  digitalWrite(ledJaune, LOW);
  // Affichage du mode
  Serial.println("Mode Maintenance");
  // 
  accesSD();
  // 
  affSerie();
  if (boutonRouge == HIGH){
     Eco();
  }
  else if (boutonVert == HIGH){
    Standard();
  }
  else {
    Maintenance();
  }
}