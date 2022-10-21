// Importation de la bibliothèque permettant de gérer la LED
#include <ChainableLED.h>
// Importation de la bibliothèque permettant d'utiliser le capteur d'humidité et de température
#include "DHT.h"
// Importation des bibliothèques permettant d'utiliser la carte SD
#include "SPI.h"
#include "SD.h"

// Déclaration du type et du pin utilisés pour les capteurs d'humidité et de température
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

// Booléens pour controler la pression ou non du bouton, true = non pressé
bool bRouge = true;
bool bVert = true;

// Variable stockant le mode précédent pour le mode maintenance
int precMode;

// Définition du délai entre deux mesure (1000 = 1 seconde)
const int delaiMesure = 10000;

// Variable stockant le temps écoulé depuis le début du programme à l'aide de millis()
long duree, delai;

// Variables contenant les valeurs obtenues des capteurs
float lumiere, temperature, humidite, pression;

// Enumération des modes principaux
enum mode {Standard = 0, Eco, Maintenance, Config, Debut};
//
mode Mode;

// 
File monFichier;


// Intitialisation du programme
void setup() 
{
  // Initialisation du mode à "Debut" : dans aucun des modes principaux
  Mode = Debut;
  // Définition du débit de données
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
  attendre(5);
  // Si pas d'interruption pour passer au mode config, passage au mode standard
  if(Mode == Debut){
    Mode = Standard;
  }
}


// Fonction d'interruption en cas de changement de position du bouton rouge
void appuiRouge(){
  // Lecture de la position du bouton rouge
  bRouge = digitalRead(boutonRouge);
  // Si le bouton est appuyé (on vient de le presser), on stocke le temps écoulé depuis le début du programme
  if(!bRouge){
    duree = millis();
  }

///////////////////////////////////////////////////////////////////////////////////////////////////

  // Si le bouton n'est pas appuyé (on vient de le lâcher), on teste s'il a été appuyé pendant au moins 5 secondes
  else if(bRouge){
    if(millis()-duree > 5000){
      // Si l'on est au début du programme, on passe Mode à Config
      if(Mode == Debut){
        Mode = Config;
      }
      // Si on est dans le mode Standard ou Eco, on passe au mode Maintenance après avoir stocké le précédent mode
      else if(Mode == Standard || Mode == Eco){
        precMode = Mode;
        Mode = Maintenance;
      }
      // Si on était dans le mode Maintenance, on repasse au mode précédent (Standard ou Eco)
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
  // Si le bouton est appuyé (on vient de le presser), on stocke le temps écoulé depuis le début du programme
  if(!bVert){
    duree = millis();
  }
  // Si le bouton n'est pas appuyé (on vient de le lâcher), on teste s'il a été appuyé pendant au moins 5 secondes
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
  case 3:
    modeConfig();
    break;
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
  attendre(delaiMesure);
  Serial.println("Mode standard");
  delay(10);
  leds.setColorRGB(0, 0, 255, 0);
  lumiere = mesureLumiere();
  humidite = mesureHumidite();
  temperature = mesureTemperature();
  pression = mesurePression();
  erreur(lumiere, humidite, temperature, pression);
  sauvMesures(lumiere, humidite, temperature, pression);
}

void modeEco(){
  if(millis()-delai >= delaiMesure * 2){
    delai = millis();
    Serial.println("Eco");
    delay(10);
    leds.setColorRGB(0, 0, 0, 255);
  }
}
void modeConfig(){
  Serial.println("Conf");
  leds.setColorRGB(0, 255, 255, 0);
}

void modeMaintenance(){
  Serial.println("Maintenance");
  delay(10);
  leds.setColorRGB(0, 255, 127, 0);
  lumiere = mesureLumiere();
  humidite = mesureHumidite();
  temperature = mesureTemperature();
  pression = mesurePression();
  erreur(lumiere, humidite, temperature, pression);
  affichage(lumiere, humidite, temperature, pression);
  delay(100);

}

void affichage(float lumiere, float humidite, float temperature, float pression){
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


float mesureLumiere()
{
  float lumiere = analogRead(pinLux);
  Serial.println(lumiere);
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
  float pression = 8.3144621 * (273.15 + temperature) * masseVolumique(temperature) / 0.029;
  return pression;
}



 float masseVolumique(float temperature){
   if(temperature < 7.5){
     return 1.292;
   }
    else if(7.5 <= temperature < 17.5){
     return 1.225 ;
   }
    else if(17.5 <= temperature < 22.5){
     return 1.204 ;
   }
    else if(22.5 <= temperature){
     return 1.292 ;
   }
 }

void erreur(float lumiere, float humidite, float temperature, float pression){
  // horloge 0
  // GPS 1
  // capteur inaccessible 2
  //données incoherentes 3
  if(lumiere < 0 || lumiere > 1000 || humidite < 0 || humidite > 100 || temperature < -50 || temperature > 50 || pression < 100000 || pression > 110000){
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
        delay(1000);
        leds.setColorRGB(0, 0, 0, 255);
        delay(1000);
        break;
      case 1: // GPS
        leds.setColorRGB(0, 255, 0, 0);
        delay(1000);
        leds.setColorRGB(0, 255, 255, 0);
        delay(1000);
        break;
      case 2: // acces capteur
        leds.setColorRGB(0, 255, 0, 0);
        delay(1000);
        leds.setColorRGB(0, 0, 255, 0);
        delay(1000);
        break;
      case 3: // donnee incoherente
        leds.setColorRGB(0, 255, 0, 0);
        delay(1000);
        leds.setColorRGB(0, 0, 255, 0);
        delay(2000);
        break;
      case 4: // SD pleine
        leds.setColorRGB(0, 255, 0, 0);
        delay(1000);
        leds.setColorRGB(0, 255, 255, 255);
        delay(1000);
        break;
      case 5: // SD inaccessible
        leds.setColorRGB(0, 255, 0, 0);
        delay(1000);
        leds.setColorRGB(0, 255, 255, 255);
        delay(2000);
        break;
    }
  }
}

void sauvMesures(float mes1, float mes2,float mes3, float mes4)
{
  //Initialiser une variable qui compte le nombre de fichiers dans un dossier
  int nbFichiers = 0;
  
  int FILE_MAX_SIZE = 4096;

  if (!SD.exists("sys3w")) {
    Serial.println (F("Création dossier"));
   if(!SD.mkdir("sys3w")){
      Serial.println("Erreur creation dossier");
  }   
  } 
  
  
  Serial.println (F("Dossier cree "));
  // Serial.println("Dossier sys3w cree ");
  

  //ouvrir un fichier dans repertoire "sys3w_releve_mesures"

  //Déterminer le chemin pour arriver à notre dossier et modifier son nom
  char datafile[33];
  int jour = 15; // moment.day();
  int mois = 10; // moment.month();
  int annee = 22; // moment.year()
  
  sprintf(datafile,"sys3w/%d%d%d_%d.LOG",jour,mois,annee,nbFichiers);  //  %d pour un int
  //datafile = "sys3w_releve_mesures" + "/" + jour + mois + année + "_" + nbFichiers + "." + "LOG" ;

  Serial.print("Ouverture ");
  Serial.println(datafile);
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

      fichier.close();
      do
      {
        Serial.println(nbFichiers);
        
        nbFichiers++;
        if(nbFichiers==9){
          Serial.println (F("Dossier plein"));
          Archivage();
          nbFichiers=0;
          move=false;
        }
        sprintf(datafile2,"sys3w/%d%d%d_%d.LOG",jour,mois,annee,nbFichiers);  //  %d pour un int
        //datafile2 = "sys3w" + "/" + jour + mois + annee + "_" + nbFichiers ;
        Serial.println(datafile2);
        
      }while(SD.exists(datafile2));
      
      
      if(move)
      {
        Serial.println("move");
        //renommer le fichier
        SDrename(datafile,datafile2);
      }

      //ouvrir à nouveau le fichier (maintenant vide)
      fichier = SD.open(datafile, FILE_WRITE);
      if(!fichier){
        Serial.println (F("erreur d'ouverture fichier"));
      }
      Serial.println(datafile);
    }
    // ecrire les donnees dans le fichier
    fichier.print(lumiere); //("%d,%d,%d,%d", lumiere, humidite, temperature, pression);  //datafile.println
    fichier.print(",");
    
    fichier.print(humidite);
    fichier.print(",");
    
    fichier.print(temperature);
    fichier.print(",");
    
    fichier.print(pression);
    fichier.println(".");
    
    fichier.close();
    Serial.println (F("Releve des capteurs ecrits sur la carte SD"));
    
  }
  else
  {
    Serial.println (F("erreur ouverture fichier"));
  }
}

