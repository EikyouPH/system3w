/*include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>

#include <RTClib.h>


RTC_DS1307 rtc;*/
// Importation de la bibliothèque permettant de gerer la LED
#include <ChainableLED.h>
// Importation de la bibliothèque permettant d'utiliser le capteur d'humidite et de temperature
#include "DHT.h"
// Importation des bibliothèques permettant d'utiliser la carte SD
#include "SPI.h"
#include "SD.h"
#include <EEPROM.h> // Pour la sauvegarde des paramètres
//#include <TinyGPS++.h>
//#include <SoftwareSerial.h>
String valeurEnterString; // variable pour stocker la valeur entree par l’utilisateur
int valeurEnterInt; 
int ligne = 0;
// static const uint32_t GPSBaud = 9600;
// Declaration du type et du pin utilises pour les capteurs d'humidite et de temperature
#define DHTTYPE DHT11
#define DHTPIN 8
DHT dht(DHTPIN, DHTTYPE);

// Pin pour le capteur de luminosite
const int pinLux = A1;
// Pins pour les boutons
const int boutonRouge = 2;
const int boutonVert = 3;
// Pin pour la carte SD
const int pinCS = 4;
// Pins pour la LED (pin6, pin7, nombre de LED)
ChainableLED leds(6, 7, 1);

// Booleens pour controler la pression ou non du bouton, true = non presse
bool bRouge = true;
bool bVert = true;

// Variable stockant le mode precedent pour le mode maintenance
int precMode;

// Definition du delai entre deux mesure (1000 = 1 seconde)

int LOG_INTERVAL = EEPROM.get(0, LOG_INTERVAL) * 1000;

int logInterval = EEPROM.get(0, logInterval); // On stocke la valeur de logInterval dans l'EEPROM

int file_max_size = EEPROM.get(2, file_max_size); // On stocke la valeur de file_max_size dans l'EEPROM

int timeout = EEPROM.get(4, timeout); // On stocke la valeur de timeout dans l'EEPROM

int lumin = EEPROM.get(6, lumin); // On stocke la valeur de lumin dans l'EEPROM

int lumin_min = EEPROM.get(8, lumin_min); // On stocke la valeur de lumin_min dans l'EEPROM

int lumin_max = EEPROM.get(10, lumin_max); // On stocke la valeur de lumin_max dans l'EEPROM

int temp_air = EEPROM.get(12, temp_air); // On stocke la valeur de temp_air dans l'EEPROM

int min_temp_air = EEPROM.get(14, min_temp_air); // On stocke la valeur de min_temp_air dans l'EEPROM

int max_temp_air = EEPROM.get(16, max_temp_air); // On stocke la valeur de max_temp_air dans l'EEPROM

int hygr = EEPROM.get(18, hygr); // On stocke la valeur de hygr dans l'EEPROM

int hygr_mint = EEPROM.get(20, hygr_mint); // On stocke la valeur de hygr_mint dans l'EEPROM

int hygr_maxt = EEPROM.get(22, hygr_maxt); // On stocke la valeur de hygr_maxt dans l'EEPROM

int pressure = EEPROM.get(24, pressure); // On stocke la valeur de pressure dans l'EEPROM

int pressure_min = EEPROM.get(26, pressure_min); // On stocke la valeur de pressure_min dans l'EEPROM

int pressure_max = EEPROM.get(28, pressure_max); // On stocke la valeur de pressure_max dans l'EEPROM

int heure = EEPROM.get(30, heure); // On stocke la valeur de heure dans l'EEPROM

int minute = EEPROM.get(32, minute); // On stocke la valeur de minute dans l'EEPROM

int seconde = EEPROM.get(34, seconde); // On stocke la valeur de seconde dans l'EEPROM

int jour = EEPROM.get(36, jour); // On stocke la valeur de jour dans l'EEPROM

int mois = EEPROM.get(38, mois); // On stocke la valeur de mois dans l'EEPROM

int annee = EEPROM.get(40, annee); // On stocke la valeur de annee dans l'EEPROM

// Variable stockant le temps ecoule depuis le debut du programme à l'aide de millis()
long duree, delai;

// Variables contenant les valeurs obtenues des capteurs
int lumiere, temperature, humidite, pression;
// Enumeration des modes principaux
enum mode {Standard = 0, Eco, Maintenance, Config, Debut};
//
mode Mode;
/*
// 
//#define RX 2 // Affectation des broches pour la liaison série logicielle
//#define TX 3 // de l'Arduino/
//SoftwareSerial GPS(RX, TX); // Création de l'objet GPS pour la liaison série
                            // entre l'Arduino et le module GPS
byte recu; // Variable pour le stockage des données recues du module
File monFichier;*/


// Intitialisation du programme
void setup() 
{
  // Initialisation du mode à "Debut" : dans aucun des modes principaux
  Mode = Debut;
  // Definition du debit de donnees
  Serial.begin(9600);
  Serial.println("Demarrage du programme");
  // Initialisation des pins
  pinMode(boutonRouge, INPUT);
  pinMode(boutonVert, INPUT);
  pinMode(pinLux, INPUT);
  SD.begin(pinCS);
  // 
  Wire.begin();
  dht.begin();
  // Initialisation des interruptions (changement de position d'un des deux boutons)
  attachInterrupt(digitalPinToInterrupt(boutonRouge), appuiRouge, CHANGE);
  attachInterrupt(digitalPinToInterrupt(boutonVert), appuiVert, CHANGE);

  //interruption possible sur le bouton rouge pour passer en mode config
  attendre(5000);
  // Si pas d'interruption pour passer au mode config, passage au mode standard
  if(Mode == Debut){
    Mode = Standard;
  }
  //GPS.begin(9600);    // initialisation de la liaison série du GPS pour reception données
  //rtc.begin();
  // Attente de la connection serie avec l'Arduino
  while (!Serial);
  // Lance le communication I2C avec le module RTC et 
  // attend que la connection soit operationelle
/*  while (! rtc.begin()) {
    Serial.println("Attente du module RTC...");
    delay(500);
  }/*
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println("Horloge du module RTC mise a jour");*/
}


// Fonction d'interruption en cas de changement de position du bouton rouge
void appuiRouge(){
  // Lecture de la position du bouton rouge
  bRouge = digitalRead(boutonRouge);
  // Si le bouton est appuye (on vient de le presser), on stocke le temps ecoule depuis le debut du programme
  if(!bRouge){
    duree = millis();
  }

///////////////////////////////////////////////////////////////////////////////////////////////////

  // Si le bouton n'est pas appuye (on vient de le lâcher), on teste s'il a ete appuye pendant au moins 5 secondes
  else if(bRouge){
    if(millis()-duree > 5000){
      // Si l'on est au debut du programme, on passe Mode à Config
      if(Mode == Debut){
        Mode = Config;
      }
      // Si on est dans le mode Standard ou Eco, on passe au mode Maintenance après avoir stocke le precedent mode
      else if(Mode == Standard || Mode == Eco){
        precMode = Mode;
        Mode = Maintenance;
      }
      // Si on etait dans le mode Maintenance, on repasse au mode precedent (Standard ou Eco)
      else if(Mode == Maintenance){
        if(precMode == Eco){
          Mode = Eco;
        }
        else{
          Mode = Standard;
        }
      }
    }
  }
}


