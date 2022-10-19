// Ce mode permet de configurer le système grâce à une interaction depuis une console sur l’interface série

//#include "mode_config.h"
//#include "config.h"
//#include "mode.h"
//#include <iostream>

String commandeEnter; // variable pour stocker la commande entrée par l’utilisateur
String valeurEnter;   // variable pour stocker la valeur entrée par l’utilisateur
int valeurEnterInt;   // variable pour stocker la valeur entrée par l’utilisateur en int

// Initialisation du mode
void setup()
{
    Serial.begin(9600);                    // Initialisation de la liaison série
    Serial.println("Mode config");         // Affichage d’un message de bienvenue
    Serial.println("Entrez une commande"); // Affichage d’un message d’information
}

void loop()
{
    Commande(); // Appel de la fonction Commande
}

// Boucle de traitement du mode
void Commande()
{
    while (Serial.available() == 0)
    {
    }                                           // wait for data available
    String commandeEnter = Serial.readString(); // read until timeout
    commandeEnter.trim();
    if (commandeEnter == "LOG_INTERVAL")
    {
        Serial.println("LOG_INTERVAL");
        Serial.println("Rentrez la nouvelle valeur pour LOG_INTERVAL");
        while (Serial.available() == 0)
        {
        }
        valeurEnter = Serial.readString();
        if (valeurEnter != "")
        {
            valeurEnterInt = valeurEnter.toInt();
            Serial.print("LOG_INTERVAL mis à jour à la valeur ");
            Serial.println(valeurEnterInt);
            setup();
        }
    }

    else if (commandeEnter == "FILE_MAX_SIZE")
    {
        Serial.println("FILE_MAX_SIZE");
    }

    else if (commandeEnter == "RESET")
    {
        Serial.println("RESET");
    }

    else if (commandeEnter == "VERSION")
    {
        Serial.println("VERSION");
    }

    else if (commandeEnter == "TIMEOUT")
    {
        Serial.println("TIMEOUT");
    }

    else if (commandeEnter == "LUMIN_LOW")
    {
        Serial.println("LUMIN_LOW");
    }

    else if (commandeEnter == "LUMIN_HIGH")
    {
        Serial.println("LUMIN_HIGH");
    }

    else if (commandeEnter == "TEMP_AIR")
    {
        Serial.println("TEMP_AIR");
    }

    else if (commandeEnter == "MIN_TEMP_AIR")
    {
        Serial.println("MIN_TEMP_AIR");
    }

    else if (commandeEnter == "MAX_TEMP_AIR")
    {
        Serial.println("MAX_TEMP_AIR");
    }

    else if (commandeEnter == "HYGR")
    {
        Serial.println("HYGR");
    }

    else if (commandeEnter == "HYGR_MINT")
    {
        Serial.println("HYGR_MINT");
    }

    else if (commandeEnter == "HYGR_MAXT")
    {
        Serial.println("HYGR_MAXT");
    }

    else if (commandeEnter == "PRESSURE")
    {
        Serial.println("PRESSURE");
    }

    else if (commandeEnter == "PRESSURE_MIN")
    {
        Serial.println("PRESSURE_MIN");
    }

    else if (commandeEnter == "PRESSURE_MAX")
    {
        Serial.println("PRESSURE_MAX");
    }

    else if (commandeEnter == "CLOCK")
    {
        Serial.println("CLOCK");
    }

    else if (commandeEnter == "DATE")
    {
        Serial.println("DATE");
    }

    else if (commandeEnter == "DAY")
    {
        Serial.println("DAY");
    }

    else if (commandeEnter == "HELP")
    {
        Serial.println("HELP");
    }

    else
    {
        Serial.print("Commande ");
        Serial.print(commandeEnter);
        Serial.print(" inconnue \n");
        setup();
    }
}