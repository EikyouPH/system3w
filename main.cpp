// Programme principal
// Il est le premier lancé et contient toutes les fonctions principales
#include <Arduino/SPI.h>
#include <Arduino/SD.h>
#include <fonctions.cpp>

SoftwareSerial ss(4, 3);
const int delaiMesure = 10000;
const int delaiMesureEco = 20000;
const int pinCS = 11;
const int boutonRouge = 5;
const int boutonVert = 6;
const int ledOrange = 7;
const int ledVert = 8;
const int ledBleue = 9;
const int ledJaune = 10;
const int pinLux = A1;
enum Mode {Standard = 0, Eco, Maintenance};

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  Serial.println("Demarrage du programme");
  SD.begin(pinCS);
  pinMode(ledVert, OUTPUT);
  pinMode(ledBleue, OUTPUT);
  pinMode(ledJaune, OUTPUT);
  pinMode(ledOrange, OUTPUT);
  pinMode(boutonRouge, INPUT);
  pinMode(boutonVert, INPUT);
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
      if (boutonVert == HIGH){
        Eco();
      }
      else if (boutonRouge == HIGH){
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
        checkErreur();
        delay(delaiMesureEco);
        if (boutonVert == HIGH){
            modeStandard();
        }
        else if (boutonRouge == HIGH){
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
    if (boutonRouge == HIGH){
      Eco();
    }
    else if (boutonVert == HIGH){
      Standard();
    }
    else {
      Maintenance();
    }
  }
}