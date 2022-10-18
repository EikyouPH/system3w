// Ce mode permet de configurer le système grâce à une interaction depuis une console sur l’interface série

//#include "mode_config.h"
//#include "config.h"
//#include "mode.h"
//#include <iostream>

String c; // variable pour stocker la commande entrée par l’utilisateur

// Initialisation du mode
void setup()
{
    Serial.begin(9600);                    // Initialisation de la liaison série
    Serial.println("Mode config");         // Affichage d’un message de bienvenue
    Serial.println("Entrez une commande"); // Affichage d’un message d’information
}

// Boucle de traitement du mode
void loop()
{
    if (Serial.available() > 0) // Si les données sont disponibles sur la liaison série
    {
        int RetourNumComm;
        while (c == "") // Atendre l'entré de l'utilisateur
        {
            delay(10);
            c = Serial.readString();
        }

        if (c == "LOG_INTERVAL")
        {
            Serial.println("LOG_INTERVAL");
        }

        else if (c == "FILE_MAX_SIZE")
        {
            Serial.println("FILE_MAX_SIZE");
        }

        else if (c == "RESET")
        {
            Serial.println("RESET");
        }

        else if (c == "VERSION")
        {
            Serial.println("VERSION");
        }

        else if (c == "TIMEOUT")
        {
            Serial.println("TIMEOUT");
        }

        else if (c == "LUMIN_LOW")
        {
            Serial.println("LUMIN_LOW");
        }

        else if (c == "LUMIN_HIGH")
        {
            Serial.println("LUMIN_HIGH");
        }

        else if (c == "TEMP_AIR")
        {
            Serial.println("TEMP_AIR");
        }

        else if (c == "MIN_TEMP_AIR")
        {
            Serial.println("MIN_TEMP_AIR");
        }

        else if (c == "MAX_TEMP_AIR")
        {
            Serial.println("MAX_TEMP_AIR");
        }

        else if (c == "HYGR")
        {
            Serial.println("HYGR");
        }

        else if (c == "HYGR_MINT")
        {
            Serial.println("HYGR_MINT");
        }

        else if (c == "HYGR_MAXT")
        {
            Serial.println("HYGR_MAXT");
        }

        else if (c == "PRESSURE")
        {
            Serial.println("PRESSURE");
        }

        else if (c == "PRESSURE_MIN")
        {
            Serial.println("PRESSURE_MIN");
        }

        else if (c == "PRESSURE_MAX")
        {
            Serial.println("PRESSURE_MAX");
        }

        else if (c == "CLOCK")
        {
            Serial.println("CLOCK");
        }

        else if (c == "DATE")
        {
            Serial.println("DATE");
        }

        else if (c == "DAY")
        {
            Serial.println("DAY");
        }

        else if (c == "HELP")
        {
            Serial.println("HELP");
        }

        else
        {
            Serial.println("Commande inconnue");
        }
    }
}
