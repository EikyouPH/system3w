#include <SPI.h>
#include <SD.h>
#include <fonctions.cpp>

const int delaiMesure = 10000;
const int pinCS = 10;

void setup() {
  Serial.begin(9600);
  Serial.println("Demarrage Standard");
  SD.begin(pinCS);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop() {
  Serial.println("Mode standard");
    delay(1000);
    void modeStandard(){
      mesureCapteurs();
      delay(1000);
      saveMesure();
      delay(delaiMesure);
      checkErreur();
      if (BoutonVert == HIGH){
        modeEco();
      }
      else if (BoutonRouge == HIGH){
        modeMaintenance();
      }
      else {
        modeStandard();
      }
  }
}

void modeEco() {
    Serial.println("Mode Eco");
    delay(1000);
    void modeEco(){
        mesureCapteurs();
        delay(1000);
        saveMesure();
        delay(delaiMesure);
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

void modeMaintenance() {
  Serial.println("Mode Maintenance");
  delay(1000);
  void modeMaintenance(){
    mesureCapteurs();
    delay(1000);
    saveMesure();
    delay(delaiMesure);
    checkErreur();
    if (BoutonVert == HIGH){
      modeEco();
    }
    else if (BoutonRouge == HIGH){
      modeStandard();
    }
    else {
      modeMaintenance();
    }
  }
}