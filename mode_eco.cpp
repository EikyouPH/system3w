#include <SPI.h>
#include <SD.h>
const int delaiMesure = 1000;
const int pinCS = 10;

void setup() {
    Serial.begin(9600);
    Serial.println("Demarrage Eco");
    SD.begin(pinCS);
    pinMode(14, OUTPUT);
    digitalWrite(14, HIGH);
    delay(10000);
    digitalWrite(14, LOW);
}

void loop() {
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