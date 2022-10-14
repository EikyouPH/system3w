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
const int pinRouge = 9;
const int pinVert = 10;
const int pinBleu = 11;
// Pins pour les capteurs
const int pinLux = A1;
enum Mode {Standard = 0, Eco, Maintenance, Config};
enum Couleur {Rouge = 0, Vert, Jaune, Bleu, Orange, Blanc};

//
void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  Serial.println("Demarrage du programme");
  SD.begin(pinCS);
  pinMode(boutonRouge, INPUT);
  pinMode(boutonVert, INPUT);
  pinMode(pinRouge, OUTPUT);
  pinMode(pinVert, OUTPUT);
  pinMode(pinBleu, OUTPUT);
  time.begin();
  attachinterrupt(digitalPinToInterrupt(boutonRouge), appuiBoutonRouge, FALLING);
}

void appuiBoutonRouge(){
  Couleur = Rouge;
  Mode = Maintenance;
}

// Fonction permettant de basculer d'un mode à l'autre
void Modes(Mode) {
  // Mode Standard
  if(Mode == Standard){
    modeStandard();
  }
  // Mode Eco
  else if(Mode == Eco){
    modeEco();
  }
  // Mode Maintenance
  else if(Mode == Maintenance){
    modeMaintenance();
  }
  // Mode Config
  else if(Mode == Config){
    modeConfig();
  }
  // Gestion des erreurs : si pas de mode, renvoie 0
  return 0;
}

// Fonction du mode Standard
void modeStandard(){  
  // Mesure et sauvegarde des capteurs avec vérification des erreurs 
  mesureCapteurs();
  sauvMesure();
  checkErreur();
  // Mise à jour de la variable Mode en fonction des boutons
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

// Fonction mode Eco
void modeEco(){ 
  // Mesure et sauvegarde des capteurs avec vérification des erreurs 
  mesureCapteurs();
  sauvMesure();
  checkErreur();
  // Mise à jour de la variable Mode en fonction des boutons
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

// Fonction mode Maintenance
void modeMaintenance() {
  // Mise à jour du clignottement des LEDs
  couleurLed(Orange)
  // Affichage du mode
  Serial.println("Mode Maintenance");
  stopMesure();
  accesSD();
  affSerie();
  // Mise à jour de la variable Mode en fonction des boutons
  if (boutonRouge == HIGH){
    Mode = Standard;
  }
  else {
    Mode = Maintenance;
  }
  return Mode;
}

void loop() {
  Modes();
}