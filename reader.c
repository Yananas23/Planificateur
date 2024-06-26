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


int formatage(char *date, Date d)
{
    int n = sscanf(date, "%d-%d-%d %d:%d:%d", &d.year, &d.month, &d.day, &d.hour, &d.minute, &d.second);
    d.timestamp = timestamps(d);
    if (n == 6)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


long int timestamps(Date date)
{
    long int tpm = 0;
    tpm += date.second;
    tpm += date.minute * 60;
    tpm += date.hour * 3600;
    tpm += date.day * 86400;
    tpm += date.month * 2592000;
    tpm += date.year * 31536000;
    
    return time(NULL) - tpm;
}


void parse_cmd(const char *line, Command *cmd, char *log)
{
    // Recherche de la commande, du délai, des itérations et de l'argument à l'aide de sscanf
    char *c_time = malloc(30);
    Date d;
    int n = sscanf(line, "\"%20s %99[^\"]\" %d %s", cmd->command, cmd->argument, &cmd->iterations, c_time);
    formatage(c_time, d);
    cmd->delay = timestamps(d);
    if (n != 4)
    {
        error_log(log, 7, (char*)line);
    }
}


void parse_date(const char *line, Command *cmd, char *log)
{
    char *c_time = malloc(30);
    int n = sscanf(line, "=> %s", c_time);
    if (n == 1)
    {
        if (!formatage(c_time, cmd->start))
        {
            cmd->start.timestamp = 0;
        }
        
        date_log(log, cmd->start);
    }
    else
    {
        error_log(log, 7, (char*)line);
    }
}


FILE *open_file(char *file_path)
{
    FILE *file = fopen(file_path, "a");
    if (file == NULL)
    {
        perror("Erreur : impossible d'ouvrir le fichier d'entrée");
        exit(1);
    }
    
    return file;
}


CommandList read_file(Command cmd, char *file_path, char *log_path)
{

    char line[500];
    int line_number = 0;
    CommandList commands;
    commands.tasks = NULL;
    commands.nb_cmd = 0;

    FILE *input_file = open_file(file_path);
    file_log(log_path, file_path);

    while (fgets(line, sizeof(line), input_file) != NULL)
    {
        line_number++;
        char token = line[0];

        if (token == '#')
        {
            commentaire(log_path, line);
        }
        else if (token == '\"')
        {
            parse_cmd(line, &cmd, log_path);
            commands.tasks = realloc(commands.tasks, (commands.nb_cmd + 1) * sizeof(CommandList));
            commands.tasks[commands.nb_cmd] = cmd;
            commands.nb_cmd += 1;
            command(log_path, cmd, commands.nb_cmd);
        }
        else if (token == '=')
        {
            parse_date(line, &cmd, log_path);
        }
        else
        {
            error_log(log_path, 11, line);
        }
    }

    space_line(log_path);
    
    free(commands.tasks);
    fclose(input_file);

    return commands;
}



CommandList read_user(Command cmd, char *log_path)
{
    ask_command(cmd, log_path);

    ask_arguments(cmd, log_path);

    ask_iterations(cmd, log_path);
    
    ask_delay(cmd, log_path);
    
    ask_date(cmd, log_path);

    command(log_path, cmd, 0);

    CommandList commands;
    commands.tasks = &cmd;
    commands.nb_cmd = 1;
    free(commands.tasks);
    
    return commands;
}


void ask_command(Command cmd, char *log)
{
    printf("Entrez la commande : ");
    if (scanf(" %99[^\n]", cmd.command) != 1)
    {
        error_log(log, 6, "");
        exit(1);
    }    
}


void ask_arguments(Command cmd, char *log)
{
    printf("Entrez l'argument : ");
    if (scanf(" %99[^\n]", cmd.argument) != 1)
    {
        error_log(log, 5, "");
        exit(1);
    }    
}


void ask_iterations(Command cmd, char *log)
{
    printf("Entrez le nombre d'itérations : ");
    if (scanf(" %d", &cmd.iterations) == 0)
    {
        error_log(log, 4, "");
        exit(1);
    }
    if (cmd.iterations < 0)
    {
        infini_log(log);
        fopen("end.tmp", "w");
    }
}


void ask_delay(Command cmd, char *log)
{
    char *c_time = malloc(30);
    Date d;

    printf("Entrez le délai au format DD-MM-YYYY hh:mm:ss : ");
    scanf(" %s", c_time);

    formatage(c_time, d);
    cmd.delay = timestamps(d);

    if (cmd.delay != 1 || cmd.delay <= 0)
    {
        error_log(log, 3, "");
        exit(1);
    }
}


void ask_date(Command cmd, char *log)
{
    int choice;
    printf("Voulez-vous entrer une date de début d'éxécution ? oui = 1 / non = 2");
    scanf(" %d", &choice);

    if (choice == 1)
    {
        printf("Entrez la date de début d'éxécution au format DD-MM-YYYY hh:mm:ss : ");
        char *c_time = malloc(30);
        Date d = {2024, 5, 7, 12, 30, 0, 0};
        scanf(" %s", c_time);
        formatage(c_time, d);
        date_log(log, d);
        cmd.start = d;
    }
    else if (choice == 2)
    {
        cmd.start.timestamp = 0;
    }
    else
    {
        printf("Choix invalide.\n");
    }
}