// Ce mode permet de configurer le système grâce à une interaction depuis une console sur l’interface série

#include "mode_config.h"
#include "config.h"
#include "mode.h"

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
        string c = Serial.read();
        Serial.print("Commande reçue : ");
        Serial.println(c);
        switch (c)
        {
        case 'LOG_INTERVALL':
            Serial.println("LOG_INTERVALL");
            // Demander la valeur de LOG_INTERVALL à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable LOG_INTERVALL
            break;
        case 'FILE_MAX_SIZE':
            Serial.println("FILE_MAX_SIZE");
            // Demander la valeur de FILE_MAX_SIZE à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable FILE_MAX_SIZE
            break;
        case 'RESET':
            Serial.println("RESET");
            // Demander la confirmation à l’utilisateur
            // Si la confirmation est positive
            // Remettre les valeurs par défaut des variables
            break;
        case 'VERSION':
            Serial.println("VERSION");
            // Afficher la version du système
            break;
        case 'TIMEOUT':
            Serial.println("TIMEOUT");
            // Demander la valeur de TIMEOUT à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable TIMEOUT
            break;
        case 'LUMIN':
            Serial.println("LUMIN");
            // Demander la valeur de LUMIN à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable LUMIN
            break;
        case 'LUMIN_MIN':
            Serial.println("LUMIN_MIN");
            // Demander la valeur de LUMIN_MIN à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable LUMIN_MIN
            break;
        case 'LUMIN_MAX':
            Serial.println("LUMIN_MAX");
            // Demander la valeur de LUMIN_MAX à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable LUMIN_MAX
            break;
        case 'TEMP_AIR':
            Serial.println("TEMP_AIR");
            // Demander la valeur de TEMP_AIR à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable TEMP_AIR
            break;
        case 'MIN_TEMP_AIR':
            Serial.println("TEMP_AIR_MIN");
            // Demander la valeur de TEMP_AIR_MIN à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable TEMP_AIR_MIN
            break;
        case 'MAX_TEMP_AIR':
            Serial.println("TEMP_AIR_MAX");
            // Demander la valeur de TEMP_AIR_MAX à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable TEMP_AIR_MAX
            break;
        case 'HYGR':
            Serial.println("HYGR");
            // Demander la valeur de HYGR à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable HYGR
            break;
        case 'HYGR_MINT':
            Serial.println("HYGR_MINT");
            // Demander la valeur de HYGR_MINT à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable HYGR_MINT
            break;
        case 'HYGR_MAXT':
            Serial.println("HYGR_MAXT");
            // Demander la valeur de HYGR_MAXT à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable HYGR_MAXT
            break;
        case 'PRESSURE':
            Serial.println("PRESSURE");
            // Demander la valeur de PRESSURE à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable PRESSURE
            break;
        case 'PRESSURE_MIN':
            Serial.println("PRESSURE_MIN");
            // Demander la valeur de PRESSURE_MIN à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable PRESSURE_MIN
            break;
        case 'PRESSURE_MAX':
            Serial.println("PRESSURE_MAX");
            // Demander la valeur de PRESSURE_MAX à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable PRESSURE_MAX
            break;
        case 'CLOCK':
            Serial.println("CLOCK");
            // Demander la valeur de CLOCK à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable CLOCK
        case 'DATE':
            Serial.println("DATE");
            // Demander la valeur de DATE à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable DATE
            break;
        case 'DAY':
            Serial.println("DAY");
            // Demander la valeur de DAY à l’utilisateur
            // Verifier si la valeur est dans le domaine de définition
            // Mettre à jour la valeur de la variable DAY
            break;
        case 'HELP':
            Serial.println("HELP");
            // Afficher la liste des commandes
            break;
        default:
            Serial.println("Commande inconnue");
            break;
        }
    }
}