#include <ChainableLED.h>
ChainableLED leds(6, 7, 1);
enum mode {Standard = 0, Eco, Maintenance, Config, Debut};
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

void appuiRouge(){
  bRouge = digitalRead(boutonRouge);
  if(!bRouge){
    duree = millis();
  }
  else if(bRouge){
    if(millis()-duree > 5000){
      if(Mode == Debut){
        Mode = Config;
      }
      else if(Mode == Standard || Mode == Eco){
        precMode = Mode;
        Mode = Maintenance;
      }
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


void appuiVert(){
  bVert = digitalRead(boutonVert);
  if(!bVert){
    duree = millis();
  }
  else if(bVert){
    if(millis()-duree > 5000){
      if(Mode == Standard){
        Mode = Eco;
      }
      else if(Mode == Eco){
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

  //interruption possible sur le bouton rouge pour passer en mode config
  delay(5000);
  if(Mode == Debut){
    Mode = Standard;
  }
}

// Fonction permettant de basculer d'un mode à l'autre
void Modes() 
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

void loop(){
  Modes();
}

void modeStandard(){
  Serial.println("Mode standard");
  delay(10);
  leds.setColorRGB(0, 0, 255, 0);
  mesureCapteurs();
}

void modeEco(){
  Serial.println("Eco");
  delay(10);
  leds.setColorRGB(0, 0, 0, 255);
}
void modeConfig(){
  Serial.println("Conf");
  leds.setColorRGB(0, 255, 255, 0);
}
void modeMaintenance(){
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
}

float masseVolumique(int temperature){
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