#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "planificateur.h"
#include "log.h"
#include "reader.h"


char *create_log()
{
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);

    char time_str[16];
    strftime(time_str, sizeof(time_str), "%d%m%y_%H%M%S", current_time);

    char *log = malloc(50);
    if (!log)
    {
        printf("Erreur : impossible d'allouer de la mémoire pour le nom de fichier\n");
        exit(1);
    }

    // Créée un fichier de log avec le nom formaté
    strcpy(log, "Log/");
    strcat(log, time_str);
    strcat(log, ".txt");

    // Ouvre le fichier en mode append
    FILE *file = fopen(log, "a");
    if (file == NULL)
    {
        printf("Erreur : impossible de créer le fichier\n");
        free(log);
        exit(1);
    }

    fprintf(file, "Fichier de logs créé le %s\n\n", time_str);
    fclose(file);

    return log;
}

//voir si c'est mieux d'ouvrir le log dans chaque fonction ou de le faire dans la fonction "mère" read_file et read_cmd

FILE *open_log(char *log)
{
    FILE *file = fopen(log, "a");
    if (file == NULL)
    {
        perror("Erreur : impossible d'ouvrir le fichier de logs");
        exit(1);
    }
    return file;
}

int parse_code(char *line)
{
    int code;
    sscanf(line, "%d", &code);
    return code;
}


char *get_error(int error_code)
{
    FILE *Erreur = fopen("Erreur.txt", "r");
    char* error = malloc(150 * sizeof(char));
    if (error == NULL) {
        fclose(Erreur);
        return NULL;
    }

    int line = 0;
    // exemple => "3 : Erreur"
    char line_buffer[150];
    char extract[150];
    
    strcpy(error, "Erreur inconnue");

    while (fgets(line_buffer, sizeof(line_buffer), Erreur) != NULL)
    {
        if (error_code == parse_code(line_buffer))
        {
            sscanf(line_buffer, "%*[^:]: %99[^\n]", extract);
            strcpy(error, extract);
            break;
        }
        line++;
    }
    fclose(Erreur);
    return error;
}

void error_log(char *log, int error_code, char *info)
{
    FILE *file = open_log(log);
    char error[150];
    strcpy(error, get_error(error_code));
    fprintf(file, "Erreur : %s\n", error);
    if (strcmp(info, "") != 0){
        fprintf(file, " %s\n", info);
    }
}