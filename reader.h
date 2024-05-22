#ifndef READER_H
#define READER_H


/**
    \file reader.h


    \brief Lis les fichiers d'entrée et les commandes

    \author Boulogne Yanis
    \version 1
    \date Mai 2024
*/


/**
    \brief Formate une date en une structure Date.

    \param date : char* : une chaîne de caractères contenant la date à formater
    \param d : Date : une structure Date pour stocker les informations

    \return long int : un entier long contenant le timestamp de la date
*/
int formatage(char *date, Date d);


/**
    \brief Convertit une date en timestamp.

    \param date : Date : une structure Date contenant la date à convertir

    \return long int : un entier long contenant le timestamp de la date
*/
long int timestamps(Date date);


/**
    @brief Lit le fichier d'entrée et exécute les commandes qui y sont contenues.

    \param cmd : Command : une structure Command pour stocker les informations
    \param file_path : Char* : le chemin du fichier d'entrée
    \param log : char* : une chaîne de caractères pour stocker le nom du fichier de logs

    \return CommandList : renvoie une liste de commandes
*/
CommandList read_file(Command cmd, char *file_path, char *log);


/**
    \brief Ouvre un fichier en mode append.

    \param file_path : char* : le chemin du fichier à ouvrir

    \return FILE* : un pointeur vers le fichier ouvert
*/
FILE *open_file(char *file_path);


/**
    \brief Parse une ligne de commande et stocke les informations dans une structure Command.

    \param line : la ligne de commande à parser
    \param cmd : un pointeur vers une structure Command pour stocker les informations
    \param log : un pointeur vers le fichier de log

*/
void parse_cmd(const char *line, Command *cmd, char* log);


/**
    \brief Parse une date et stocke les informations dans une structure dat elle même stocker dans une structure Command.

    \param line : la ligne de commande à parser
    \param cmd : un pointeur vers une structure Command pour stocker les informations
    \param log : un pointeur vers le fichier de log

*/
void parse_date(const char *line, Command *cmd, char* log);


/**
    \brief : lit l'entrée utilisateur pour la commande, l'argument, le délai et le nombre d'itérations

    \param cmd : Command : une structure Command pour stocker les informations

    \remark : La fonction vérifie que l'entrée utilisateur est valide avant de stocker les valeurs dans les paramètres

    \return CommandList : renvoie une liste de commandes
*/
CommandList read_user(Command cmd, char *file_name);


/**
    \brief Demande à l'utilisateur de rentrer une commande.

    \param cmd : Command : une structure Command pour stocker les informations
    \param log : char* : une chaîne de caractères pour stocker le nom du fichier de logs
*/
void ask_command(Command cmd, char *log);


/**
    \brief Demande à l'utilisateur de rentrer un argument.

    \param cmd : Command : une structure Command pour stocker les informations
    \param log : char* : une chaîne de caractères pour stocker le nom du fichier de logs
*/
void ask_arguments(Command cmd, char *log);


/**
    \brief Demande à l'utilisateur de rentrer un nombre d'itération.
    \remark : si la valeur est inférieure à 0, le nombre d'itération est infini.

    \param cmd : Command : une structure Command pour stocker les informations
    \param log : char* : une chaîne de caractères pour stocker le nom du fichier de logs
*/
void ask_iterations(Command cmd, char *log);


/**
    \brief Demande à l'utilisateur de rentrer un delai d'éxécution.

    \param cmd : Command : une structure Command pour stocker les informations
    \param log : char* : une chaîne de caractères pour stocker le nom du fichier de logs
*/
void ask_delay(Command cmd, char *log);


/**
    \brief Demande à l'utilisateur de rentrer une date.

    \param cmd : Command : une structure Command pour stocker les informations
    \param log : char* : une chaîne de caractères pour stocker le nom du fichier de logs
*/
void ask_date(Command cmd, char *log);


#endif