// De la même manière que appuiRouge, fonction d'interruption en cas de changement du bouton vert
void appuiVert(){
  // Lecture de la position du bouton vert
  bVert = digitalRead(boutonVert);
  // Si le bouton est appuye (on vient de le presser), on stocke le temps ecoule depuis le debut du programme
  if(!bVert){
    duree = millis();
  }
  // Si le bouton n'est pas appuye (on vient de le lâcher), on teste s'il a ete appuye pendant au moins 5 secondes
  else{
    if(millis()-duree > 5000){
      // Si on est en mode Standard, on passe ne mode Eco
      if(Mode == Standard){
        Mode = Eco;
      }
      // Si on est en mode Eco, on passe en mode Standard
      else if(Mode == Eco){
        Mode = Standard;
      }
    }
  }
}




// Fonction permettant de basculer d'un mode à l'autre
void Modes(){
  switch(Mode){
     // Mode Standard
    case 0:
      modeStandard();
      break;
    // Mode Eco
    case 1: 
      modeEco();
      break;
    // Mode Maintenance
    case 2: 
      modeMaintenance();
      break;
    // Mode Config
    /*case 3:
      modeConfig();
    break;*/
  }
}

void attendre(int timer){
  delai = millis();
  while(millis()-delai < timer){}
}

void loop(){
  Modes();
}

void modeStandard(){
  attendre(LOG_INTERVAL);
  Serial.println("Mode standard");
  attendre(10);
  leds.setColorRGB(0, 0, 255, 0);
  lumiere = mesureLumiere();
  humidite = mesureHumidite();
  temperature = mesureTemperature();
  pression = mesurePression();
  //mesureGPS();
  //renvoieDate();
  erreur(lumiere, humidite, temperature, pression);
  sauvMesures(lumiere, humidite, temperature, pression);
}

void modeEco(){
  if(millis()-delai >= LOG_INTERVAL * 2){
    delai = millis();
    Serial.println("Eco");
    attendre(10);
    leds.setColorRGB(0, 0, 0, 255);
    lumiere = mesureLumiere();
    humidite = mesureHumidite();
    temperature = mesureTemperature();
    pression = mesurePression();
    //mesureGPS();
    //renvoieDate();
    erreur(lumiere, humidite, temperature, pression);
    sauvMesures(lumiere, humidite, temperature, pression);
  }
}

void modeMaintenance(){
  Serial.println("Maintenance");
  attendre(10);
  leds.setColorRGB(0, 255, 127, 0);
  lumiere = mesureLumiere();
  humidite = mesureHumidite();
  temperature = mesureTemperature();
  pression = mesurePression();
  erreur(lumiere, humidite, temperature, pression);
  affichage(lumiere, humidite, temperature, pression);
  attendre(100);

}

void affichage(int lumiere, int humidite, int temperature, int pression){
  Serial.print(F("Niveau luminosite : "));
  Serial.print(lumiere);
  Serial.println(" lux");
  Serial.print(F("Taux d'humidite : "));
  Serial.print(humidite);
  Serial.println(F("%"));
  Serial.print(F("Temperature : "));
  Serial.print(temperature);
  Serial.println(F("*C"));
  Serial.print(F("Pression  : "));
  Serial.print(pression);
  Serial.println(F(" Pa"));
}


int mesureLumiere()
{
  int lumiere = analogRead(pinLux);
  return lumiere;
}

float mesureHumidite(){
  float temp_hum_val[2] = {0};
  dht.readTempAndHumidity(temp_hum_val);
  float humidite = temp_hum_val[0];
  return humidite;
}

float mesureTemperature(){
  float temp_hum_val[2] = {0};
  dht.readTempAndHumidity(temp_hum_val);
  float temperature = temp_hum_val[1];
  return temperature;
}

float mesurePression(){
  float pression = 8.3144621 * (273.15 + temperature) * masseVolumique(temperature) / 0.029
  ;
  return pression;
}

float masseVolumique(int temperature){
   if(temperature < 7){
     return 1.292;
   }
    else if(7 <= temperature < 17){
     return 1.225 ;
   }
    else if(17 <= temperature < 22){
     return 1.204 ;
   }
    else if(22 <= temperature){
     return 1.292 ;
   }
 }

void erreur(int lumiere, int humidite, int temperature, int pression){
  // horloge 0
  // GPS 1
  // capteur inaccessible 2
  //donnees incoherentes 3
  if(lumiere < lumin_min || lumiere > lumin_max /*|| humidite < hygr_mint || humidite > hygr_maxt || temperature < min_temp_air || temperature > max_temp_air || pression < pressure_min || pression > pressure_max*/){
    clignotement(3);
  }
  // carte sd pleine 4
  // carte sd inaccessible 5
  if(!SD.begin(pinCS)){
    clignotement(5);
  }
}

void clignotement(int type){
  while(true){
    switch(type)
    {
      case 0: // horloge
        leds.setColorRGB(0, 255, 0, 0);
        attendre(1000);
        leds.setColorRGB(0, 0, 0, 255);
        attendre(1000);
        break;
      case 1: // GPS
              leds.setColorRGB(0, 255, 0, 0);
        attendre(1000);
        leds.setColorRGB(0, 255, 255, 0);
        attendre(1000);
        break;
      case 2: // acces capteur
        leds.setColorRGB(0, 255, 0, 0);
        attendre(1000);
        leds.setColorRGB(0, 0, 255, 0);
        attendre(1000);
        break;
      case 3: // donnee incoherente
        leds.setColorRGB(0, 255, 0, 0);
        attendre(1000);
        leds.setColorRGB(0, 0, 255, 0);
        attendre(2000);
        break;
      case 4: // SD pleine
        leds.setColorRGB(0, 255, 0, 0);
        attendre(1000);
        leds.setColorRGB(0, 255, 255, 255);
        attendre(1000);
        break;
      case 5: // SD inaccessible
        leds.setColorRGB(0, 255, 0, 0);
        attendre(1000);
        leds.setColorRGB(0, 255, 255, 255);
        attendre(2000);
        break;
    }
  }
}

/*String renvoieDate ()
{
    DateTime now = rtc.now();
    char heure[10];
    int annee= now.year();
    String Sanneevar = String(annee);
    String Sannee = Sanneevar.substring(2);
    int mois= now.month();
    String Smois = String(mois);
    int jour= now.second();
    String Sday = String(jour);
    String NomFichiers= Sannee+Smois+Sday;
    return NomFichiers;
}
*/
/*void mesureGPS()
// Boucle de lecture des données
  {
   if (GPS.available() > 0 )  {
    recu = GPS.read();  // Lecture de la trame envoyée par le module GPS
    Serial.write(recu); // Affichage dans le moniteur série
    }
}*/


// Fonction pour sauvegarder les mesures dans la carte SD

