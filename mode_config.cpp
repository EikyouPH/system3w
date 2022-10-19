// Ce mode permet de configurer le système grâce à une interaction depuis une console sur l’interface série

String commandeEnter;     // variable pour stocker la commande entrée par l’utilisateur
String valeurEnterString; // variable pour stocker la valeur entrée par l’utilisateur
int valeurEnterInt;       // variable pour stocker la valeur entrée par l’utilisateur en int

// Initialisation du mode
void setup()
{
    Serial.begin(9600);                       // Initialisation de la liaison série
    Serial.println(F("Mode config"));         // Affichage d’un message de bienvenue
    Serial.println(F("Entrez une commande")); // Affichage d’un message d’information
}

void loop()
{
    Commande(); // Appel de la fonction Commande
}

// Boucle de traitement du mode
void Commande()
{

    while (Serial.available() == 0) // Tant que rien n’est entré dans la console on attend
    {
    }

    commandeEnter = Serial.readString(); // On stocke la commande entrée dans la variable commandeEnter
    commandeEnter.trim();                // On supprime les espaces avant et après la commande

    if (commandeEnter == "LOG_INTERVAL") // Si la commande est LOG_INTERVAL
    {
        Serial.println(F("LOG_INTERVAL"));                                 // On affiche LOG_INTERVAL
        Serial.println(F("Rentrez la nouvelle valeur pour LOG_INTERVAL")); // On demande la nouvelle valeur pour LOG_INTERVAL
        ReadandConvert();                                                  // On appelle la fonction ReadandConvert
        if (valeurEnterInt > 0)                                            // Si la valeur est supérieure à 0
        {
            Serial.print(F("LOG_INTERVAL mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a été mise à jour
            Serial.println(valeurEnterInt);                          // On affiche la valeur entrée
            setup();                                                 // On relance le mode
        }
        else // Si la valeur est inférieure ou égale à 0
        {
            Serial.print(F("ERREUR : Valeur incorrecte ")); // On informe l’utilisateur que la valeur est incorrecte
            Serial.print(valeurEnterInt);
            Serial.println(F(" doit être supérieure à 0"));
            setup(); // On relance le mode
        }
    }

    else if (commandeEnter == "FILE_MAX_SIZE") // Si la commande est FILE_MAX_SIZE
    {
        Serial.println(F("FILE_MAX_SIZE"));                                 // On affiche FILE_MAX_SIZE
        Serial.println(F("Rentrez la nouvelle valeur pour FILE_MAX_SIZE")); // On demande la nouvelle valeur pour FILE_MAX_SIZE
        ReadandConvert();                                                   // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= 0)                                            // Si la valeur est supérieure à 0
        {
            Serial.print(F("FILE_MAX_SIZE mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a été mise à jour
            Serial.println(valeurEnterInt);                           // On affiche la valeur entrée
            setup();                                                  // On relance le mode
        }
        else // Si la valeur est inférieure à 0
        {
            Serial.print(F("ERREUR : Valeur incorrecte ")); // On informe l’utilisateur que la valeur est incorrecte
            Serial.print(valeurEnterInt);
            Serial.println(F(" doit être supérieure à 0"));
            setup(); // On relance le mode
        }
    }

    else if (commandeEnter == "RESET") // Si la commande est RESET
    {
        Serial.println(F("RESET"));                                                                                           // On affiche RESET
        Serial.println(F("Voulez-vous vraiment réinitialiser l’ensemble des paramètres à leurs valeurs par défaut ? (O/N)")); // On demande confirmation à l’utilisateur pour réinitialiser les paramètres à leurs valeurs par défaut
        while (Serial.available() == 0)                                                                                       // Tant que rien n’est entré dans la console on attend
        {
        }
        valeurEnterString = Serial.readString(); // On stocke la valeur entrée dans la variable valeurEnter
        if (valeurEnterString == "O")            // Si la valeur est O
        {
            // oui tkt ça va reset les paramètres à leurs valeurs par défaut
            Serial.println(F("RESET effectué")); // On informe l’utilisateur que le RESET a été effectué
        }
        else if (valeurEnterString == "N") // Si la valeur est N
        {
            Serial.println(F("RESET annulé")); // On affiche comme quoi le reset a été annulé
            setup();                           // On relance le mode
        }
        else // Si la valeur est autre chose que O ou N
        {
            Serial.print(F("ERREUR : Valeur incorrecte -> ")); // On affiche un message d’erreur
            Serial.println(valeurEnterString);                 // On affiche la valeur entrée
            Serial.println(F("Veuillez entrer O ou N"));       // On affiche un message d’information
            setup();                                           // On relance le mode
        }
    }

    else if (commandeEnter == "VERSION") // Si la commande est VERSION
    {
        Serial.println(F("VERSION"));                          // On affiche VERSION
        Serial.println(F("La version du système est la 1.0")); // On affiche la version du système
        Serial.println(F("Le numéro de lot est le 123456"));   // On affiche le numéro de lot
        setup();                                               // On relance le mode
    }

    else if (commandeEnter == "TIMEOUT") // Si la commande est TIMEOUT
    {
        Serial.println(F("TIMEOUT"));                                     // On affiche TIMEOUT
        Serial.println(F("Rentrez la nouvelle valeur pour TIMEOUT (s)")); // On demande la nouvelle valeur pour TIMEOUT en secondes
        ReadandConvert();                                                 // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= 0)                                          // Si la valeur n'est pas vide et si la valeur est supérieure ou égale à 0
        {
            Serial.print(F("TIMEOUT mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a été mise à jour
            Serial.println(valeurEnterInt);                     // On affiche la valeur entrée
            setup();                                            // On relance le mode
        }
        else // Si la valeur est inférieure à 0
        {
            Serial.println(F("Valeur incorrecte -> "));                              // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                          // On affiche la valeur entrée
            Serial.println(F("Veuillez entrer une valeur supérieure ou égale à 0")); // On affiche un message d’information
            setup();                                                                 // On relance le mode
        }
    }
    else if (commandeEnter == "LUMIN") // Si la commande est LUMIN
    {
        Serial.println(F("LUMIN"));                                 // On affiche LUMIN
        Serial.println(F("Rentrez la nouvelle valeur pour LUMIN")); // On demande une nouvelle valeur pour LUMIN
        ReadandConvert();                                           // On appelle la fonction ReadandConvert
        switch (valeurEnterInt)                                     // On vérifie la valeur entrée
        {
            {
            case 0:                                                               // Si la valeur est 0
                Serial.println(F("Capteur de luminosité désactivé avec succès")); // On informe l’utilisateur que la valeur a été mise à jour
                setup();                                                          // On relance le mode
                break;
            case 1:                                                            // Si la valeur est 1
                Serial.println(F("Capteur de luminosité activé avec succès")); // On informe l’utilisateur que la valeur a été mise à jour
                setup();                                                       // On relance le mode
                break;
            default:                                                          // Si la valeur est autre chose que 0 ou 1
                Serial.println(F("Valeur incorrecte, choisir entre 0 et 1")); // On affiche un message d’erreur
                setup();                                                      // On relance le mode
                break;
            }
        }
    }

    else if (commandeEnter == "LUMIN_LOW") // Si la commande est LUMIN_LOW
    {
        Serial.println(F("LUMIN_LOW"));                                 // On affiche LUMIN_LOW
        Serial.println(F("Rentrez la nouvelle valeur pour LUMIN_LOW")); // On demande la nouvelle valeur pour LUMIN_LOW
        ReadandConvert();                                               // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= 0 && valeurEnterInt <= 1023)              // Si la valeur n'est pas vide et est comprise entre 0 et 1023
        {
            Serial.print(F("LUMIN_LOW mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a été mise à jour
            Serial.println(valeurEnterInt);                       // On affiche la valeur entrée
            setup();                                              // On relance le mode
        }
    }

    else if (commandeEnter == "LUMIN_HIGH") // Si la commande est LUMIN_HIGH
    {
        Serial.println(F("LUMIN_HIGH"));                                 // On affiche LUMIN_HIGH
        Serial.println(F("Rentrez la nouvelle valeur pour LUMIN_HIGH")); // On demande la nouvelle valeur pour LUMIN_HIGH
        ReadandConvert();                                                // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= 0 && valeurEnterInt <= 1023)               // Si la valeur n'est pas vide et est comprise entre 0 et 1023
        {
            Serial.print("LUMIN_HIGH mis à jour à la valeur "); // On informe l’utilisateur que la valeur a été mise à jour
            Serial.println(valeurEnterInt);                     // On affiche la valeur entrée
            setup();                                            // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre 0 et 1023
        {
            Serial.println(F("Valeur incorrecte -> "));                               // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                           // On affiche la valeur entrée
            Serial.println(F("Veuillez entrer une valeur comprise entre 0 et 1023")); // On affiche un message d’information
            setup();                                                                  // On relance le mode
        }
    }

    else if (commandeEnter == "TEMP_AIR") // Si la commande est TEMP_AIR
    {
        Serial.println(F("TEMP_AIR"));                                 // On affiche TEMP_AIR
        Serial.println(F("Rentrez la nouvelle valeur pour TEMP_AIR")); // On demande la nouvelle valeur pour TEMP_AIR
        ReadandConvert();                                              // On appelle la fonction ReadandConvert
        switch (valeurEnterInt)                                        // On vérifie la valeur entrée
        {
            {
            case 0:                                                                // Si la valeur est 0
                Serial.println(F("Capteur de température désactivé avec succès")); // On informe l’utilisateur que la valeur a été mise à jour
                setup();                                                           // On relance le mode
                break;
            case 1:                                                             // Si la valeur est 1
                Serial.println(F("Capteur de température activé avec succès")); // On informe l’utilisateur que la valeur a été mise à jour
                setup();                                                        // On relance le mode
                break;
            default:                                                // Si la valeur est autre chose que 0 ou 1
                Serial.println(F("Valeur incorrecte -> "));         // On affiche un message d’erreur
                Serial.println(valeurEnterInt);                     // On affiche la valeur entrée
                Serial.println(F("Veuillez choisir entre 0 et 1")); // On affiche un message d’erreur
                setup();                                            // On relance le mode
                break;
            }
        }
    }

    else if (commandeEnter == "MIN_TEMP_AIR") // Si la commande est MIN_TEMP_AIR
    {
        Serial.println(F("MIN_TEMP_AIR"));                                 // On affiche MIN_TEMP_AIR
        Serial.println(F("Rentrez la nouvelle valeur pour MIN_TEMP_AIR")); // On demande la nouvelle valeur pour MIN_TEMP_AIR
        ReadandConvert();                                                  // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= -40 && valeurEnterInt <= 85)                 // Si la valeur n'est pas vide et est comprise entre -40 et 85
        {
            Serial.print(F("MIN_TEMP_AIR mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a été mise à jour
            Serial.println(valeurEnterInt);                          // On affiche la valeur entrée
            setup();                                                 // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre -40 et 85
        {
            Serial.println(F("Valeur incorrecte -> "));                               // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                           // On affiche la valeur entrée
            Serial.println(F("Veuillez entrer une valeur comprise entre -40 et 85")); // On affiche un message d’information
            setup();                                                                  // On relance le mode
        }
    }

    else if (commandeEnter == "MAX_TEMP_AIR") // Si la commande est MAX_TEMP_AIR
    {
        Serial.println(F("MAX_TEMP_AIR"));                                 // On affiche MAX_TEMP_AIR
        Serial.println(F("Rentrez la nouvelle valeur pour MAX_TEMP_AIR")); // On demande la nouvelle valeur pour MAX_TEMP_AIR
        ReadandConvert();                                                  // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= -40 && valeurEnterInt <= 85)                 // Si la valeur n'est pas vide et est comprise entre -40 et 85
        {
            Serial.print(F("MAX_TEMP_AIR mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a été mise à jour
            Serial.println(valeurEnterInt);                          // On affiche la valeur entrée
            setup();                                                 // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre -40 et 85
        {
            Serial.println(F("Valeur incorrecte -> "));                               // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                           // On affiche la valeur entrée
            Serial.println(F("Veuillez entrer une valeur comprise entre -40 et 85")); // On affiche un message d’information
            setup();                                                                  // On relance le mode
        }
    }

    else if (commandeEnter == "HYGR") // Si la commande est HYGR
    {
        Serial.println(F("HYGR"));                                 // On affiche HYGR
        Serial.println(F("Rentrez la nouvelle valeur pour HYGR")); // On demande la nouvelle valeur pour HYGR
        ReadandConvert();                                          // On appelle la fonction ReadandConvert
        switch (valeurEnterInt)                                    // On vérifie la valeur entrée
        {
            {
            case 0:                                                            // Si la valeur est 0
                Serial.println(F("Capteur d'humidité désactivé avec succès")); // On informe l’utilisateur que la valeur a été mise à jour
                setup();                                                       // On relance le mode
                break;
            case 1:                                                         // Si la valeur est 1
                Serial.println(F("Capteur d'humidité activé avec succès")); // On informe l’utilisateur que la valeur a été mise à jour
                setup();                                                    // On relance le mode
                break;
            default:                                                // Si la valeur est autre chose que 0 ou 1
                Serial.println(F("Valeur incorrecte -> "));         // On affiche un message d’erreur
                Serial.println(valeurEnterInt);                     // On affiche la valeur entrée
                Serial.println(F("Veuillez choisir entre 0 et 1")); // On affiche un message d’erreur
                setup();                                            // On relance le mode
                break;
            }
        }
    }

    else if (commandeEnter == "HYGR_MINT") // Si la commande est HYGR_MINT
    {
        Serial.println(F("HYGR_MINT"));                                 // On affiche HYGR_MINT
        Serial.println(F("Rentrez la nouvelle valeur pour HYGR_MINT")); // On demande la nouvelle valeur pour HYGR_MINT
        ReadandConvert();                                               // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= -45 && valeurEnterInt <= 85)              // Si la valeur n'est pas vide et est comprise entre -45 et 85
        {
            Serial.print(F("HYGR_MINT mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a été mise à jour
            Serial.println(valeurEnterInt);                       // On affiche la valeur entrée
            setup();                                              // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre -45 et 85
        {
            Serial.println(F("Valeur incorrecte -> "));                               // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                           // On affiche la valeur entrée
            Serial.println(F("Veuillez entrer une valeur comprise entre -45 et 80")); // On affiche un message d’information
            setup();                                                                  // On relance le mode
        }
    }

    else if (commandeEnter == "HYGR_MAXT") // Si la commande est HYGR_MAXT
    {
        Serial.println(F("HYGR_MAXT"));                                 // On affiche HYGR_MAXT
        Serial.println(F("Rentrez la nouvelle valeur pour HYGR_MAXT")); // On demande la nouvelle valeur pour HYGR_MAXT
        ReadandConvert();                                               // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= -45 && valeurEnterInt <= 85)              // Si la valeur n'est pas vide et est comprise entre -45 et 85
        {
            Serial.print(F("HYGR_MAXT mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a été mise à jour
            Serial.println(valeurEnterInt);                       // On affiche la valeur entrée
            setup();                                              // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre -45 et 85
        {
            Serial.println(F("Valeur incorrecte -> "));                               // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                           // On affiche la valeur entrée
            Serial.println(F("Veuillez entrer une valeur comprise entre -45 et 80")); // On affiche un message d’information
            setup();                                                                  // On relance le mode
        }
    }

    else if (commandeEnter == "PRESSURE") // Si la commande est PRESSURE
    {
        Serial.println(F("PRESSURE"));                                 // On affiche PRESSURE
        Serial.println(F("Rentrez la nouvelle valeur pour PRESSURE")); // On demande la nouvelle valeur pour PRESSURE
        ReadandConvert();                                              // On appelle la fonction ReadandConvert
        switch (valeurEnterInt)                                        // On vérifie la valeur entrée
        {
            {
            case 0:                                                             // Si la valeur est 0
                Serial.println(F("Capteur de pression désactivé avec succès")); // On informe l’utilisateur que la valeur a été mise à jour
                setup();                                                        // On relance le mode
                break;
            case 1:                                                          // Si la valeur est 1
                Serial.println(F("Capteur de pression activé avec succès")); // On informe l’utilisateur que la valeur a été mise à jour
                setup();                                                     // On relance le mode
                break;
            default:                                                // Si la valeur est autre chose que 0 ou 1
                Serial.println(F("Valeur incorrecte -> "));         // On affiche un message d’erreur
                Serial.println(valeurEnterInt);                     // On affiche la valeur entrée
                Serial.println(F("Veuillez choisir entre 0 et 1")); // On affiche un message d’erreur
                setup();                                            // On relance le mode
                break;
            }
        }
    }

    else if (commandeEnter == "PRESSURE_MIN") // Si la commande est PRESSURE_MIN
    {
        Serial.println(F("PRESSURE_MIN"));                                 // On affiche PRESSURE_MIN
        Serial.println(F("Rentrez la nouvelle valeur pour PRESSURE_MIN")); // On demande la nouvelle valeur pour PRESSURE_MIN
        ReadandConvert();                                                  // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= 300 && valeurEnterInt <= 1100)               // Si la valeur n'est pas vide et est comprise entre 300 et 1100
        {
            Serial.print(F("PRESSURE_MIN mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a été mise à jour
            Serial.println(valeurEnterInt);                          // On affiche la valeur entrée
            setup();                                                 // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre 300 et 1100
        {
            Serial.println(F("Valeur incorrecte -> "));                                 // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                             // On affiche la valeur entrée
            Serial.println(F("Veuillez entrer une valeur comprise entre 300 et 1100")); // On affiche un message d’information
            setup();                                                                    // On relance le mode
        }
    }

    else if (commandeEnter == "PRESSURE_MAX") // Si la commande est PRESSURE_MAX
    {
        Serial.println(F("PRESSURE_MAX"));                                 // On affiche PRESSURE_MAX
        Serial.println(F("Rentrez la nouvelle valeur pour PRESSURE_MAX")); // On demande la nouvelle valeur pour PRESSURE_MAX
        ReadandConvert();                                                  // On appelle la fonction ReadandConvert
        if (valeurEnterInt >= 300 && valeurEnterInt <= 1100)               // Si la valeur n'est pas vide et est comprise entre 300 et 1100
        {
            Serial.print(F("PRESSURE_MAX mis à jour à la valeur ")); // On informe l’utilisateur que la valeur a été mise à jour
            Serial.println(valeurEnterInt);                          // On affiche la valeur entrée
            setup();                                                 // On relance le mode
        }
        else // Si la valeur est vide ou si la valeur n'est pas comprise entre 300 et 1100
        {
            Serial.println(F("Valeur incorrecte -> "));                                 // On affiche un message d’erreur
            Serial.println(valeurEnterInt);                                             // On affiche la valeur entrée
            Serial.println(F("Veuillez entrer une valeur comprise entre 300 et 1100")); // On affiche un message d’information
            setup();                                                                    // On relance le mode
        }
    }

    else if (commandeEnter == "CLOCK") // Si la commande est CLOCK
    {
        int heureEnter;   // Variable pour stocker l'heure entrée
        int minuteEnter;  // Variable pour stocker les minutes entrées
        int secondeEnter; // Variable pour stocker les secondes entrées

        Serial.println(F("CLOCK"));              // On affiche CLOCK
        Serial.println(F("Rentrez l'heure"));    // On demande l'heure
        heureEnter = ReadandConvert();           // On appelle la fonction ReadandConvert et on stocke la valeur entrée dans la variable heureEnter
        if (heureEnter >= 0 && heureEnter <= 23) // Si l'heure est comprise entre 0 et 23
        {
            Serial.println(F("Rentrez les minutes"));  // On demande les minutes
            minuteEnter = ReadandConvert();            // On appelle la fonction ReadandConvert et on stocke la valeur entrée dans la variable minuteEnter
            if (minuteEnter >= 0 && minuteEnter <= 59) // Si les minutes sont comprises entre 0 et 59
            {
                Serial.println(F("Rentrez les secondes"));   // On demande les secondes
                secondeEnter = ReadandConvert();             // On appelle la fonction ReadandConvert et on stocke la valeur entrée dans la variable secondeEnter
                if (secondeEnter >= 0 && secondeEnter <= 59) // Si les secondes sont comprises entre 0 et 59
                {
                    Serial.print(F("L'heure a été mise à jour à ")); // On informe l’utilisateur que la valeur a été mise à jour
                    Serial.print(heureEnter);                        // On affiche l'heure entrée
                    Serial.print(F(":"));                            // On affiche un :
                    Serial.print(minuteEnter);                       // On affiche les minutes entrées
                    Serial.print(F(":"));                            // On affiche un :
                    Serial.println(secondeEnter);                    // On affiche les secondes entrées
                    setup();                                         // On relance le mode
                }
                else // Si les secondes ne sont pas comprises entre 0 et 59
                {
                    Serial.println(F("Valeur incorrecte -> "));                             // On affiche un message d’erreur
                    Serial.println(secondeEnter);                                           // On affiche la valeur entrée
                    Serial.println(F("Veuillez entrer une valeur comprise entre 0 et 59")); // On affiche un message d’information
                    setup();                                                                // On relance le mode
                }
            }
            else // Si les minutes ne sont pas comprises entre 0 et 59
            {
                Serial.println(F("Valeur incorrecte -> "));                             // On affiche un message d’erreur
                Serial.println(minuteEnter);                                            // On affiche la valeur entrée
                Serial.println(F("Veuillez entrer une valeur comprise entre 0 et 59")); // On affiche un message d’information
                setup();
            }
        }
        else // Si l'heure n'est pas comprise entre 0 et 23
        {
            Serial.println(F("Valeur incorrecte -> "));                             // On affiche un message d’erreur
            Serial.println(heureEnter);                                             // On affiche la valeur entrée
            Serial.println(F("Veuillez entrer une valeur comprise entre 0 et 23")); // On affiche un message d’information
            setup();                                                                // On relance le mode
        }
    }

    else if (commandeEnter == "DATE") // Si la commande est DATE
    {
        int moisEnter;  // Variable pour stocker le mois entré
        int jourEnter;  // Variable pour stocker le jour entré
        int anneeEnter; // Variable pour stocker l'année entrée

        Serial.println(F("DATE"));             // On affiche DATE
        Serial.println(F("Rentrez le mois"));  // On demande le mois
        moisEnter = ReadandConvert();          // On appelle la fonction ReadandConvert et on stocke la valeur entrée dans la variable moisEnter
        if (moisEnter >= 1 && moisEnter <= 12) // Si le mois est compris entre 1 et 12
        {
            Serial.println(F("Rentrez le jour"));  // On demande le jour
            jourEnter = ReadandConvert();          // On appelle la fonction ReadandConvert et on stocke la valeur entrée dans la variable jourEnter
            if (jourEnter >= 1 && jourEnter <= 31) // Si le jour est compris entre 1 et 31
            {
                Serial.println(F("Rentrez l'année"));    // On demande l'année
                anneeEnter = ReadandConvert();           // On appelle la fonction ReadandConvert et on stocke la valeur entrée dans la variable anneeEnter
                if (anneeEnter >= 0 && anneeEnter <= 99) // Si l'année est comprise entre 0 et 99
                {
                    Serial.print(F("La date a été mise à jour à ")); // On informe l’utilisateur que la valeur a été mise à jour
                    Serial.print(jourEnter);                         // On affiche le jour entré
                    Serial.print(F("/"));                            // On affiche un /
                    Serial.print(moisEnter);                         // On affiche le mois entré
                    Serial.print(F("/"));                            // On affiche un /
                    Serial.println(anneeEnter);                      // On affiche l'année entrée
                    setup();                                         // On relance le mode
                }
                else // Si l'année n'est pas comprise entre 0 et 99
                {
                    Serial.println(F("Valeur incorrecte -> "));                             // On affiche un message d’erreur
                    Serial.println(anneeEnter);                                             // On affiche la valeur entrée
                    Serial.println(F("Veuillez entrer une valeur comprise entre 0 et 99")); // On affiche un message d’information
                    setup();                                                                // On relance le mode
                }
            }
            else // Si le jour n'est pas compris entre 1 et 31
            {
                Serial.println(F("Valeur incorrecte -> "));                             // On affiche un message d’erreur
                Serial.println(jourEnter);                                              // On affiche la valeur entrée
                Serial.println(F("Veuillez entrer une valeur comprise entre 1 et 31")); // On affiche un message d’ information
                setup();                                                                // On relance le mode
            }
        }
        else // Si le mois n'est pas compris entre 1 et 12
        {
            Serial.println(F("Valeur incorrecte -> "));                             // On affiche un message d’erreur
            Serial.println(moisEnter);                                              // On affiche la valeur entrée
            Serial.println(F("Veuillez entrer une valeur comprise entre 1 et 12")); // On affiche un message d’information
            setup();                                                                // On relance le mode
        }
    }

    else if (commandeEnter == "DAY") // Si la commande est DAY
    {
        Serial.println(F("DAY"));                           // On affiche DAY
        Serial.println(F("Rentrez le jour de la semaine")); // On demande le jour de la semaine
        while (Serial.available() == 0)                     // Tant que rien n’est entré dans la console on attend
        {
        }
        String jourSemaineEnter = Serial.readString();                                                                                                                                                               // On stocke le jour de la semaine entré dans la variable jourSemaineEnter
        if (jourSemaineEnter == "MON" || jourSemaineEnter == "TUE" || jourSemaineEnter == "WED" || jourSemaineEnter == "THU" || jourSemaineEnter == "FRI" || jourSemaineEnter == "SAT" || jourSemaineEnter == "SUN") // Si le jour de la semaine entré est égal à l'un des jours de la semaine
        {
            Serial.print(F("Le jour de la semaine a été mis à jour à ")); // On informe l’utilisateur que la valeur a été mise à jour
            Serial.println(jourSemaineEnter);                             // On affiche le jour de la semaine entré
            setup();                                                      // On relance le mode
        }
        else // Si le jour de la semaine entré n'est pas égal à l'un des jours de la semaine
        {
            Serial.println(F("Valeur incorrecte -> ")); // On affiche un message d’erreur
            Serial.println(jourSemaineEnter);           // On affiche la valeur entrée
            setup();                                    // On relance le mode
        }
    }

    else if (commandeEnter == "HELP") //  Si la commande est HELP
    {
        Serial.println(F("HELP"));                                                                                                                      // On affiche HELP
        Serial.println(F("LOG_INTERVALL : définition de l’intervalle entre 2 mesures."));                                                               // On affiche la description de la commande LOG_INTERVALL
        Serial.println(F("FILE_MAX_SIZE : définition de la taille maximale (en octets) d'un fichier de log."));                                         // On affiche la description de la commande FILE_MAX_SIZE
        Serial.println(F("RESET : réinitialisation de l’ensemble des paramètres à leurs valeurs par défaut."));                                         // On affiche la description de la commande RESET
        Serial.println(F("VERSION : affiche la version du programme et un numéro de lot."));                                                            // On affiche la description de la commande VERSION
        Serial.println(F("TIMEOUT : durée (en s) au bout de laquelle l’acquisition des données d’un capteur est abandonnée."));                         // On affiche la description de la commande TIMEOUT
        Serial.println(F("LUMIN : définition de l’activation (1) ou la désactivation (0) du capteur de luminosité"));                                   // On affiche la description de la commande LUMIN
        Serial.println(F("LUMIN_LOW : définition de la valeur en dessous de laquelle la luminosité est considérée comme faible"));                      // On affiche la description de la commande LUMIN_LOW
        Serial.println(F("LUMIN_HIGH : définition de la valeur au-dessus de laquelle la luminosité est considérée comme forte"));                       // On affiche la description de la commande LUMIN_HIGH
        Serial.println(F("TEMP_AIR : définition de l’activation (1) ou la désactivation (0) du capteur de température de l'air"));                      // On affiche la description de la commande TEMP_AIR
        Serial.println(F("MIN_TEMP_AIR : définition du seuil de température de l'air (en °C) en dessous duquel le capteur se mettra en erreur."));      // On affiche la description de la commande MIN_TEMP_AIR
        Serial.println(F("MAX_TEMP_AIR : définition du seuil de température de l'air (en °C) au-dessus duquel le capteur se mettra en erreur."));       // On affiche la description de la commande MAX_TEMP_AIR
        Serial.println(F("HYGR : définition de l’activation (1) ou la désactivation (0) du capteur d’hygrométrie"));                                    // On affiche la description de la commande HYGR
        Serial.println(F("HYGR_MINT : définition de la température en dessous de laquelle les mesures d’hygrométrie ne seront pas prises en compte.")); // On affiche la description de la commande HYGR_MINT
        Serial.println(F("HYGR_MAXT : définition de la température au-dessus de laquelle les mesures d’hygrométrie ne seront pas prises en compte."));  // On affiche la description de la commande HYGR_MAXT
        Serial.println(F("PRESSURE : définition de l’activation (1) ou la désactivation (0) du capteur de pression atmosphérique."));                   // On affiche la description de la commande PRESSURE
        Serial.println(F("PRESSURE_MIN : définition du seuil de pression atmosphérique (en HPa) en dessous duquel le capteur se mettra en erreur."));   // On affiche la description de la commande PRESSURE_MIN
        Serial.println(F("PRESSURE_MAX : définition du seuil de pression atmosphérique (en HPa) au-dessus duquel le capteur se mettra en erreur."));    // On affiche la description de la commande PRESSURE_MAX
        Serial.println(F("CLOCK : configuration de l’heure du jour au format HEURE{0-23}:MINUTE{0-59}:SECONDE{0-59}"));                                 // On affiche la description de la commande CLOCK
        Serial.println(F("DATE : configuration de la date du jour au format MOIS{1-12},JOUR{1-31},ANNEE{2000-2099}"));                                  // On affiche la description de la commande DATE
        Serial.println(F("DAY : Configuration du jour de la semaine{MON,TUE,WED,THU,FRI,SAT,SUN}"));                                                    // On affiche la description de la commande DAY
        setup();
    }

    else
    {
        Serial.print(F("Commande "));
        Serial.print(commandeEnter);
        Serial.print(F(" inconnue \n"));
        setup();
    }
}

int ReadandConvert()
{
    while (Serial.available() == 0) // Tant que rien n’est entré dans la console on attend
    {
    }
    valeurEnterString = Serial.readString();    // On stocke la valeur entrée dans la variable valeurEnterString
    valeurEnterString.trim();                   // On supprime les espaces avant et après la valeur entrée
    valeurEnterInt = valeurEnterString.toInt(); // On convertit la valeur entrée en int
    return valeurEnterInt;                      // On retourne la valeur entrée en int
}