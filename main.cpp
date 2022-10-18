#include <ChainableLED.h>
ChainableLED leds(6, 7, 1);
enum mode {Standard = 0, Eco, Maintenance, Config, Debut};
const int boutonRouge = 2;
const int boutonVert = 3;
bool bRouge = true;
bool bVert = true;
mode Mode;


void appuiRouge(){
  if(Mode == Debut){
    Mode = Config;
  }
  else if(Mode == Standard || Mode == Eco){
    Mode = Maintenance;
  }
}

void appuiVert(){
  if(Mode == Standard){
    Mode = Eco;
  }
  else if(Mode == Eco){
    Mode = Standard;
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
  // Initialisation des interruptions
  attachInterrupt(digitalPinToInterrupt(boutonRouge), appuiRouge, LOW);
  attachInterrupt(digitalPinToInterrupt(boutonVert), appuiVert, LOW);

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
  leds.setColorRGB(0, 0, 255, 0);
}

void modeEco(){
  Serial.println("Eco");
  leds.setColorRGB(0, 0, 0, 255);
}
void modeConfig(){
  Serial.println("Conf");
  leds.setColorRGB(0, 255, 255, 0);
}
void modeMaintenance(){
  Serial.println("Maintenance");
  leds.setColorRGB(0, 255, 127, 0);
}