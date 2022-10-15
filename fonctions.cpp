// Programme contenant les differentes focntions annexes
// Ces fonctions sont utiles à l'exécution du fichier main.cpp qui contient le programe principale

iarduino_RTC watch (RTC_DS1307);

// Définition de la fonction verifErreurs, qui retournera le nombre d'erreurs rencontrées à chaque série de mesure.
int verifErreurs(lux, pression, tempAir, tempEau, hygro)
{
  // Initialisation d'erreur à zéro à chaque vérification
  int erreur = 0;
  // Vérfication de la présence des mesures
  if(lux || pression || tempAir || tempEau || hygro == NULL) 
  {
    erreur++;
  }
  // Vérification de la cohérence physique des mesures
  else if(lux || pression || tempEau || hygro < 0) 
  {
    erreur++;
  }
  // Vérification de la cohérence de la luminosité
  else if(lux > 10000) 
  {
    erreur++;
  }
  // Vérification de la cohérence de la pression
  else if(pression > 1000000) 
  {
    erreur++;
  }
  // Vérification de la cohérence de la température de l'air
  else if(tempAir > 50) 
  {
    erreur++;
  }
  // Vérification de la cohérence de la température de l'eau
  else if(temp Eau > 50)
  {
    erreur++;
  }
  // Vérification de la cohérence de l'hygrométrie
  else if(hygro > 100)
  {
    erreur++;
  }
}

// Renvoie un flottant envoyé en entrée arrondi au centieme
float arrondi(float var)
{
  float value = (int)(var * 100 + .5);
  return (float)value / 100;
}

// Mesure les différentes museures des capteurs et les renvoie sur forme de float
float mesureCapteurs() 
{
  // Capteur de luminosité
  float lux = arrondi(analogRead(pinLux));

  // Capteur de pression atmophérique
  float pression = arrondi(analogRead(pinPression))

  // Capteur de température de l'air extérieur
  float tempAir = arrondi(analogRead(pinTempAir))

  // Capteur de température de l'eau
  float tempEau = arrondi(analogRead(pinTempEau))

  // Capteur d'humidité de l'air
  float hygro = arrondi(analogRead(pinHygro));

  // Renvoi de toutes les valeurs captées
  return lux, pression, tempAir, tempEau, hygro;
}

// Renvoie l'heure de captation des mesures et le renvoie sous forme de float
float Heure()
{
  heure = time.gettime("d-m-Y, H:i:s, D"); // Récupération de l'heure
  return heure; // Renvoi de l'heure
}

// Fonction GPS, renvoie la latitude et la longitude sous forme de float en récupéranr les données du GPS par le canal ss
byte GPS(pinGPS)
{
  while (ss.available() > 0)
  { // Si le canal ss est disponible
    gps.encode(ss.read()); // On récupère les données du GPS
    if (gps.location.isUpdated())
    { // Si les données sont mises à jour
      Serial.print("Latitude= ");  // On affiche la latitude
      Serial.print(gps.location.lat(), 6); // On affiche la latitude avec 6 chiffres après la virgule
      Serial.print(" Longitude= "); // On affiche la longitude
      Serial.println(gps.location.lng(), 6); // On affiche la longitude avec 6 chiffres après la virgule
    }
  }
}

// Gestion de la couleur de la LED
void couleurLed(Couleur)
{
  // Rouge
  if(Couleur == Rouge)
  {
    analogWrite(pinRouge, 255);
    analogWrite(pinVert, 0);
    analogWrite(pinBleu, 0);
  }
  // Vert
  else if (Couleur == Vert)
  {
    analogWrite(pinRouge, 0);
    analogWrite(pinVert, 255);
    analogWrite(pinBleu, 0);
  }
  // Jaune
  else if (Couleur == Jaune)
  {
    analogWrite(pinRouge, 127);
    analogWrite(pinVert, 127);
    analogWrite(PIN_BLUE, 0);
  }
  // Bleu
  else if (Couleur == Bleu)
  {
    analogWrite(pinRouge, 0);
    analogWrite(pinVert, 255);
    analogWrite(pinBleu, 0);
  }
  // Orange
  else if (Couleur == Orange)
  {
    analogWrite(pinRouge, 255);
    analogWrite(pinVert, 127);
    analogWrite(pinBleu,  127);
  }
  // Blanc
  else {
    analogWrite(pinRouge, 255);
    analogWrite(pinVert, 255);
    analogWrite(pinBleu, 255);
  }
}