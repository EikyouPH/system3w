// Programme principal
// Il est le premier lancé et contient toutes les fonctions principales

// Importation d'une bibliothèque permettant d'interagir avec des périphériques SPI
#include <Arduino/SPI.h>
// Importation d'une bibliothèque permettant l'écriture sur la carte SD
#include <Arduino/SD.h>
// Importation de l'ensemble de nos fonctions secondaires dans le fichier fonctions.cpp
#include <fonctions.cpp>
// Importation de la librairie SoftwareSerial
#include <SoftwareSerial.h>
// Importation de la librairie RTC
#include <iarduino_RTC.h>


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
// Enum pour faciliter l'emploi des modes et des couleurs
enum Mode {Standard = 0, Eco, Maintenance, Config};
enum Couleur {Rouge = 0, Vert, Jaune, Bleu, Orange, Blanc};

// Intitialisation du programme
void setup() 
{
  // Definition du débit de communication
  Serial.begin(9600);
  Serial.println("Demarrage du programme");
  // Definintion du pin SD
  SD.begin(pinCS);
  // Definition des peripheriques (entrees, sorties)
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
int modeStandard()
{  
  // Initialisation des interruptions
  // Mesure et sauvegarde des capteurs avec vérification des erreurs 
  mesureCapteurs();
  sauvMesure();
  verifErreurs();
  return Mode;
}

// Fonction mode Eco
int modeEco()
{ 
  // Initialisation des interruptions
  // Mesure et sauvegarde des capteurs avec vérification des erreurs 
  mesureCapteurs();
  sauvMesure();
  checkErreur();
  return Mode;
}

// Fonction mode Maintenance
int modeMaintenance() 
{
  // Initialisation des interruptions
  // Mise à jour du clignottement des LEDs
  couleurLed(Orange)
  // Affichage du mode
  stopMesure();
  accesSD();
  affSerie();
  return Mode;
}

float sauvMesure()
{
  //Initialiser une variable qui compte le nombre de fichiers dans un dossier
    int NbFichiers = 0;

  //Ouvrir un dossier
    //Si le dossier contient 9 fichiers
      
        //Créer un nouveau dossier 
        //Réinitialiser la variable qui compte le nombre de fichiers dans un dossier

    //ouvrir le fichier
        //si le fichier s'ouvre correctement
            //écrire les mesures dans le fichier
            //incrémenter la variable qui compte le nombre de fichiers dans un dossier
            //fermer le fichier

        //sinon (le fichier ne s'ouvre pas correctement)
            //afficher un message d'erreur
}

float arrondi(float var)
{
  // Renvoie un flottant envoyé en entrée arrondi au centieme
}

struct MesureCapteurs
{
  float Heure[1];
  float GPS[2];
  float Capteurs[5];
} mesure;

// Rassemble les différentes mesures des capteurs et les renvoie sur forme de float


float Heure()
{
  // Renvoie l'heure de captation des mesures sous forme de float
}

// Fonction GPS, renvoie la latitude et la longitude sous forme de float
byte GPS(pinGPS)
{
  if(canal disponible)
  {
    // Récupération des données du GPS
    if (gps.location.isUpdated())
    {                                      // Si les données sont mises à jour
      float lat = (gps.location.lat(), 6); // On récupère la latitude
      float lng = (gps.location.lng(), 6); // On récupère la longitude
    }
    return lat, lng; // On renvoie la latitude et la longitude
  }
}

void mesureCapteurs(lat, lng)
{
  // Declaration des variables qui captent les différents paramètres
  float lux;
  float pression;
  float tempAir;
  float hygro;
  // Insertion des valeurs dans la structure MesureCapteurs
  struct MesureCapteurs mesure = {heure, lat, lng, lux, pression, tempAir, hygro};
}

// Gestion de la couleur de la LED
void couleurLed(Couleur)
{
  // Détermination des couleurs en variant les paramètres RVB de la DEL
  if (Couleur == Rouge)
  {
    // R = 255
  }
  else if (Couleur == Vert)
  {
    // V = 255
  }
  else if (Couleur == Jaune)
  {
    // R = 127
    // V = 127)
  }
  else if (Couleur == Bleu)
  {
    // B = 255
  }
  else if (Couleur == Orange)
  {
    // R = 255
    // V = 127
    // B = 127
  }
  // Blanc
  else
  {
    // R = 255
    // V = 255
    // B = 255
  }
}

// Définition de la fonction verifErreurs, qui vérifie toutes les erreurs possibles
void verifErreurs(lux, pression, tempAir, hygro, Heure)
{
  
  if (Heure == NULL)
  {
    // Vérification d’accès à l’horloge RTC
    // si pas d'accès à l'horloge RTC, alors Clignotement de la LED intermittent rouge et bleu
  }

  if (GPS == NULL)
  {
    // Vérification d’accès au GPS
    // si pas d'accès au GPS, alors Clignotement de la LED intermittent rouge et jaune
  }
  
  else if (lux || pression || tempAir || hygro == NULL)
  {
    // Vérification d’accès aux données des capteurs
    // Si pas de données des capteurs, alors clignotement de la LED intermittent rouge et vert
  }
  
  else if (lux > 10000)
  {
    // Vérification de la cohérence de la luminosité
    // Si luminosité incohérente, alors clignotement de la LED intermittent rouge et vert (durée 2x plus longue pour le vert)
  }
  
  else if (pression > 1000000)
  {
    // Vérification de la cohérence de la pression
    // Si pression incohérente, alors clignotement de la LED intermittent rouge et vert (durée 2x plus longue pour le vert)
  }
  
  else if (tempAir > 50)
  {
    // Vérification de la cohérence de la température de l'air
    //Si température de l'air incohérente, alors clignotement de la LED intermittent rouge et vert (durée 2x plus longue pour le vert)
  }
  
  else if (hygro > 100)
  {
    // Vérification de la cohérence de l'hygrométrie
    // Si hygronométrie incohérente, alors clignotement de la LED intermittent rouge et vert (durée 2x plus longue pour le vert)
  }

  else if(Erreur d'accès ou d'écriture carte SD){
    // Clignotement de la LED intermittent rouge et blanc (durée 2x plus longue pour le blanc)
  }

  else if(Carte SD pleine){
    // Clignotement de la LED intermittent rouge et blanc
  }

}

// Fonction appelée lors de l'appui sur le bouton rouge en mode standard
void appuiBoutonRougeS()
{
  // On passe en mode maintenance
  // On renvoie le mode
}

// Fonction appelée lors de l'appui sur le bouton rouge en mode standard
void appuiBoutonVertS()
{
  // On passe en mode économique
  // On renvoie le mode
}

// Fonction appelée lors de l'appui sur le bouton rouge en mode maintenance
void appuiBoutonRougeM()
{
  if (Mode == Eco)
  {
    // On repasse en mode économique
    // On renvoie le mode
  }
  else
  {
    //On repasse en mode standard
    // On renvoie le mode
  }
}

// Fonction appelée lors de l'appui sur le bouton rouge en mode éco
void appuiBoutonRougeE()
{
    // On passe en mode maintenance
    // On renvoie le mode
}

// Fonction appelée lors de l'appui sur le bouton rouge en mode éco
void appuiBoutonVertE()
{
  // On passe en mode standard
  // On renvoie le mode
}

// Ce mode permet de configurer le système grâce à une interaction depuis une console sur l’interface série
// Boucle de traitement du mode

void ModeConfig()
{
    if (Serial.available() > 0) // Si les données sont disponibles sur la liaison série
    {
        //Lit l'entrée de la console

        switch (c) 
        // Dans le cas où :
        {
        case 'LOG_INTERVALL':       //L'utilisateur entre 'LOG_INTERVALL' dans l'interface série :
            // Demander la valeur de LOG_INTERVALL à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable LOG_INTERVALL
            break;              //Sortir de la fonction
        case 'FILE_MAX_SIZE':       //L'utilisateur entre 'FILE_MAX_SIZE' dans l'interface série :
            // Demander la valeur de FILE_MAX_SIZE à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable FILE_MAX_SIZE
            break;
        case 'RESET':       //L'utilisateur entre 'RESET' dans l'interface série :
            // Demander la confirmation à l’utilisateur
            // Si la confirmation est positive
            // Remettre les valeurs par défaut des variables
            break;
        case 'VERSION':        //L'utilisateur entre 'VERSION' dans l'interface série :
            // Afficher la version du système
            break;
        case 'TIMEOUT':       //L'utilisateur entre 'TIMEOUT' dans l'interface série :
            // Demander la valeur de TIMEOUT à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable TIMEOUT
            break;
        case 'LUMIN':        //L'utilisateur entre 'LUMIN' dans l'interface série :
            // Demander la valeur de LUMIN à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable LUMIN
            break;
        case 'LUMIN_MIN':        //L'utilisateur entre 'LUMIN_MIN' dans l'interface série :
            // Demander la valeur de LUMIN_MIN à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable LUMIN_MIN
            break;
        case 'LUMIN_MAX':        //L'utilisateur entre 'LUMIN_MAX' dans l'interface série :
            // Demander la valeur de LUMIN_MAX à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable LUMIN_MAX
            break;
        case 'TEMP_AIR':        //L'utilisateur entre 'TEMP_AIR' dans l'interface série :
            // Demander la valeur de TEMP_AIR à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable TEMP_AIR
            break;
        case 'MIN_TEMP_AIR':        //L'utilisateur entre 'MIN_TEMP_AIR' dans l'interface série :
            // Demander la valeur de TEMP_AIR_MIN à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable TEMP_AIR_MIN
            break;
        case 'MAX_TEMP_AIR':        //L'utilisateur entre 'MAX_TEMP_AIR' dans l'interface série :
            // Demander la valeur de TEMP_AIR_MAX à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable TEMP_AIR_MAX
            break;
        case 'HYGR':        //L'utilisateur entre 'HYGR' dans l'interface série :
            // Demander la valeur de HYGR à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable HYGR
            break;
        case 'HYGR_MINT':        //L'utilisateur entre 'HYGR_MINT' dans l'interface série :
            // Demander la valeur de HYGR_MINT à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable HYGR_MINT
            break;
        case 'HYGR_MAXT':        //L'utilisateur entre 'HYGR_MAXT' dans l'interface série :
            // Demander la valeur de HYGR_MAXT à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable HYGR_MAXT
            break;
        case 'PRESSURE':        //L'utilisateur entre 'PRESSURE' dans l'interface série :
            // Demander la valeur de PRESSURE à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable PRESSURE
            break;
        case 'PRESSURE_MIN':        //L'utilisateur entre 'PRESSURE_MIN' dans l'interface série :
            // Demander la valeur de PRESSURE_MIN à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable PRESSURE_MIN
            break;
        case 'PRESSURE_MAX':        //L'utilisateur entre 'PRESSURE_MAX' dans l'interface série :
            // Demander la valeur de PRESSURE_MAX à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable PRESSURE_MAX
            break;
        case 'CLOCK':        //L'utilisateur entre 'CLOCK' dans l'interface série :
            // Demander la valeur de CLOCK à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable CLOCK
        case 'DATE':        //L'utilisateur entre 'DATE' dans l'interface série :
            // Demander la valeur de DATE à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable DATE
            break;
        case 'DAY':        //L'utilisateur entre 'DAY' dans l'interface série :
            // Demander la valeur de DAY à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable DAY
            break;
        case 'HELP':        //L'utilisateur entre 'HELP' dans l'interface série :
            // Afficher la liste des commandes
            break;
        default:        //L'utilisateur n'entre rien dans l'interface série :
            Serial.println("Commande inconnue");
            break;
        }
    }
}

// Boucle infinie
void loop() 
{
  Modes();
}
