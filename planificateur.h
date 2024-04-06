#ifndef PLANIFICATEUR_H
#define PLANIFICATEUR_H


/**
    \file planificateur.h


    \brief Planificateur de tâches en ligne de commande

    \author Ghoul Massinissa / Boulogne Yanis / Martin Maxence
    \version 1
    \date Avril 2024
*/


/**
    \struct Command
    \brief Structure pour stocker les informations de la commande saisie par l'utilisateur

    Command est une structure contenant la commande, l'argument, le délai et le nombre d'itérations
*/
typedef struct {
    char command[10];
    char argument[100];
    int delay;
    int iterations;
} Command;


/**
    \brief : lit l'entrée utilisateur pour la commande, l'argument, le délai et le nombre d'itérations

    \param command : command : une chaîne de caractères pour stocker la commande saisie par l'utilisateur
    \param argument : argument : une chaîne de caractères pour stocker l'argument saisi par l'utilisateur
    \param delay : delay : un pointeur vers un entier pour stocker le délai saisi par l'utilisateur
    \param iterations : iterations : un pointeur vers un entier pour stocker le nombre d'itérations saisi par l'utilisateur
    \param file_name : file_name : une chaîne de caractères pour stocker le nom du fichier de logs

    \remark : La fonction vérifie que l'entrée utilisateur est valide avant de stocker les valeurs dans les paramètres

    \return void : ne renvoie rien
 */
void read_user_input(char *command, char *argument, int *delay, int *iterations, char *file_name);


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
void execute_command(char *command, char *argument, int delay, int iterations, char *file_name);


/**
 * @brief Crée un nouveau fichier de log et renvoie un pointeur vers celui-ci.
 *
 * @return char* : le nom du fichier créé
 */
char* create_file();


/**
 * @brief Vérifie si le fichier de fin de boucle existe.
 *
 * @param path : le chemin du dossier ou trouver le .tmp
 * @return int : 0 si le fichier existe, 1 sinon
 */
int check_end_file(char *path);


/**
 * @brief Lit le fichier d'entrée et exécute les commandes qui y sont contenues.
 *
 * @param file_path : le chemin du fichier d'entrée
 * @param file_name : une chaîne de caractères pour stocker le nom du fichier de logs
 */
void read_file_input(char *file_path, char *file_name);


/**
 * @brief Parse une ligne de commande et stocke les informations dans une structure Command.
 *
 * @param line : la ligne de commande à parser
 * @param cmd : un pointeur vers une structure Command pour stocker les informations
 * @return int : 1 si la ligne a été correctement parsée, 0 sinon
 */
int parse(const char *line, Command *cmd);



#endif