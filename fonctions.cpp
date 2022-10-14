// Programme contenant les differentes focntions annexes
// Ces fonctions sont utiles à l'exécution du fichier main.cpp qui contient le programe principale

iarduino_RTC watch (RTC_DS1307);

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
    heure = time.gettime("d-m-Y, H:i:s, D");
    return heure;
}

//
byte GPS(pinGPS)
{
   while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
    }
  }
}

// Gestion de la couleur de la LED
void couleurLed(couleur)
{
  // Rouge
  if(couleur == Rouge){
  analogWrite(pinRouge, 255);
  analogWrite(pinVert, 0);
  analogWrite(pinBleu, 0);
  }
  // Vert
  else if (couleur == Vert){
  analogWrite(pinRouge, 0);
  analogWrite(pinVert, 255);
  analogWrite(pinBleu, 0);
  }
  // Jaune
  else if (couleur == Jaune){
  analogWrite(pinRouge, 127);
  analogWrite(pinVert, 127);
  analogWrite(PIN_BLUE, 0);
  }
  // Bleu
  else if (couleur == Bleu){
  analogWrite(pinRouge, 0);
  analogWrite(pinVert, 255);
  analogWrite(pinBleu, 0);
  }
  // Orange
  else if (couleur == Orange){
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