void sauvMesures(float mes1, float mes2,float mes3, float mes4)
{
  //Initialiser une variable qui compte le nombre de fichiers dans un dossier
  int nbFichiers = 0;
  int FILE_MAX_SIZE = 4096;
/*  DateTime now = rtc.now();*/

// Si le dossier à créer n'existe pas, on le crée
// Test de la création du dossier

  if (!SD.exists("sys3w")) {
    Serial.println (F("Creation dossier"));
   if(!SD.mkdir("sys3w")){
      Serial.println(F("Erreur creation dossier"));
  }   
  } 
  
  
  Serial.println (F("Dossier cree "));

  //ouvrir un fichier dans repertoire "sys3w_releve_mesures"

  //Determiner le chemin pour arriver à notre dossier et modifier son nom
  char datafile[33];
  int jour =1;
  int mois = 10;
  int annee = 22;
  
  sprintf(datafile,"sys3w/%d%d%d_%d.LOG",annee,mois,jour,nbFichiers);  //  %d pour un int
  // Le tableau datafile contient le chemin complet et le nom du fichier à créer

  Serial.print(F("Ouverture "));
  Serial.println(datafile);

  // La variable fichier va ouvrir notre fichier en écriture

  File fichier = SD.open(datafile, FILE_WRITE);
  //Si le fichier s'ouvre
  if(fichier)
  {
    Serial.println (F("Fichier ouvert"));
    //Si le fichier est plein
    if(fichier.size()>FILE_MAX_SIZE)
    {
      Serial.println (F("Fichier plein"));
      char datafile2[33];
      bool move=true;

      // Fermer le fichier
      fichier.close();

      // Tant que le fichier datafile2 existe
      do
      {
        Serial.println(nbFichiers);
        // On incrémente le nombre de fichiers
        nbFichiers++;

        // Si le nombre de fichier dans le dossier est de 10 (compte de 0 à 9)
        if(nbFichiers==9){

          Serial.println (F("Dossier plein"));

          // On crée un nouveau dossier (archivage)
          Archivage();

          // Le nombre de fichiers est donc réinitialisé
          nbFichiers=0;
          move=false;
        }

        // Mettre dans datafile2 le chemin complet et le nom du fichier à créer
        sprintf(datafile2,"sys3w/%d%d%d_%d.LOG",annee,mois,jour,nbFichiers);  //  %d pour un int
        

        Serial.println(datafile2);
        
      }while(SD.exists(datafile2));
      
      
      if(move)
      {
        Serial.println(F("move"));
        //Copie et renomme le fichier
        SDrename(datafile,datafile2);
      }

      //ouvrir à nouveau le fichier (maintenant vide)
      fichier = SD.open(datafile, FILE_WRITE);

      // Test de l'ouverture du fichier
      if(!fichier){
        Serial.println (F("erreur d'ouverture fichier"));
      }
      Serial.println(datafile);
    }
    // ecrire les données des mesures des capteurs dans le fichier
    fichier.print(lumiere); 
    fichier.print(",");
    
    fichier.print(humidite);
    fichier.print(",");
    
    fichier.print(temperature);
    fichier.print(",");
    
    fichier.print(pression);
    fichier.println(".");
    
    // Une fois que la session de données est écrite sur le fichier, on ferme le fichier
    fichier.close();
    Serial.println (F("Releve des capteurs ecrits sur la carte SD"));
    Serial.print("Mesure n*");
    Serial.println(ligne);
    ligne++;    
    
  }
  else
  {
    Serial.println (F("erreur ouverture fichier"));
  }
}

// Fonction pour copier un fichier dans un autre et renommer le nouveau fichier en fonction des autres fichiers existants
void SDrename(char* source,char* destination){
  
  // Déclaration des variables pour la fonction rename, qui sont des pointeurs vers les adresses des caractères du nom du fichier à 'copier'
  File ficsource;
  File ficdestination;
  Serial.println(F("SDrename"));
  
  // Ouverture du fichier source
  ficsource = SD.open(source, FILE_READ);
  Serial.println(source);
  
  // Test si le fichier source s'ouvre bien
  if(!ficsource)
  {
    Serial.println (F("erreur ouverture fichier source"));
  }
  
  // Si le fichier source s'ouvre bien, ouvrir le fichier destination
  ficdestination = SD.open(destination, FILE_WRITE);
  Serial.println(destination);
  
  // Test si le fichier destination s'ouvre bien
  if(!ficdestination)
  {
    Serial.println (F("erreur ouverture fichier destination"));
  }


  /*
  // Alternative (qui lit des plus grosses quantités de données pour que la copie se fasse plus rapidement )
  size_t data;
  uint8_t buf[64];
  */

  // Déclarer la variable qui va contenir les données lues dans le fichier source
  long data;
  
  // Tant que le fichier source n'est pas vide, lire les données dans le fichier source et les écrire dans le fichier destination
  while(data = ficsource.read() >= 0){ // ou (data= ficsource.read(buf,sizeof(buf))) >= 0
    Serial.println();
    ficdestination.write(data); // ou avec buf,

  }
  
  Serial.println(F("data copiee"));
  
  // Quand la copie est terminée, fermer les deux fichiers (source et destination)
  ficsource.close();
  ficdestination.close();
  SD.remove(source);
}


//fonction pour archiver les fichiers déjà existants, dès que le dossier est plein, en créé un nouveau et mets les fichiers pleins dedans

