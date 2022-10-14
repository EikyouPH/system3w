// Ce mode permet de configurer le système grâce à une interaction depuis une console sur l’interface série

#include "mode_config.h"
#include "config.h"
#include "mode.h"

// Initialisation du mode
void mode_config_init()
{
    Serial.begin(9600);                    // Initialisation de la liaison série
    Serial.println("Mode config");         // Affichage d’un message de bienvenue
    Serial.println("Entrez une commande"); // Affichage d’un message d’information
}

// Boucle de traitement du mode
void mode_config_loop()
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
            // Mettre à jour la valeur de la variable LOG_INTERVALL
            break;
        case 'm':
            Serial.println("Activation du mode manuel");
            mode_manuel_init();
            break;
        case 'c':
            Serial.println("Activation du mode config");
            mode_config_init();
            break;
        case 's':
            Serial.println("Activation du mode stop");
            mode_stop_init();
            break;
        case 'd':
            Serial.println("Activation du mode démo");
            mode_demo_init();
            break;
        case 'r':
            Serial.println("Activation du mode réglage");
            mode_reglage_init();
            break;
        case 't':
            Serial.println("Activation du mode test");
            mode_test_init();
            break;
        case 'p':
            Serial.println("Activation du mode programmation");
            mode_programmation_init();
            break;
        case 'q':
            Serial.println("Activation du mode quitter");
            mode_quitter_init();
            break;
        default:
            Serial.println("Commande inconnue");
            break;
        }
    }
}