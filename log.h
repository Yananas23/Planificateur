#ifndef LOG_H
#define LOG_H


/**
    \file log.h


    \brief Gère les logs du planificateur

    \author Boulogne Yanis
    \version 1
    \date Mai 2024
*/


/**
    \brief Crée un nouveau fichier de log et renvoie un pointeur vers celui-ci.

    \return char* : le nom du fichier créé
*/
char* create_log();


/**
    \brief Ouvre un fichier de log en mode append.

    \param log : char* : le nom du fichier de log

    \return FILE* : un pointeur vers le fichier ouvert
*/
FILE* open_log(char* log);


/**
    \brief Parse le code d'une erreur.

    \param line : char* : la ligne à parser

    \return int : le code attribué à une erreur
*/
int parse_code(char* line);


/**
    \brief Récupère le message d'erreur correspondant à un code d'erreur.

    \param error_code : int : le code d'erreur

    \return char* : le message d'erreur
*/
char *get_error(int error_code);


/**
    \brief Ajoute un message en cas d'erreur.

    \param log : FILE* : le fichier de log
    \param code : int : le code de l'erreur
    \param message : char* : le message à ajouter
*/
void error_log(char* log, int code, char* message);


/**
    \brief Ajoute la date de 1ère éxécution.

    \param log : FILE* : le fichier de log
    \param d : Date : Une structure date avec les informations de 1ère exécution
*/
void date_log(char *log, Date d);


/**
    \brief Ajoute une ligne pour prévenir de la répétition infini.

    \param log : FILE* : le fichier de log
*/
void infini_log(char *log);


/**
    \brief Ecris la commande qui va être éxécutée et ses informations.

    \param log : FILE* : le fichier de log
    \param cmd : Command : la structure contenant la commande
    \param nb_cmd : int : le numéro de la commande
*/
void command(char *log, Command cmd, int nb_cmd);


/**
    \brief Réécris le commentaire présent dans le fichier d'input.

    \param log : FILE* : le fichier de log
    \param comm : char* : le commentaire de l'utilisateur
*/
void commentaire(char *log, char *comm);


/**
    \brief Passe une ligne.

    \param log : FILE* : le fichier de log
*/
void space_line(char *log);


/**
    \brief Ajoute le nom du fichier avec les commandes à lire.


    \param log : FILE* : le fichier de log
    \param file_path : char* : le chemin du fichier d'input
*/
void file_log(char *log, char *file_path);


/**
    \brief Annonce le nombre d'éxécution d'une commande.

    \param log : FILE* : le fichier de log
    \param iteration : int : le nombre de fois que la commande a été éxécutée
    \param cmd : Command : La commande en cours d'éxécution
*/
void execute_log(char *log, int iteration, Command cmd);


/**
    \brief Annonce la fin d'éxécution d'une commande.

    \param log : FILE* : le fichier de log
    \param cmd : Command : La commande éxécutée
*/
void end_execute(char *log, Command cmd);


#endif