// Programme contenant les differentes focntions annexes
// Ces fonctions sont utiles à l'exécution du fichier main.cpp qui contient le programe principale

// On définit l'horloge
iarduino_RTC watch(RTC_DS1307);

// Renvoie un flottant envoyé en entrée arrondi au centieme
float arrondi(float var)
{
  float value = (int)(var * 100 + .5);
  return (float)value / 100;
}

struct MesureCapteurs
{
  float Heure[1];
  float GPS[2];
  float Capteurs[5];
} mesure;

// Mesure les différentes museures des capteurs et les renvoie sur forme de float

// Renvoie l'heure de captation des mesures et le renvoie sous forme de float
float Heure()
{
  heure = time.gettime("d-m-Y, H:i:s, D"); // Récupération de l'heure
  return heure;                            // Renvoi de l'heure
}

// Fonction GPS, renvoie la latitude et la longitude sous forme de float en récupéranr les données du GPS par le canal ss
byte GPS(pinGPS)
{
  while (ss.available() > 0)
  {                        // Si le canal ss est disponible
    gps.encode(ss.read()); // On récupère les données du GPS
    if (gps.location.isUpdated())
    {                                      // Si les données sont mises à jour
      float lat = (gps.location.lat(), 6); // On récupère la latitude
      float lng = (gps.location.lng(), 6); // On récupère la longitude
    }
    return lat, lng; // On renvoie la latitude et la longitude
  }
}

float mesureCapteurs(lat, lng)
{

  // Capteur de luminosité
  float lux = arrondi(analogRead(pinLux));

  // Capteur de pression atmophérique
  float pression = captPrTemp.readPressure();

  // Capteur de température de l'air extérieur
  float tempAir = captPrTemp.readTemperature();

  // Capteur de température de l'eau
  float tempEau = arrondi(analogRead(pinTempEau));

  // Capteur d'humidité de l'air
  float hygro = arrondi(analogRead(pinHygro));

  struct MesureCapteurs mesure = {heure, lat, lng, lux, pression, tempAir, tempEau, hygro};
}

// Gestion de la couleur de la LED
void couleurLed(Couleur)
{
  // Rouge
  if (Couleur == Rouge)
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
    analogWrite(pinBleu, 127);
  }
  // Blanc
  else
  {
    analogWrite(pinRouge, 255);
    analogWrite(pinVert, 255);
    analogWrite(pinBleu, 255);
  }
}
