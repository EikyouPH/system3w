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
    void Standard(){
      mesureCapteurs();
      delay(1000);
      saveMesure();
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

void Maintenance() {
  Serial.println("Mode Maintenance");
  delay(1000);
  void Maintenance(){
    mesureCapteurs();
    delay(1000);
    saveMesure();
    delay(delaiMesure);
    checkErreur();
    if (BoutonVert == HIGH){
      Eco();
    }
    else if (BoutonRouge == HIGH){
      Standard();
    }
    else {
      Maintenance();
    }
  }
}