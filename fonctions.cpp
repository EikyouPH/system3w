#include <stdlib.h>
#include <SoftwareSerial.h>
#include <Tiny GPS++.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

//Arrondi un float à 2 chiffres après la virgule
float arround2(float var)
{
  float value = (int)(var * 100 + .5);
  return (float)value / 100;
}

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