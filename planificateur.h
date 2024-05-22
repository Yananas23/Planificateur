#ifndef PLANIFICATEUR_H
#define PLANIFICATEUR_H


/**
    \file planificateur.h


    \brief Planificateur de tâches en ligne de commande

    \author Ghoul Massinissa / Boulogne Yanis / Martin Maxence
    \version 1
    \date Avril 2024
*/


typedef struct Date{
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
    long int timestamp;
} Date;


/**
    \struct Command
    \brief Structure pour stocker les informations de la commande saisie par l'utilisateur

    Command est une structure contenant la commande, l'argument, le délai,
    le nombre d'itérations et la date de première exécution si besoin
*/
typedef struct Command{
    char command[100];
    char argument[100];
    int iterations;
    long int delay;
    Date start;
} Command;


typedef struct CommandList{
    Command *tasks;
    int nb_cmd;
} CommandList;




/**
    \brief : exécute la commande saisie par l'utilisateur avec l'argument, le délai et le nombre d'itérations spécifiés

    \param command : command : une chaîne de caractères contenant la commande à exécuter
    \param argument : argument : une chaîne de caractères contenant l'argument à passer à la commande
    \param delay : delay : un entier contenant le délai entre chaque itération
    \param iterations : iterations : un entier contenant le nombre d'itérations à exécuter
    \param file_name : file_name : une chaîne de caractères contenant le nom du fichier de logs

    \remark : La fonction utilise la fonction execl() pour exécuter la commande et vérifie que l'exécution est réussie

    \return void : ne renvoie rien
 */
void execute_command_aux(Command cmd, char *file_name);


/**
    \brief : Crée un nouveau processus et exécute la commande saisie par l'utilisateur avec l'argument, le délai et le nombre d'itérations spécifiés dans ce processus.

    \param command : command : une chaîne de caractères contenant la commande à exécuter
    \param argument : argument : une chaîne de caractères contenant l'argument à passer à la commande
    \param delay : delay : un entier contenant le délai entre chaque itération
    \param iterations : iterations : un entier contenant le nombre d'itérations à exécuter
    \param log : log : une chaîne de caractères contenant le nom du fichier de logs

    \remark : La fonction utilise la fonction fork() pour créer un nouveau processus et appelle execute_command_aux() dans ce processus pour exécuter la commande.

    \return void : ne renvoie rien
*/
void execute_command(CommandList cmd,  char *file_name);



/**
 * @brief Vérifie si le fichier de fin de boucle existe.
 *
 * @param path : le chemin du dossier ou trouver le .tmp
 * @return int : 0 si le fichier existe, 1 sinon
 */
int check_end_file(char *path);



#endif