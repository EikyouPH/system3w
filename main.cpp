// Programme principal
// Il est le premier lancé et contient toutes les fonctions principales
#include <Arduino/SPI.h>
#include <Arduino/SD.h>
#include <fonctions.cpp>

SoftwareSerial ss(4, 3);
const int delaiMesure = 10000;
const int delaiMesureEco = 20000;
const int pinCS = 11;
const int BoutonRouge = 5;
const int BoutonVert = 6;
const int ledOrange = 7;
const int ledVert = 8;
const int ledBleue = 9;
const int ledJaune = 10;

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  Serial.println("Demarrage du programme");
  SD.begin(pinCS);
  pinMode(ledVert, OUTPUT);
  pinMode(ledBleue, OUTPUT);
  pinMode(ledJaune, OUTPUT);
  pinMode(ledOrange, OUTPUT);
  pinMode(BoutonRouge, INPUT);
  pinMode(BoutonVert, INPUT);
  time.begin();
}

void loop() {
  Serial.println("Mode standard");
    digitalWrite(ledBleue, LOW);
    digitalWrite(ledOrange, LOW);
    digitalWrite(ledVert, HIGH);
    digitalWrite(ledJaune, LOW);
    delay(1000);
    void Standard(){
      mesureCapteurs();
      delay(1000);
      sauvMesure();
      delay(delaiMesure);
      checkErreur();
      if (BoutonVert == HIGH){
        Eco();
      }
      else if (BoutonRouge == HIGH){
        Maintenance();
      }
      else {
        Standard();
      }
  }
}

void Eco() {
    digitalWrite(ledBleue, HIGH);
    digitalWrite(ledOrange, LOW);
    digitalWrite(ledVert, lOW);
    digitalWrite(ledJaune, LOW);
    Serial.println("Mode Eco");
    delay(1000);
    void modeEco(){
        mesureCapteurs();
        delay(1000);
        sauvMesure();
        delay(delaiMesureEco);
        checkErreur();
        if (BoutonVert == HIGH){
            modeStandard();
        }
        else if (BoutonRouge == HIGH){
            modeMaintenance();
        }
        else {
            modeEco();
        }
    }
}

void Maintenance() {
  digitalWrite(ledBleue, LOW);
  digitalWrite(ledOrange, HIGH);
  digitalWrite(ledVert, lOW);
  digitalWrite(ledJaune, LOW);
  Serial.println("Mode Maintenance");
  delay(1000);
  void Maintenance(){
    stopMesure();
    accesSD();
    affSerie();
    if (BoutonRouge == HIGH){
      Eco();
    }
    else if (BoutonVert == HIGH){
      Standard();
    }
    else {
      Maintenance();
    }
  }
}