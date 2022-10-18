enum mode {Standard = 0, Eco, Maintenance, Config};
const int boutonRouge = 2;
const int boutonVert = 3;
bool bRouge = true;
bool bVert = true;
mode Mode;

void appuiRouge(){
  if(Mode == NULL){
    Mode = Config;
  }
}

// Intitialisation du programme
void setup() 
{
  Serial.begin(9600);
  Serial.println("Demarrage du programme");
  pinMode(boutonRouge, INPUT);
  pinMode(boutonVert, INPUT);

  //interruption possible sur le bouton rouge pour passer en mode config
  attachInterrupt(digitalPinToInterrupt(boutonRouge), appuiRouge, CHANGE);
  delay(10000);
  if(Mode == NULL){
    Mode = Standard;
  }
}

void loop(){
  if(Mode == Standard){
    Serial.println("Stand");
  }
  else if(Mode == Config){
    Serial.println("Conf");
  }
}

