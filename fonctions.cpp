// Programme contenant les differentes focntions annexes
// Ces fonctions sont utiles à l'exécution du fichier main.cpp qui contient le programe principale

#include <stdlib.h>
#include <SoftwareSerial.h>
#include <iarduino_RTC.h>

iarduino_RTC watch (RTC_DS1307);

// Renvoie un flottant envoyé en entrée arrondi au centieme
float arrondi(float var)
{
  float value = (int)(var * 100 + .5);
  return (float)value / 100;
}


/*
Toutes les fonctions suivantes permettent de récupérer les données des cpateurs
Chaque fonction attend en paramètre le numero de pin des capteurs
Elles renvoient ensuite la valeur captee de type float (a l'exception de la fonction GPS)
*/
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

float Heure()
{
    heure = time.gettime("d-m-Y, H:i:s, D");
    return heure;
}

float mesureLumière(pinLux)
{
    float lux = arrondi(analogRead(pinLux));
    return lux;
}

float mesureAir(pinAir)
{
    float air = arrondi(analogRead(pinAir));
    return air;
}

float mesureTempAir(pinTempAir)
{
    float tempAir = arrondi(analogRead(pinTempAir));
    return tempAir;
}

float mesureTempEau(pinTempEau)
{
    float tempEau = arrondi(analogRead(pinTempEau));
    return tempEau;
}

float mesureHygro(pinHygro)
{
    float hygro = arrondi(analogRead(pinHygro));
    return hygro;
}