void Archivage() {

  // Ouvre le premier dossier
  File repfile = SD.open("/sys3w/");

  //Parcours le dossier
  File entry = repfile.openNextFile();
  
  int a=0;
  
  String en = String(entry);
  Serial.println(en);
  Serial.println(F("Parcours dossiers rep"));

  // Tant qu'il y a un fichier dans le dossier
  while (entry) {
    Serial.println(entry.name());
    
    // Si le dossier est un dossier
    if (entry.isDirectory()) {
      a++;
      Serial.println(F("rep"));     
    } 
    entry = repfile.openNextFile();
  }

  //Ferme le fichier
  repfile.close();
  
  //Création du dossier d'archivage
  char nomDoss[17];
  sprintf(nomDoss,"sys3w/arch_%d", a);
  // Le nouveau dossier aura le nom "arch_a" avec a le nombre de dossier déjà existant
  Serial.println(nomDoss);
  
  // Test de la création du nouveau dossier
  Serial.println(F("Creation nouveau dossier")); 
  if(!SD.mkdir(nomDoss)){
      Serial.println(F("Erreur creation dossier"));
  }
  Serial.println(F("doss cree"));

  //Parcours le dossier pour déplacer les fichiers
  File repfile2 = SD.open("/sys3w/");

  entry = repfile2.openNextFile();
  
  String rep = String(repfile2);
  Serial.println(rep);
  //mettre dans ce nouveau doss

  en = String(entry);
  Serial.println(en);
  
  Serial.println(F("Parcours fichiers rep"));
  *
  // Tant qu'il y a un fichier dans le dossier
  while (entry) {
    Serial.println(entry.name());
    
    // Si le dossier est un dossier
    if (entry.isDirectory()) {
    Serial.println(F("rep"));
    } else { 
      
      //Création du nom du fichier
      Serial.println(F("fic"));
      char nomFic[50];
      sprintf(nomFic,"sys3w/arch_%d/%s", a, entry.name());
      
      // Utilise la fonction SDrename pour déplacer les fichiers
      SDrename(entry.name(),nomFic);
      Serial.println(nomFic);
      
    } 


    entry = repfile2.openNextFile();
  }
    repfile2.close();
  Serial.println(F("fin Parcours fichiers rep"));
  
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void modeConfig()
{
    Serial.println(F("Mode config")); // Affichage d’un message de bienvenue
    leds.setColorRGB(0, 255, 255, 0);    // initialisation des variables ?
    Serial.println(F("Voulez-vous initialiser les variables à leurs valeurs pas defaut ? (O/N)")); // Demande à l’utilisateur s’il veut initialiser les variables à leurs valeurs par defaut
    String InitialisationParametre = ReadandTrimString();                                          // On appelle la fonction ReadandTrimString
    if (InitialisationParametre == "O")                                                            // Si la valeur est O
    {
        IntialisationsVar();                        // On appelle la fonction IntialisationsVar
        Serial.println(F("initialisation faites")); // On informe l’utilisateur que l’initialisation est faite
    }
    else if (InitialisationParametre == "N") // Si la valeur est N
    {
        Serial.println(F("initialisation annulee")); // On informe l’utilisateur que l’initialisation est annulee
    }
    else // Si la valeur est autre chose que O ou N
    {
        Serial.print(F("ERREUR : Valeur incorrecte -> ")); // On affiche un message d’erreur
        Serial.println(InitialisationParametre);           // On affiche la valeur entree
        Serial.println(F("Veuillez entrer O ou N"));       // On affiche un message d’information
        modeConfig();                                           // On relance le mode
    }

    // Boucle de traitement des commandes
    Serial.println(F("Entrez une commande"));   // Affichage d’un message d’information
    String commandeEnter = ReadandTrimString(); // Lecture de la commande entree par l’utilisateur
    if (commandeEnter == "LOG_INTERVAL")        // Si la commande est LOG_INTERVAL
    {
        Serial.println(F("LOG_INTERVAL"));                                 // On affiche LOG_INTERVAL
        Serial.println(F("Rentrez la nouvelle valeur pour LOG_INTERVAL")); // On demande la nouvelle valeur pour LOG_INTERVAL
        ReadandConvert();                                                  // On appelle la fonction ReadandConvert
        if (valeurEnterInt > 0)                                            // Si la valeur est superieure à 0
        {
            Serial.print(F("LOG_INTERVAL mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a ete mise à jour
            EEPROM.put(0, valeurEnterInt);                           // On met à jour la valeur dans l’EEPROM
            Serial.println(EEPROM.get(0, valeurEnterInt));           // On affiche la nouvelle valeur
            modeConfig();                                                 // On relance le mode
        }
        else // Si la valeur est inferieure ou egale à 0
        {
            Serial.print(F("ERREUR : Valeur incorrecte ")); // On informe l’utilisateur que la valeur est incorrecte
            Serial.print(valeurEnterInt);
            Serial.println(F(" doit être superieure à 0"));
            modeConfig(); // On relance le mode
        }
    }

    else if (commandeEnter == "FILE_MAX_SIZE") // Si la commande est FILE_MAX_SIZE
    {
        Serial.println(F("FILE_MAX_SIZE"));                                 // On affiche FILE_MAX_SIZE
        Serial.println(F("Rentrez la nouvelle valeur pour FILE_MAX_SIZE")); // On demande la nouvelle valeur pour FILE_MAX_SIZE
        ReadandConvert();                                                   // On appelle la fonction ReadandConvert
        if (valeurEnterInt > 0)                                             // Si la valeur est superieure à 0
        {
            Serial.print(F("FILE_MAX_SIZE mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a ete mise à jour
            EEPROM.put(2, valeurEnterInt);                            // On met à jour la valeur dans l’EEPROM
            Serial.println(EEPROM.get(2, valeurEnterInt));            // On affiche la nouvelle valeur
            modeConfig();                                                  // On relance le mode
        }
        else // Si la valeur est inferieure à 0
        {
            Serial.print(F("ERREUR : Valeur incorrecte ")); // On informe l’utilisateur que la valeur est incorrecte
            Serial.print(valeurEnterInt);
            Serial.println(F(" doit être superieure à 0"));
            modeConfig(); // On relance le mode
        }
    }

    else if (commandeEnter == "RESET") // Si la commande est RESET
    {
        Serial.println(F("RESET"));                                                                                           // On affiche RESET
        Serial.println(F("Voulez-vous vraiment reinitialiser l’ensemble des paramètres à leurs valeurs par defaut ? (O/N)")); // On demande confirmation à l’utilisateur pour reinitialiser les paramètres à leurs valeurs par defaut
        ReadandTrimString();                                                                                                  // On appelle la fonction ReadandTrimString
        if (valeurEnterString == "O")                                                                                         // Si la valeur est O
        {
            // oui tkt ça va reset les paramètres à leurs valeurs par defaut
            Serial.println(F("RESET effectue")); // On informe l’utilisateur que le RESET a ete effectue
            IntialisationsVar();                 // On appelle la fonction IntialisationsVar
            modeConfig();                             // On relance le mode
        }
        else if (valeurEnterString == "N") // Si la valeur est N
        {
            Serial.println(F("RESET annule")); // On affiche comme quoi le reset a ete annule
            modeConfig();                           // On relance le mode
        }
        else // Si la valeur est autre chose que O ou N
        {
            Serial.print(F("ERREUR : Valeur incorrecte -> ")); // On affiche un message d’erreur
            Serial.println(valeurEnterString);                 // On affiche la valeur entree
            Serial.println(F("Veuillez entrer O ou N"));       // On affiche un message d’information
            modeConfig();                                           // On relance le mode
        }
    }

    else if (commandeEnter == "VERSION") // Si la commande est VERSION
    {
        Serial.println(F("VERSION")); // On affiche VERSION
        attendre(100);
        Serial.println(F("La version du système est la 1.0")); // On affiche la version du système
        attendre(100);
        Serial.println(F("Le numero de lot est le 123456")); // On affiche le numero de lot
        attendre(100);
        modeConfig(); // On relance le mode
    }

    else if (commandeEnter == "TIMEOUT") // Si la commande est TIMEOUT
    {
        Serial.println(F("TIMEOUT"));                                     // On affiche TIMEOUT
        Serial.println(F("Rentrez la nouvelle valeur pour TIMEOUT (s)")); // On demande la nouvelle valeur pour TIMEOUT en secondes
        ReadandConvert();                                                 // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= 0)                                          // Si la valeur n'est pas vide et si la valeur est superieure ou egale à 0
        {
            Serial.print(F("TIMEOUT mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a ete mise à jour
            EEPROM.put(4, valeurEnterInt);                      // On met à jour la valeur dans l’EEPROM
            Serial.println(EEPROM.get(4, valeurEnterInt));      // On affiche la nouvelle valeur
            modeConfig();                                            // On relance le mode
        }
        else // Si la valeur est inferieure à 0
        {
            Serial.println(F("Valeur incorrecte -> "));                              // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                          // On affiche la valeur entree
            Serial.println(F("Veuillez entrer une valeur superieure ou egale à 0")); // On affiche un message d’information
            modeConfig();                                                                 // On relance le mode
        }
    }
    else if (commandeEnter == "LUMIN") // Si la commande est LUMIN
    {
        Serial.println(F("LUMIN"));                                 // On affiche LUMIN
        Serial.println(F("Rentrez la nouvelle valeur pour LUMIN")); // On demande une nouvelle valeur pour LUMIN
        ReadandConvert();                                           // On appelle la fonction ReadandConvert
        switch (valeurEnterInt)                                     // On verifie la valeur entree
        {
            {
            case 0:                                                               // Si la valeur est 0
                Serial.println(F("Capteur de luminosite desactive avec succès")); // On informe l’utilisateur que la valeur a ete mise à jour
                EEPROM.put(6, valeurEnterInt);                                    // On met à jour la valeur dans l’EEPROM
                modeConfig();                                                          // On relance le mode
                break;
            case 1:                                                            // Si la valeur est 1
                Serial.println(F("Capteur de luminosite active avec succès")); // On informe l’utilisateur que la valeur a ete mise à jour
                EEPROM.put(6, valeurEnterInt);                                 // On met à jour la valeur dans l’EEPROM
                modeConfig();                                                       // On relance le mode
                break;
            default:                                                          // Si la valeur est autre chose que 0 ou 1
                Serial.println(F("Valeur incorrecte, choisir entre 0 et 1")); // On affiche un message d’erreur
                modeConfig();                                                      // On relance le mode
                break;
            }
        }
    }

    else if (commandeEnter == "LUMIN_LOW") // Si la commande est LUMIN_LOW
    {
        Serial.println(F("LUMIN_LOW"));                                 // On affiche LUMIN_LOW
        Serial.println(F("Rentrez la nouvelle valeur pour LUMIN_LOW")); // On demande la nouvelle valeur pour LUMIN_LOW
        ReadandConvert();                                               // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= 0 && valeurEnterInt <= 1023)              // Si la valeur n'est pas vide et est comprise entre 0 et 1023
        {
            Serial.print(F("LUMIN_LOW mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a ete mise à jour
            EEPROM.put(8, valeurEnterInt);                        // On met à jour la valeur dans l’EEPROM
            Serial.println(EEPROM.get(8, valeurEnterInt));        // On affiche la nouvelle valeur
            modeConfig();                                              // On relance le mode
        }
    }

    else if (commandeEnter == "LUMIN_HIGH") // Si la commande est LUMIN_HIGH
    {
        Serial.println(F("LUMIN_HIGH"));                                 // On affiche LUMIN_HIGH
        Serial.println(F("Rentrez la nouvelle valeur pour LUMIN_HIGH")); // On demande la nouvelle valeur pour LUMIN_HIGH
        ReadandConvert();                                                // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= 0 && valeurEnterInt <= 1023)               // Si la valeur n'est pas vide et est comprise entre 0 et 1023
        {
            Serial.print("LUMIN_HIGH mis à jour à la valeur "); // On informe l’utilisateur que la valeur a ete mise à jour
            EEPROM.put(10, valeurEnterInt);                     // On met à jour la valeur dans l’EEPROM
            Serial.println(EEPROM.get(10, valeurEnterInt));     // On affiche la nouvelle valeur
            modeConfig();                                            // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre 0 et 1023
        {
            Serial.println(F("Valeur incorrecte -> "));                               // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                           // On affiche la valeur entree
            Serial.println(F("Veuillez entrer une valeur comprise entre 0 et 1023")); // On affiche un message d’information
            modeConfig();                                                                  // On relance le mode
        }
    }

    else if (commandeEnter == "TEMP_AIR") // Si la commande est TEMP_AIR
    {
        Serial.println(F("TEMP_AIR"));                                 // On affiche TEMP_AIR
        Serial.println(F("Rentrez la nouvelle valeur pour TEMP_AIR")); // On demande la nouvelle valeur pour TEMP_AIR
        ReadandConvert();                                              // On appelle la fonction ReadandConvert
        switch (valeurEnterInt)                                        // On verifie la valeur entree
        {
            {
            case 0:                                                                // Si la valeur est 0
                Serial.println(F("Capteur de temperature desactive avec succès")); // On informe l’utilisateur que la valeur a ete mise à jour
                EEPROM.put(12, valeurEnterInt);                                    // On met à jour la valeur dans l’EEPROM
                modeConfig();                                                           // On relance le mode
                break;
            case 1:                                                             // Si la valeur est 1
                Serial.println(F("Capteur de temperature active avec succès")); // On informe l’utilisateur que la valeur a ete mise à jour
                EEPROM.put(12, valeurEnterInt);                                 // On met à jour la valeur dans l’EEPROM
                modeConfig();                                                        // On relance le mode
                break;
            default:                                                // Si la valeur est autre chose que 0 ou 1
                Serial.println(F("Valeur incorrecte -> "));         // On affiche un message d’erreur
                Serial.println(valeurEnterInt);                     // On affiche la valeur entree
                Serial.println(F("Veuillez choisir entre 0 et 1")); // On affiche un message d’erreur
                modeConfig();                                            // On relance le mode
                break;
            }
        }
    }

    else if (commandeEnter == "MIN_TEMP_AIR") // Si la commande est MIN_TEMP_AIR
    {
        Serial.println(F("MIN_TEMP_AIR"));                                 // On affiche MIN_TEMP_AIR
        Serial.println(F("Rentrez la nouvelle valeur pour MIN_TEMP_AIR")); // On demande la nouvelle valeur pour MIN_TEMP_AIR
        ReadandConvert();                                                  // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= -40 && valeurEnterInt <= 85)                 // Si la valeur n'est pas vide et est comprise entre -40 et 85
        {
            Serial.print(F("MIN_TEMP_AIR mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a ete mise à jour
            EEPROM.put(14, valeurEnterInt);                          // On met à jour la valeur dans l’EEPROM
            Serial.println(EEPROM.get(14, valeurEnterInt));          // On affiche la nouvelle valeur
            modeConfig();                                                 // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre -40 et 85
        {
            Serial.println(F("Valeur incorrecte -> "));                               // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                           // On affiche la valeur entree
            Serial.println(F("Veuillez entrer une valeur comprise entre -40 et 85")); // On affiche un message d’information
            modeConfig();                                                                  // On relance le mode
        }
    }

    else if (commandeEnter == "MAX_TEMP_AIR") // Si la commande est MAX_TEMP_AIR
    {
        Serial.println(F("MAX_TEMP_AIR"));                                 // On affiche MAX_TEMP_AIR
        Serial.println(F("Rentrez la nouvelle valeur pour MAX_TEMP_AIR")); // On demande la nouvelle valeur pour MAX_TEMP_AIR
        ReadandConvert();                                                  // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= -40 && valeurEnterInt <= 85)                 // Si la valeur n'est pas vide et est comprise entre -40 et 85
        {
            Serial.print(F("MAX_TEMP_AIR mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a ete mise à jour
            EEPROM.put(16, valeurEnterInt);                          // On met à jour la valeur dans l’EEPROM
            Serial.println(EEPROM.get(16, valeurEnterInt));          // On affiche la nouvelle valeur
            modeConfig();                                                 // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre -40 et 85
        {
            Serial.println(F("Valeur incorrecte -> "));                               // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                           // On affiche la valeur entree
            Serial.println(F("Veuillez entrer une valeur comprise entre -40 et 85")); // On affiche un message d’information
            modeConfig();                                                                  // On relance le mode
        }
    }

    else if (commandeEnter == "HYGR") // Si la commande est HYGR
    {
        Serial.println(F("HYGR"));                                 // On affiche HYGR
        Serial.println(F("Rentrez la nouvelle valeur pour HYGR")); // On demande la nouvelle valeur pour HYGR
        ReadandConvert();                                          // On appelle la fonction ReadandConvert
        switch (valeurEnterInt)                                    // On verifie la valeur entree
        {
            {
            case 0:                                                            // Si la valeur est 0
                Serial.println(F("Capteur d'humidite desactive avec succès")); // On informe l’utilisateur que la valeur a ete mise à jour
                EEPROM.put(18, valeurEnterInt);                                // On met à jour la valeur dans l’EEPROM
                modeConfig();                                                       // On relance le mode
                break;
            case 1:                                                         // Si la valeur est 1
                Serial.println(F("Capteur d'humidite active avec succès")); // On informe l’utilisateur que la valeur a ete mise à jour
                EEPROM.put(18, valeurEnterInt);                             // On met à jour la valeur dans l’EEPROM
                modeConfig();                                                    // On relance le mode
                break;
            default:                                                // Si la valeur est autre chose que 0 ou 1
                Serial.print(F("Valeur incorrecte -> "));           // On affiche un message d’erreur
                Serial.println(valeurEnterInt);                     // On affiche la valeur entree
                Serial.println(F("Veuillez choisir entre 0 et 1")); // On affiche un message d’erreur
                modeConfig();                                            // On relance le mode
                break;
            }
        }
    }

    else if (commandeEnter == "HYGR_MINT") // Si la commande est HYGR_MINT
    {
        Serial.println(F("HYGR_MINT"));                                 // On affiche HYGR_MINT
        Serial.println(F("Rentrez la nouvelle valeur pour HYGR_MINT")); // On demande la nouvelle valeur pour HYGR_MINT
        ReadandConvert();                                               // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= -45 && valeurEnterInt <= 85)              // Si la valeur n'est pas vide et est comprise entre -45 et 85
        {
            Serial.print(F("HYGR_MINT mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a ete mise à jour
            EEPROM.put(20, valeurEnterInt);                       // On met à jour la valeur dans l’EEPROM
            Serial.println(EEPROM.get(20, valeurEnterInt));       // On affiche la nouvelle valeur
            modeConfig();                                              // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre -45 et 85
        {
            Serial.println(F("Valeur incorrecte -> "));                               // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                           // On affiche la valeur entree
            Serial.println(F("Veuillez entrer une valeur comprise entre -45 et 80")); // On affiche un message d’information
            modeConfig();                                                                  // On relance le mode
        }
    }

    else if (commandeEnter == "HYGR_MAXT") // Si la commande est HYGR_MAXT
    {
        Serial.println(F("HYGR_MAXT"));                                 // On affiche HYGR_MAXT
        Serial.println(F("Rentrez la nouvelle valeur pour HYGR_MAXT")); // On demande la nouvelle valeur pour HYGR_MAXT
        ReadandConvert();                                               // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= -45 && valeurEnterInt <= 85)              // Si la valeur n'est pas vide et est comprise entre -45 et 85
        {
            Serial.print(F("HYGR_MAXT mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a ete mise à jour
            EEPROM.put(22, valeurEnterInt);                       // On met à jour la valeur dans l’EEPROM
            Serial.println(EEPROM.get(22, valeurEnterInt));       // On affiche la nouvelle valeur
            modeConfig();                                              // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre -45 et 85
        {
            Serial.println(F("Valeur incorrecte -> "));                               // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                           // On affiche la valeur entree
            Serial.println(F("Veuillez entrer une valeur comprise entre -45 et 80")); // On affiche un message d’information
            modeConfig();                                                                  // On relance le mode
        }
    }

    else if (commandeEnter == "PRESSURE") // Si la commande est PRESSURE
    {
        Serial.println(F("PRESSURE"));                                 // On affiche PRESSURE
        Serial.println(F("Rentrez la nouvelle valeur pour PRESSURE")); // On demande la nouvelle valeur pour PRESSURE
        ReadandConvert();                                              // On appelle la fonction ReadandConvert
        switch (valeurEnterInt)                                        // On verifie la valeur entree
        {
            {
            case 0:                                                             // Si la valeur est 0
                Serial.println(F("Capteur de pression desactive avec succès")); // On informe l’utilisateur que la valeur a ete mise à jour
                EEPROM.put(24, valeurEnterInt);                                 // On met à jour la valeur dans l’EEPROM
                modeConfig();                                                        // On relance le mode
                break;
            case 1:                                                          // Si la valeur est 1
                Serial.println(F("Capteur de pression active avec succès")); // On informe l’utilisateur que la valeur a ete mise à jour
                EEPROM.put(24, valeurEnterInt);                              // On met à jour la valeur dans l’EEPROM
                modeConfig();                                                     // On relance le mode
                break;
            default:                                                // Si la valeur est autre chose que 0 ou 1
                Serial.println(F("Valeur incorrecte -> "));         // On affiche un message d’erreur
                Serial.println(valeurEnterInt);                     // On affiche la valeur entree
                Serial.println(F("Veuillez choisir entre 0 et 1")); // On affiche un message d’erreur
                modeConfig();                                            // On relance le mode
                break;
            }
        }
    }

    else if (commandeEnter == "PRESSURE_MIN") // Si la commande est PRESSURE_MIN
    {
        Serial.println(F("PRESSURE_MIN"));                                 // On affiche PRESSURE_MIN
        Serial.println(F("Rentrez la nouvelle valeur pour PRESSURE_MIN")); // On demande la nouvelle valeur pour PRESSURE_MIN
        ReadandConvert();                                                  // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= 300 && valeurEnterInt <= 1100)               // Si la valeur n'est pas vide et est comprise entre 300 et 1100
        {
            Serial.print(F("PRESSURE_MIN mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a ete mise à jour
            EEPROM.put(26, valeurEnterInt);                          // On met à jour la valeur dans l’EEPROM
            Serial.println(EEPROM.get(26, valeurEnterInt));          // On affiche la nouvelle valeur
            modeConfig();                                                 // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre 300 et 1100
        {
            Serial.println(F("Valeur incorrecte -> "));                                 // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                             // On affiche la valeur entree
            Serial.println(F("Veuillez entrer une valeur comprise entre 300 et 1100")); // On affiche un message d’information
            modeConfig();                                                                    // On relance le mode
        }
    }

    else if (commandeEnter == "PRESSURE_MAX") // Si la commande est PRESSURE_MAX
    {
        Serial.println(F("PRESSURE_MAX"));                                 // On affiche PRESSURE_MAX
        Serial.println(F("Rentrez la nouvelle valeur pour PRESSURE_MAX")); // On demande la nouvelle valeur pour PRESSURE_MAX
        ReadandConvert();                                                  // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= 300 && valeurEnterInt <= 1100)               // Si la valeur n'est pas vide et est comprise entre 300 et 1100
        {
            Serial.print(F("PRESSURE_MAX mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a ete mise à jour
            EEPROM.put(28, valeurEnterInt);                          // On met à jour la valeur dans l’EEPROM
            Serial.println(EEPROM.get(28, valeurEnterInt));          // On affiche la nouvelle valeur
            modeConfig();                                                 // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre 300 et 1100
        {
            Serial.println(F("Valeur incorrecte -> "));                                 // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                             // On affiche la valeur entree
            Serial.println(F("Veuillez entrer une valeur comprise entre 300 et 1100")); // On affiche un message d’information
            modeConfig();                                                                    // On relance le mode
        }
    }

    else if (commandeEnter == "CLOCK") // Si la commande est CLOCK
    {
        int heureEnter;   // Variable pour stocker l'heure entree
        int minuteEnter;  // Variable pour stocker les minutes entrees
        int secondeEnter; // Variable pour stocker les secondes entrees

        Serial.println(F("CLOCK"));              // On affiche CLOCK
        Serial.println(F("Rentrez l'heure"));    // On demande l'heure
        heureEnter = ReadandConvert();           // On appelle la fonction ReadandConvert et on stocke la valeur entree dans la variable heureEnter
        if (heureEnter >= 0 && heureEnter <= 23) // Si l'heure est comprise entre 0 et 23
        {
            Serial.println(F("Rentrez les minutes"));  // On demande les minutes
            minuteEnter = ReadandConvert();            // On appelle la fonction ReadandConvert et on stocke la valeur entree dans la variable minuteEnter
            if (minuteEnter >= 0 && minuteEnter <= 59) // Si les minutes sont comprises entre 0 et 59
            {
                Serial.println(F("Rentrez les secondes"));   // On demande les secondes
                secondeEnter = ReadandConvert();             // On appelle la fonction ReadandConvert et on stocke la valeur entree dans la variable secondeEnter
                if (secondeEnter >= 0 && secondeEnter <= 59) // Si les secondes sont comprises entre 0 et 59
                {
                    Serial.print(F("L'heure a ete mise à jour à ")); // On informe l’utilisateur que la valeur a ete mise à jour
                    EEPROM.put(30, heureEnter);                      // On met à jour la valeur dans l’EEPROM
                    Serial.print(EEPROM.get(30, heureEnter));        // On affiche la nouvelle valeur
                    Serial.print(F(":"));                            // On affiche un :
                    EEPROM.put(32, minuteEnter);                     // On met à jour la valeur dans l’EEPROM
                    Serial.print(EEPROM.get(32, minuteEnter));       // On affiche la nouvelle valeur
                    Serial.print(F(":"));                            // On affiche un :
                    EEPROM.put(34, secondeEnter);                    // On met à jour la valeur dans l’EEPROM
                    Serial.println(EEPROM.get(34, secondeEnter));    // On affiche la nouvelle valeur
                    modeConfig();                                         // On relance le mode
                }
                else // Si les secondes ne sont pas comprises entre 0 et 59
                {
                    Serial.println(F("Valeur incorrecte -> "));                             // On affiche un message d’erreur
                    Serial.println(secondeEnter);                                           // On affiche la valeur entree
                    Serial.println(F("Veuillez entrer une valeur comprise entre 0 et 59")); // On affiche un message d’information
                    modeConfig();                                                                // On relance le mode
                }
            }
            else // Si les minutes ne sont pas comprises entre 0 et 59
            {
                Serial.println(F("Valeur incorrecte -> "));                             // On affiche un message d’erreur
                Serial.println(minuteEnter);                                            // On affiche la valeur entree
                Serial.println(F("Veuillez entrer une valeur comprise entre 0 et 59")); // On affiche un message d’information
                modeConfig();
            }
        }
        else // Si l'heure n'est pas comprise entre 0 et 23
        {
            Serial.println(F("Valeur incorrecte -> "));                             // On affiche un message d’erreur
            Serial.println(heureEnter);                                             // On affiche la valeur entree
            Serial.println(F("Veuillez entrer une valeur comprise entre 0 et 23")); // On affiche un message d’information
            modeConfig();                                                                // On relance le mode
        }
    }

    else if (commandeEnter == "DATE") // Si la commande est DATE
    {
        int moisEnter;  // Variable pour stocker le mois entre
        int jourEnter;  // Variable pour stocker le jour entre
        int anneeEnter; // Variable pour stocker l'annee entree

        Serial.println(F("DATE"));             // On affiche DATE
        Serial.println(F("Rentrez le mois"));  // On demande le mois
        moisEnter = ReadandConvert();          // On appelle la fonction ReadandConvert et on stocke la valeur entree dans la variable moisEnter
        if (moisEnter >= 1 && moisEnter <= 12) // Si le mois est compris entre 1 et 12
        {
            Serial.println(F("Rentrez le jour"));  // On demande le jour
            jourEnter = ReadandConvert();          // On appelle la fonction ReadandConvert et on stocke la valeur entree dans la variable jourEnter
            if (jourEnter >= 1 && jourEnter <= 31) // Si le jour est compris entre 1 et 31
            {
                Serial.println(F("Rentrez l'annee"));    // On demande l'annee
                anneeEnter = ReadandConvert();           // On appelle la fonction ReadandConvert et on stocke la valeur entree dans la variable anneeEnter
                if (anneeEnter >= 0 && anneeEnter <= 99) // Si l'annee est comprise entre 0 et 99
                {
                    Serial.print(F("La date a ete mise à jour à ")); // On informe l’utilisateur que la valeur a ete mise à jour
                    EEPROM.put(36, moisEnter);                       // On met à jour la valeur dans l’EEPROM
                    Serial.print(EEPROM.get(36, moisEnter));         // On affiche la nouvelle valeur
                    Serial.print(F("/"));                            // On affiche un /
                    EEPROM.put(38, jourEnter);                       // On met à jour la valeur dans l’EEPROM
                    Serial.print(EEPROM.get(38, jourEnter));         // On affiche la nouvelle valeur
                    Serial.print(F("/"));                            // On affiche un /
                    EEPROM.put(40, anneeEnter);                      // On met à jour la valeur dans l’EEPROM
                    Serial.println(EEPROM.get(40, anneeEnter));      // On affiche la nouvelle valeur
                    modeConfig();                                         // On relance le mode
                }
                else // Si l'annee n'est pas comprise entre 0 et 99
                {
                    Serial.println(F("Valeur incorrecte -> ")); // On affiche un message d’erreur
                    attendre(100);
                    Serial.println(anneeEnter); // On affiche la valeur entree
                    attendre(100);
                    Serial.println(F("Veuillez entrer une valeur comprise entre 0 et 99")); // On affiche un message d’information
                    attendre(100);
                    modeConfig(); // On relance le mode
                }
            }
            else // Si le jour n'est pas compris entre 1 et 31
            {
                Serial.println(F("Valeur incorrecte -> ")); // On affiche un message d’erreur
                attendre(100);
                Serial.println(jourEnter); // On affiche la valeur entree
                attendre(100);
                Serial.println(F("Veuillez entrer une valeur comprise entre 1 et 31")); // On affiche un message d’ information
                attendre(100);
                modeConfig(); // On relance le mode
            }
        }
        else // Si le mois n'est pas compris entre 1 et 12
        {
            Serial.println(F("Valeur incorrecte -> ")); // On affiche un message d’erreur
            attendre(100);
            Serial.println(moisEnter); // On affiche la valeur entree
            attendre(100);
            Serial.println(F("Veuillez entrer une valeur comprise entre 1 et 12")); // On affiche un message d’information
            attendre(100);
            modeConfig(); // On relance le mode
        }
    }

    else if (commandeEnter == "DAY") // Si la commande est DAY
    {
        Serial.println(F("DAY"));                                                                                                                                                     // On demande le jour de la semaine
        String jourSemaineEnter = ReadandTrimString();                                                                                                                                                               // On appelle la fonction ReadandTrimString et on stocke la valeur entree dans la variable jourSemaineEnter
        if (jourSemaineEnter == "MON" || jourSemaineEnter == "TUE" || jourSemaineEnter == "WED" || jourSemaineEnter == "THU" || jourSemaineEnter == "FRI" || jourSemaineEnter == "SAT" || jourSemaineEnter == "SUN") // Si le jour de la semaine entre est egal à l'un des jours de la semaine
        {
            attendre(100);
            EEPROM.put(42, jourSemaineEnter);                             // On met à jour la valeur dans l’EEPROM
            Serial.println(EEPROM.get(42, jourSemaineEnter));             // On affiche la nouvelle valeur
            modeConfig();                                                      // On relance le mode
        }
        else // Si le jour de la semaine entre n'est pas egal à l'un des jours de la semaine
        {
            Serial.println(F("Valeur incorrecte -> ")); // On affiche un message d’erreur
            Serial.println(jourSemaineEnter);           // On affiche la valeur entree
            modeConfig();                                    // On relance le mode
        }
    }

    else
    {
        Serial.print(F("Commande "));
        Serial.print(commandeEnter);
        Serial.print(F(" inconnue \n"));
        modeConfig();
    }
}

int ReadandConvert()
{
    valeurEnterString = ReadandTrimString();    // On lit la valeur entree et on la convertit en String (pour pouvoir utiliser la fonction .toInt())
    valeurEnterInt = valeurEnterString.toInt(); // On convertit la valeur entree en int
    return valeurEnterInt;                      // On retourne la valeur entree en int
}

String ReadandTrimString()
{
    while (Serial.available() == 0) // Tant que rien n’est entre dans la console on attend
    {
    }
    valeurEnterString = Serial.readString(); // On stocke la valeur entree dans la variable valeurEnterString
    valeurEnterString.trim();                // On supprime les espaces avant et après la valeur entree
    return valeurEnterString;                // On retourne la valeur trimee en String
}

void IntialisationsVar()
{
    // Initialisation des variables dans l'EEPROM

    int logInterval = 10;       // Intervalle entre 2 mesures (en s)
    EEPROM.put(0, logInterval); // On stocke la valeur de logInterval dans l'EEPROM

    int file_max_size = 4096;     // Taille maximale du fichier de log (en octets)
    EEPROM.put(2, file_max_size); // On stocke la valeur de file_max_size dans l'EEPROM

    int timeout = 30;       // Duree (en s) au bout de laquelle l’acquisition des donnees d’un capteur est abandonnee.
    EEPROM.put(4, timeout); // On stocke la valeur de timeout dans l'EEPROM

    int lumin = 1;        // Activation (1) ou desactivation (0) du capteur de luminosite
    EEPROM.put(6, lumin); // On stocke la valeur de lumin dans l'EEPROM

    int lumin_min = 255;      // definition de la valeur en dessous de laquelle la luminosite est consideree comme faible
    EEPROM.put(8, lumin_min); // On stocke la valeur de lumin_min dans l'EEPROM

    int lumin_max = 768;       // definition de la valeur au-dessus de laquelle la luminosite est consideree comme forte
    EEPROM.put(10, lumin_max); // On stocke la valeur de lumin_max dans l'EEPROM

    int temp_air = 1;         // Activation (1) ou desactivation (0) du capteur de temperature de l’air
    EEPROM.put(12, temp_air); // On stocke la valeur de temp_air dans l'EEPROM

    int min_temp_air = -10;       // definition du seuil de temperature de l'air (en °C) en dessous duquel le capteur se mettra en erreur.
    EEPROM.put(14, min_temp_air); // On stocke la valeur de min_temp_air dans l'EEPROM

    int max_temp_air = 60;        // definition du seuil de temperature de l'air (en °C) au-dessus duquel le capteur se mettra en erreur.
    EEPROM.put(16, max_temp_air); // On stocke la valeur de max_temp_air dans l'EEPROM

    int hygr = 1;         // Activation (1) ou desactivation (0) du capteur d’humidite
    EEPROM.put(18, hygr); // On stocke la valeur de hygr dans l'EEPROM

    int hygr_mint = 0;         // definition de la temperature en dessous de laquelle les mesures d’hygrometrie ne seront pas prises en compte
    EEPROM.put(20, hygr_mint); // On stocke la valeur de hygr_mint dans l'EEPROM

    int hygr_maxt = 50;        // definition de la temperature au-dessus de laquelle les mesures d’hygrometrie ne seront pas prises en compte
    EEPROM.put(22, hygr_maxt); // On stocke la valeur de hygr_maxt dans l'EEPROM

    int pressure = 1;         // Activation (1) ou desactivation (0) du capteur de pression
    EEPROM.put(24, pressure); // On stocke la valeur de pressure dans l'EEPROM

    int pressure_min = 850;       // definition du seuil de pression atmospherique (en HPa) en dessous duquel le capteur se mettra en erreur
    EEPROM.put(26, pressure_min); // On stocke la valeur de pressure_min dans l'EEPROM

    int pressure_max = 1080;      // definition du seuil de pression atmospherique (en HPa) au-dessus duquel le capteur se mettra en erreur
    EEPROM.put(28, pressure_max); // On stocke la valeur de pressure_max dans l'EEPROM

    int heure = 0;         // definition de l'heure
    EEPROM.put(30, heure); // On stocke la valeur de heure dans l'EEPROM

    int minute = 0;         // definition des minutes
    EEPROM.put(32, minute); // On stocke la valeur de minute dans l'EEPROM

    int seconde = 0;         // definition des secondes
    EEPROM.put(34, seconde); // On stocke la valeur de seconde dans l'EEPROM

    int jour = 01;        // definition du jour
    EEPROM.put(36, jour); // On stocke la valeur de jour dans l'EEPROM

    int mois = 01;        // definition du mois
    EEPROM.put(38, mois); // On stocke la valeur de mois dans l'EEPROM

    int annee = 00;        // definition de l'annee
    EEPROM.put(40, annee); // On stocke la valeur de annee dans l'EEPROM

    String jour_semaine = "MON";  // definition du jour de la semaine
    EEPROM.put(42, jour_semaine); // On stocke la valeur de jour_semaine dans l'EEPROM
}