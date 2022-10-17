// Programme principal
// Il est le premier lancé et contient toutes les fonctions principales

// Importation d'une bibliothèque permettant d'interagir avec des périphriques SPI
#include <Arduino/SPI.h>
// Importation d'une bibliothèque permettant l'écriture sur la carte SD
#include <Arduino/SD.h>
// Importation de l'ensemble de nos fonctions secondaires dans le fichier fonctions.cpp
#include <fonctions.cpp>
// Importation de la librairie SoftwareSerial
#include <SoftwareSerial.h>
// Importation de la librairie RTC
#include <iarduino_RTC.h>


// Initialisation du terminal série ss pour le GPS
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

// Intitialisation du programme
void setup() 
{
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
  
}

// Fonction permettant de basculer d'un mode à l'autre
void Modes(Mode) 
{
  // Mode Standard
  if(Mode == Standard)
  {
    modeStandard();
  }
  // Mode Eco
  else if(Mode == Eco)
  {
    modeEco();
  }
  // Mode Maintenance
  else if(Mode == Maintenance)
  {
    modeMaintenance();
  }
  // Mode Config
  else if(Mode == Config)
  {
    modeConfig();
  }
  // Gestion des erreurs : si pas de mode, renvoie 0
  return 0;
}

// Fonction du mode Standard
void modeStandard()
{  
  // Initialisation des interruptions
  attachinterrupt(digitalPinToInterrupt(boutonRouge), appuiBoutonRougeS, CHANGE);
  attachinterrupt(digitalPinToInterrupt(boutonVert), appuiBoutonVertS, CHANGE);
  // Mesure et sauvegarde des capteurs avec vérification des erreurs 
  mesureCapteurs();
  sauvMesure();
  verifErreurs();
  return Mode;
}

// Fonction mode Eco
void modeEco()
{ 
  // Initialisation des interruptions
  attachinterrupt(digitalPinToInterrupt(boutonRouge), appuiBoutonRougeE, CHANGE);
  attachinterrupt(digitalPinToInterrupt(boutonVert), appuiBoutonVertE, CHANGE);
  // Mesure et sauvegarde des capteurs avec vérification des erreurs 
  mesureCapteurs();
  sauvMesure();
  checkErreur();
  return Mode;
}

// Fonction mode Maintenance
void modeMaintenance() 
{
  // Initialisation des interruptions
  attachinterrupt(digitalPinToInterrupt(boutonRouge), appuiBoutonRougeM, CHANGE);
  // Mise à jour du clignottement des LEDs
  couleurLed(Orange)
  // Affichage du mode
  Serial.println("Mode Maintenance");
  stopMesure();
  accesSD();
  affSerie();
  return Mode;
}

void sauvMesure()
{
  //sauvegarder les mesures des capteurs dans un fichier avec le nom de la date et de l'heure
  
  //ouvrir le fichier
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  //si le fichier s'ouvre correctement
  if (dataFile) 
  {
    //sauvegarder les mesures dans le fichier
    dataFile.print("Temperature : ");
    dataFile.println(temperature);
    dataFile.print("Humidite : ");
    dataFile.println(humidite);
    dataFile.print("Luminosite : ");
    dataFile.println(luminosite);
    dataFile.print("Pression : ");
    dataFile.println(pression);
    dataFile.print("Altitude : ");
    dataFile.println(altitude);
    dataFile.print("Date : ");
    dataFile.println(date);
    dataFile.print("Heure : ");
    dataFile.println(heure);
    dataFile.print("Latitude : ");
    dataFile.println(latitude);
    dataFile.print("Longitude : ");
    dataFile.println(longitude);
    dataFile.print("Vitesse : ");
    dataFile.println(vitesse);
    dataFile.print("Satellites : ");
    dataFile.println(satellites);
    dataFile.print("Qualite : ");
    dataFile.println(qualite);
    dataFile.print("Erreur : ");
    dataFile.println(erreur);
    dataFile.println(" ");
    dataFile.close();
  }
  //si le fichier ne s'ouvre pas correctement
  else 
  {
    //afficher un message d'erreur
    Serial.println("erreur d'ouverture du fichier");
  }
  //Un dossier ne peut contenir que 9 fichiers
  //Si le dossier contient 9 fichiers, on en créée un nouveau
  if (dataFile == 9)
  {
    dataFile = SD.open("datalog2.txt", FILE_WRITE);
  }
}


void loop() 
{
  Modes();
}