void SDrename(char* source,char* destination){
  File ficsource;
  File ficdestination;
  Serial.println("SDrename");
  
  ficsource = SD.open(source, FILE_READ);
  Serial.println(source);
  
  if(!ficsource)
  {
    Serial.println (F("erreur ouverture fichier source"));
  }
  ficdestination = SD.open(destination, FILE_WRITE);
  Serial.println(destination);
  
  if(!ficdestination)
  {
    Serial.println (F("erreur ouverture fichier destination"));
  }


  
  size_t data;
  uint8_t buf[64];
  
  while((data= ficsource.read(buf,sizeof(buf))) >= 0){
    Serial.println(data);
    ficdestination.write(buf, data);

  }
  Serial.println("data copiée");
  ficsource.close();
  ficdestination.close();
  SD.remove(source);
}

void Archivage() {

  // Ouvre le premier fichier
  File repfile = SD.open("/sys3w/");

  File entry = repfile.openNextFile();
  int a=0;
  String en = String(entry);
  Serial.println(en);
  Serial.println("Parcours dossiers rep");

  while (entry) {
    Serial.println(entry.name());
    if (entry.isDirectory()) {
      a++;
      Serial.println("rep");     
    } 
    entry = repfile.openNextFile();
  }
  repfile.close();
  
  char nomDoss[17];
  sprintf(nomDoss,"sys3w/arch_%d", a);
    //nomDoss= "sys3w_releve_mesures" + "_" + a;
  Serial.println(nomDoss);
  
  Serial.println("Création nouveau dossier"); 
  if(!SD.mkdir(nomDoss)){
      Serial.println("Erreur creation dossier");
  }
  Serial.println("doss cree");
  delay(100);
  
  File repfile2 = SD.open("/sys3w/");

  entry = repfile2.openNextFile();
  
  String rep = String(repfile2);
  Serial.println(rep);
  //mettre dans ce nouveau doss

  en = String(entry);
  Serial.println(en);
  
  Serial.println("Parcours fichiers rep");
  while (entry) {
    Serial.println(entry.name());
     
    if (entry.isDirectory()) {
    Serial.println("rep");
    } else { 
      Serial.println("fic");
      char nomFic[50];
      sprintf(nomFic,"sys3w/arch_%d/%s", a, entry.name());
      SDrename(entry.name(),nomFic);
      Serial.println(nomFic);
      
    } 
    entry = repfile2.openNextFile();
    }
    repfile2.close();
  Serial.println("fin Parcours fichiers rep");
  
}
