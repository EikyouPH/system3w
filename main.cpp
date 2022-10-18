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
// Importation de la bibliothèque poru l'arrondi
#include <iostream>
#include <math.h>

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

  //interruption possible sur le bouton rouge pour passer en mode config
  Mode=Config;
  attachInterrupt(digitalPinToInterrupt(boutonRouge), modeConfig, CHANGE);
  delay(10000);
  Mode=Standard;
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
  //Initialiser une variable qui compte le nombre de fichiers dans un dossier
  int nbFichiers = 0;
  SdFile Fichier;
  

  //Ouvrir un dossier (le dossier ouvert est automatiquement créé si il n'existe pas)
  SD.mkdir("sys3w_relevé_mesures");

  //ouvrir un fichier dans repertoire "sys3w_relevé_mesures"
  
  char datafile[32];
  int jour=moment.day();
  int mois = moment.month();
  int annee= moment.year(); 
  sprintf(datafile,"sys3w_relevé_mesures/%d%d%d_%d.LOG",jour,mois,annee,nbFichiers);  //  %d pour un int
  if(fichier = SD.open(datafile, FILE_WRITE)){
    if(fichier.position()>FILE_MAX_SIZE){
      char datafile2[32];
      bool move=true;

      fichier.close();
      do(){
        nbFichiers++;
        if(nbFichier==9){
          Archivage();
          nbFichiers=0;
          move=false;
        }
        sprintf(datafile2,"sys3w_relevé_mesures/%d%d%d_%d.LOG",jour,mois,annee,nbFichiers);  //  %d pour un int

      }while(SD.exists(datafile2));
      
      //renommer le fichier
      if(move)
      {
        SDrename(datafile,datafile2);
      }

      //ouvrir à nouveau le fichier (maintenant vide)
      if(!fichier = SD.open(datafile, FILE_WRITE)){
        Serial.println("erreur ouverture fichier");
      }
    }
    // écrire les données dans le fichier
    dataFile.println(MesuresCapteurs);
    dataFile.close();
  }
  else{
    Serial.println("erreur ouverture fichier");
  }

     
     
     
     //Si le dossier contient 9 fichiers
      if (nbFichiers == 9){
        //Créer un nouveau dossier
        File monFichier = SD.open("/nouveauDossier");
        //Réinitialiser la variable qui compte le nombre de fichiers dans un dossier
        nbFichiers=0;
      }
      
      //
       
      
      //ouvrir le fichier
      File dataFile = SD.open(datafile, FILE_WRITE);
      //si le fichier s'ouvre correctement
      if (dataFile) {
        //écrire les mesures dans le fichier
        dataFile.println(MesuresCapteurs);
        //Quand un fichier est plein, le système crée une copie du fichier avec le numéro de révision adapté puis recommence à enregistrer les données au début du fichier (révision 0).
        //Si le fichier est plein
        if  (dataFile.size() == 512){
          //Créer une copie du fichier avec le numéro de révision adapté
          File dataFile = SD.open(datafile, FILE_WRITE);
          //Réinitialiser le fichier
          dataFile.close();
        }

      //fermer le dossier
      monFichier.close();
        
        //incrémenter la variable qui compte le nombre de fichiers dans un dossier
        nbFichiers++;
        //fermer le fichier
        dataFile.close();
      }
      else{
        //si le fichier ne s'ouvre pas correctement
        Serial.println("erreur d'ouverture du fichier");
      }

      


}

void SDrename(source, destination){
  SdFile ficsource;
  SdFile ficdestination;

  
  if(!ficsource = SD.open(source, FILE_READ)){
    Serial.println("erreur ouverture fichier source");
  }
  if(!ficdestination = SD.open(destination, FILE_WRITE)){
    Serial.println("erreur ouverture fichier destination");
  }
  while(data=ficsource.read() >= 0){
    ficdestination.write(data);
  }
  ficsource.close();
  ficdestination.close();
  ficsource.remove();

}

void Archivage() {

  /* Ouvre le premier fichier */
  repfile = SD.open("/sys3w_relevé_mesures");

  File entry = repfile.openNextFile();
  int a=0;

  while (entry) {
  
    if (entry.isDirectory()) {
      a++;
      
    } 
    entry = repfile.openNextFile();
    }
    repfile.close();

    sprintf(nomDoss,"sys3w_relevé_mesures/archives_%d", a);
    mkdir(nomDoss);
    repfile = SD.open("/sys3w_relevé_mesures");

//mettre dans ce nouveau doss
  File entry = repfile.openNextFile();
  while (entry) {
  
    if (!entry.isDirectory()) { 
      sprintf(nomFic,"sys3w_relevé_mesures/archives_%d/%s", a, entry.name());
      SDrename(entry.name(),nomFic);
    } 
    entry = repfile.openNextFile();
    }
    repfile.close();
}


void loop() 
{
  Modes();
}