/*Ce mode permet de configurer le système grâce à une interaction depuis une console sur l’interface série
du microcontrôleur (UART).
Depuis l'interface série on pourra taper des commandes de configuration pour modifier les valeurs des
paramètres enregistrés dans l’EEPROM :
LOG_INTERVALL=10 -> définition de l’intervalle entre 2 mesures, 10 minutes par défaut.
FILE_MAX_SIZE=4096 -> définition de la taille maximale (en octets) d'un fichier de log, une taille de 4ko
provoque son archivage.
RESET -> réinitialisation de l’ensemble des paramètres à leurs valeurs par défaut.
VERSION -> affiche la version du programme et un numéro de lot (permet de tracer la production)
Pour les différents capteurs:
TIMEOUT=30 -> durée (en s) au bout de laquelle l’acquisition des données d’un capteur est abandonnée.
Après 2 mesures en timeout, le capteur est signalé en erreur.*/
#include "mode_config.h"
#include "config.h"
#include "eeprom.h"
#include "uart.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "version.h"
#include "log.h"

// Variables globales
char buffer[100]; // Buffer de réception de la console
char *token; // Pointeur sur le token courant
char *token2; // Pointeur sur le token suivant
char *token3; // Pointeur sur le token suivant
char *token4; // Pointeur sur le token suivant
char *token5; // Pointeur sur le token suivant
char *token6; // Pointeur sur le token suivant
char *token7; // Pointeur sur le token suivant
char *token8; // Pointeur sur le token suivant
char *token9; // Pointeur sur le token suivant
char *token10; // Pointeur sur le token suivant
char *token11; // Pointeur sur le token suivant
char *token12; // Pointeur sur le token suivant
char *token13; // Pointeur sur le token suivant

// Fonction de configuration
void mode_config(void)
{
    // Initialisation de la console
    uart_init();
    // Affichage du message de bienvenue
    uart_puts("Configuration du systeme de mesure\r \n");
    uart_puts("Version ");
    uart_puts(VERSION);
    uart_puts(" Lot ");
    uart_puts(LOT);
    uart_puts("\r \n");
    uart_puts("Taper HELP pour afficher l'aide\r \n");
    // Boucle de configuration
    while(1)
    {
        // Affichage du prompt
        uart_puts(">");
        // Attente de la saisie d'une commande
        uart_gets(buffer, sizeof(buffer));
        // Découpage de la commande en tokens
        token = strtok(buffer, " =");
        // Traitement de la commande
        if (strcmp(token, "LOG_INTERVALL") == 0)
        {
            // Récupération de la valeur
            token2 = strtok(NULL, " =");
            // Conversion en entier
            int log_intervall = atoi(token2);
            // Vérification de la valeur
            if (log_intervall > 0)
            {
                // Enregistrement de la valeur
                eeprom_write_log_intervall(log_intervall);
                // Affichage de la valeur
                uart_puts("LOG_INTERVALL=");
                uart_puts(token2);
                uart_puts("\r \n");
            }
            else
            {
                // Affichage d'un message d'erreur
                uart_puts("Valeur incorrecte\r \n");
            }
        }
        else if (strcmp(token, "FILE_MAX_SIZE") == 0)
        {
            // Récupération de la valeur
            token2 = strtok(NULL, " =");
            // Conversion en entier
            int file_max_size = atoi(token2);
            // Vérification de la valeur
            if (file_max_size > 0)
            {
                // Enregistrement de la valeur
                eeprom_write_file_max_size(file_max_size);
                // Affichage de la valeur
                uart_puts("FILE_MAX_SIZE=");
                uart_puts(token2);
                uart_puts("\r \n");
            }
            else
            {
                // Affichage d'un message d'erreur
                uart_puts("Valeur incorrecte\r \n");
            }
        }
        else if (strcmp(token, "RESET") == 0)
        {
            // Réinitialisation des paramètres
            eeprom_write_log_intervall(LOG_INTERVALL_DEFAULT);
            eeprom_write_file_max_size(FILE_MAX_SIZE_DEFAULT);
            eeprom_write_timeout(TIMEOUT_DEFAULT);
            // Affichage d'un message de confirmation
            uart_puts("Parametres reinitialises\r \n");
        }
        else if (strcmp(token, "VERSION") == 0)
        {
            // Affichage de la version
            uart_puts("Version ");
            uart_puts(VERSION);
            uart_puts(" Lot ");
            uart_puts(LOT);
            uart_puts("\r \n");
        }
        else if (strcmp(token, "HELP") == 0)
        {
            // Affichage de l'aide
            uart_puts("LOG_INTERVALL=10 -> definition de l'intervalle entre 2 mesures, 10 minutes par defaut\r \n");
            uart_puts("FILE_MAX_SIZE=4096 -> definition de la taille maximale (en octets) d'un fichier de log, une taille de 4ko provoque son archivage\r \n");
            uart_puts("RESET -> reinitialisation de l'ensemble des parametres a leurs valeurs par defaut\r \n");
            uart_puts("VERSION -> affiche la version du programme et un numero de lot (permet de tracer la production)\r \n");
            uart_puts("TIMEOUT=30 -> duree (en s) au bout de laquelle l'acquisition des donnees d'un capteur est abandonnee. Apres 2 mesures en timeout, le capteur est signale en erreur\r \n");
        }
        else if (strcmp(token, "TIMEOUT") == 0)
        {
            // Récupération de la valeur
            token2 = strtok(NULL, " =");
            // Conversion en entier
            int timeout = atoi(token2);
            // Vérification de la valeur
            if (timeout > 0)
            {
                // Enregistrement de la valeur
                eeprom_write_timeout(timeout);
                // Affichage de la valeur
                uart_puts("TIMEOUT=");
                uart_puts(token2);
                uart_puts("\r \n");
            }
            else
            {
                // Affichage d'un message d'erreur
                uart_puts("Valeur incorrecte\r \n");
            }
        }
        else
        {
            // Affichage d'un message d'erreur
            uart_puts("Commande inconnue\r \n");
        }
    }
}

