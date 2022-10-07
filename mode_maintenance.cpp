#include <SPI.h>
#include <SD.h>
const int delaiMesure = 10000;
const int pinCS = 10;

void setup() {
  Serial.begin(9600);
  Serial.println("Demarrage Maintenance");
  SD.begin(pinCS);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
}

void loop() {
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