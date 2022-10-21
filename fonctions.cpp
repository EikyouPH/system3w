#include <ChainableLED.h>
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
#include "DHT.h"
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
  delay(5000);
  if (Mode == Debut)
  {
    Mode = Standard;
  }
}

// Fonction permettant de basculer d'un mode à l'autre
void Modes()
{
  // Mode Standard
  if (Mode == Standard)
  {
    modeStandard();
  }
  // Mode Eco
  else if (Mode == Eco)
  {
    modeEco();
  }
  // Mode Maintenance
  else if (Mode == Maintenance)
  {
    modeMaintenance();
  }
  // Mode Config
  else if (Mode == Config)
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
  Serial.println("Mode standard");
  delay(10);
  leds.setColorRGB(0, 0, 255, 0);
  mesureCapteurs();
  clignotement(5);
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
}

void mesureCapteurs()
{
  int lumiere = analogRead(pinLux);
  Serial.println(lumiere);
  float temp_hum_val[2] = {0};
  dht.readTempAndHumidity(temp_hum_val);
  int humidite = temp_hum_val[0];
  int temperature = temp_hum_val[1];
  float pression = 8.3144621 * (273.15 + temperature) * masseVolumique(temperature) / 0.029;
  erreur(lumiere, humidite, temperature, pression);
}

float masseVolumique(int temperature)
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

void erreur(int lumiere, int humidite, int temperature, float pression)
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
<<<<<<< HEAD
}

// Fonction appelée lors de l'appui sur le bouton rouge en mode éco
void appuiBoutonRougeE()
{
  Mode = Maintenance; // On passe en mode maintenance
  return Mode;        // On renvoie la couleur et le mode
}

// Fonction appelée lors de l'appui sur le bouton rouge en mode éco
void appuiBoutonVertE()
{
  Mode = Standard; // On passe en mode maintenance
  return Mode;     // On renvoie la couleur et le mode
  == == == =
>>>>>>> experimental
}