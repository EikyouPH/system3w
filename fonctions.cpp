#include <stdlib.h>
#include <SoftwareSerial.h>



//Arrondi un float à 2 chiffres après la virgule
float arround2(float var)
{
  float value = (int)(var * 100 + .5);
  return (float)value / 100;
}

byte GPS(pinGPS)
{
  while (ss.available() > 0){
    byte gpsData = ss.read();
  }
  return gpsData;
}

float Heure()
{
    float heure = round2(analogRead(pinHeure));
    return heure;
}

float mesureLumière(pinLux)
{
    float lux = round2(analogRead(pinLux));
    return lux;
}
float mesureAir(pinAir)
{
    float air = round2(analogRead(pinAir));
    return air;
}

float mesureTempAir(pinTempAir)
{
    float tempAir = round2(analogRead(pinTempAir));
    return tempAir;
}

float mesureTempEau(pinTempEau)
{
    float tempEau = round2(analogRead(pinTempEau));
    return tempEau;
}

float mesureHygro(pinHygro)
{
    float hygro = round2(analogRead(pinHygro));
    return hygro;
}