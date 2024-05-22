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
 * @brief Crée un nouveau fichier de log et renvoie un pointeur vers celui-ci.
 *
 * @return char* : le nom du fichier créé
 */
char* create_log();


/**
 * @brief Ouvre un fichier de log en mode append.
 *
 * @param log : char* : le nom du fichier de log
 * @return FILE* : un pointeur vers le fichier ouvert
 */
FILE* open_log(char* log);


/**
 * @brief Parse le code d'une commande.
 *
 * @param line : char* : la ligne à parser
 * @return int : le code de la commande
 */
int parse_code(char* line);


/**
 * @brief Récupère le message d'erreur correspondant à un code d'erreur.
 *
 * @param error_code : int : le code d'erreur
 * @return char* : le message d'erreur
 */
char *get_error(int error_code);


/**
 * @brief Ajoute une ligne de log au fichier de log.
 *
 * @param log : FILE* : le fichier de log
 * @param code : int : le code de la commande
 * @param message : char* : le message à ajouter
 */
void error_log(char* log, int code, char* message);





#endif