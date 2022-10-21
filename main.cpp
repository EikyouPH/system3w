// Importation de la bibliothèque permettant de gérer la LED
#include <ChainableLED.h>
// Importation de la bibliothèque permettant d'utiliser le capteur d'humidité et de température
#include "DHT.h"

ChainableLED leds(6, 7, 1);
enum mode
{
  Standard = 0,
  Eco,
  Maintenance,
  Config,
  Debut
};
const int boutonRouge = 2;
const int boutonVert = 3;
// Pins pour les capteurs
const int pinLux = A1;
bool bRouge = true;
bool bVert = true;
mode Mode;
int precMode;
long duree;
int tmp;

#define DHTTYPE DHT11
#define DHTPIN 8
DHT dht(DHTPIN, DHTTYPE);

void appuiRouge()
{
  bRouge = digitalRead(boutonRouge);
  if (!bRouge)
  {
    duree = millis();
  }
  else if (bRouge)
  {
    if (millis() - duree > 5000)
    {
      if (Mode == Debut)
      {
        Mode = Config;
      }
      else if (Mode == Standard || Mode == Eco)
      {
        precMode = Mode;
        Mode = Maintenance;
      }
      else if (Mode == Maintenance)
      {
        if (precMode == Eco)
        {
          Mode = Eco;
        }
        else
        {
          Mode = Standard;
        }
      }
    }
  }
}

void appuiVert()
{
  bVert = digitalRead(boutonVert);
  if (!bVert)
  {
    duree = millis();
  }
  else if (bVert)
  {
    if (millis() - duree > 5000)
    {
      if (Mode == Standard)
      {
        Mode = Eco;
      }
      else if (Mode == Eco)
      {
        Mode = Standard;
      }
    }
  }
}

// Intitialisation du programme
void setup()
{
  Mode = Debut;
  Serial.begin(9600);
  Serial.println("Demarrage du programme");
  pinMode(boutonRouge, INPUT);
  pinMode(boutonVert, INPUT);
  pinMode(pinLux, INPUT);
  Wire.begin();
  dht.begin();
  // Initialisation des interruptions
  attachInterrupt(digitalPinToInterrupt(boutonRouge), appuiRouge, CHANGE);
  attachInterrupt(digitalPinToInterrupt(boutonVert), appuiVert, CHANGE);

  // interruption possible sur le bouton rouge pour passer en mode config
  Mode = Config;
  attachInterrupt(digitalPinToInterrupt(boutonRouge), modeConfig, CHANGE);
  attendre(10);
  Mode = Standard;
}

// Fonction permettant de basculer d'un mode à l'autre
void Modes(Mode)
{
  // Mode Standard
  if  (Mode == Standard)
  {
    modeStandard();
  }
  // Mode Eco
  else if (Mode == Eco)
  {
    modeEco();
  }
  // Mode Maintenance
  else if  (Mode == Maintenance)
  {
    modeMaintenance();
  }
  // Mode Config
  else if  (Mode == Config)
  {
    modeConfig();
  }
  // Gestion des erreurs : si pas de mode, renvoie 0
  return 0;
}

void loop()
{
  Modes();
}

void modeStandard()
{
  // Initialisation des interruptions
  attachinterrupt(digitalPinToInterrupt(boutonRouge), appuiBoutonRougeS, CHANGE);
  attachinterrupt(digitalPinToInterrupt(boutonVert), appuiBoutonVertS, CHANGE);
  // Mesure et sauvegarde des capteurs avec vérification des erreurs
  mesureCapteurs();
}

void modeEco()
{
  Serial.println("Eco");
  delay(10);
  leds.setColorRGB(0, 0, 0, 255);
}
void modeConfig()
{
  Serial.println("Conf");
  leds.setColorRGB(0, 255, 255, 0);
}
void modeMaintenance()
{
  Serial.println("Maintenance");
  delay(10);
  leds.setColorRGB(0, 255, 127, 0);
  mesureCapteurs();
  delay(3000);
}

void affichage(float lumiere, float humidite, float temperature, float pression)
{
  Serial.print("Niveau luminosite : ");
  Serial.print(lumiere);
  Serial.println(" lux");
  Serial.print("Taux d'humidite : ");
  Serial.print(humidite);
  Serial.println("%");
  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println("*C");
  Serial.print("Pression  : ");
  Serial.print(pression);
  Serial.println(" Pa");
}

float mesureCapteurs()
{
  // Initialiser une variable qui compte le nombre de fichiers dans un dossier
  int nbFichiers = 0;
  SdFile fichier;

  // Ouvrir un dossier (le dossier ouvert est automatiquement créé si il n'existe pas)
  SD.mkdir("sys3w_relevé_mesures");

  // ouvrir un fichier dans repertoire "sys3w_relevé_mesures"

  char datafile[32];
  int jour = moment.day();
  int mois = moment.month();
  int annee = moment.year();
  sprintf(datafile, "sys3w_relevé_mesures/%d%d%d_%d.LOG", jour, mois, annee, nbFichiers); //  %d pour un int
  if (fichier = SD.open(datafile, FILE_WRITE))
  {
    if (fichier.position() > FILE_MAX_SIZE)
    {
      char datafile2[32];
      bool move = true;

      fichier.close();
      do
      {
        nbFichiers++;
        if (nbFichier == 9)
        {
          Archivage();
          nbFichiers = 0;
          move = false;
        }
        sprintf(datafile2, "sys3w_relevé_mesures/%d%d%d_%d.LOG", jour, mois, annee, nbFichiers); //  %d pour un int

      } while (SD.exists(datafile2));

      // renommer le fichier
      if (move)
      {
        SDrename(datafile, datafile2);
      }

      // ouvrir à nouveau le fichier (maintenant vide)
      if (!fichier = SD.open(datafile, FILE_WRITE))
      {
        Serial.println("erreur ouverture fichier");
      }
    }
    // écrire les données dans le fichier
    datafile.println(MesuresCapteurs);
    datafile.close();
  }
  else
  {
    Serial.println("erreur ouverture fichier");
  }
}

// fonction renommant un fichier

float masseVolumique(float temperature)
{
  if (temperature < 7.5)
  {
    return 1.292;
  }
  else if (7.5 <= temperature < 17.5)
  {
    return 1.225;
  }
  else if (17.5 <= temperature < 22.5)
  {
    return 1.204;
  }
  else if (22.5 <= temperature)
  {
    return 1.292;
  }
}

void erreur(float lumiere, float humidite, float temperature, float pression)
{
  // horloge 0
  // GPS 1
  // capteur inaccessible 2
  // données incoherentes 3
  if (lumiere < 0 || lumiere > 1000 || humidite < 0 || humidite > 100 || temperature < -50 || temperature > 50 || pression < 100000 || pression > 110000)
  {
    clignotement(3);
  }
  // carte sd pleine 4
  // carte sd inaccessible 5
  // passage au mode précédent
}
    if (!entry.isDirectory())
    {
      sprintf(nomFic, "sys3w_relevé_mesures/archives_%d/%s", a, entry.name());
      SDrename(entry.name(), nomFic);
    }
    entry = repfile.openNextFile();
  }
  repfile.close();
}

void clignotement(int type)
{
  while (true)
  {
    switch (type)
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
