#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "planificateur.h"


int main()
{
    Command cmd;

    char *log = create_log();

    int choice;
    printf("Voulez-vous entrer les commandes manuellement (1) ou lire les commandes à partir d'un fichier texte (2) ? ");
    scanf(" %d", &choice);

    if (choice == 1)
    {
        read_user_input(cmd, log);
    }
    else if (choice == 2)
    {
        char file_path[100];
        printf("Entrez le chemin d'accès au fichier texte : ");
        scanf("%s", file_path);
        read_file_input(cmd, file_path, log);
    }
    else
    {
        printf("Choix invalide.\n");
    }

    free(log);
    return 0;
}


int parse(const char *line, Command *cmd)
{
    // Recherche de la commande, du délai, des itérations et de l'argument à l'aide de sscanf
    int n = sscanf(line, "\"%9s %99[^\"]\" %d %d", cmd->command, cmd->argument, &cmd->delay, &cmd->iterations);
    if (n == 4)
    {
        // Si les 4 valeurs ont été correctement lues, renvoie 1
        return 1;
    }
    else
    {
        // Si une erreur s'est produite, renvoie 0
        return 0;
    }
}


void read_file_input(Command cmd, char *file_path, char *log)
{
    FILE *file = fopen(log, "a");
    if (file == NULL)
    {
        perror("Erreur : impossible d'ouvrir le fichier");
        exit(1);
    }

    FILE *input_file = fopen(file_path, "r");
    if (input_file == NULL)
    {
        perror("Erreur : impossible d'ouvrir le fichier d'entrée");
        exit(1);
    }
    fprintf(file, "Fichier d'entrée : %s\n\n", file_path);
    char line[500];
    int line_number = 0;
    Command *commands = NULL;
    int num_commands = 0;

    while (fgets(line, sizeof(line), input_file) != NULL)
    {
        line_number++;

        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r' || line[0] == '\t')
        {
            if (line[0] == '#')
            {
                fprintf(file, "\t%s\n", line);
            }
            continue;
        }
        else if (line[0] == '\"')
        {
            if (parse(line, &cmd))
            {
                commands = realloc(commands, (num_commands + 1) * sizeof(Command));
                commands[num_commands++] = cmd;
                fprintf(file, "Commande %d : %s %s %d %d\n", num_commands, cmd.command, cmd.argument, cmd.delay, cmd.iterations);
            }
            else
            {
                fprintf(file, "Erreur : entrée invalide : %s", line);
            }
        }
        else
        {
            fprintf(file, "\n%s", line);
        }
    }

    fprintf(file, "\n");
    fclose(file);

    // Executes les commandes lues à partir du fichier
    execute_command(commands, num_commands, log);
    
    free(commands);
    fclose(input_file);
}


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


void read_user_input(Command cmd, char *log)
{
    FILE *file = fopen(log, "a");
    if (file == NULL)
    {
        perror("Erreur : impossible d'ouvrir le fichier");
        exit(1);
    }

    printf("Entrez la commande : ");
    if (scanf(" %99[^\n]", cmd.command) != 1)
    {
        fprintf(file, "Erreur : commande invalide\n");
        exit(1);
    }

    printf("Entrez l'argument : ");
    if (scanf(" %99[^\n]", cmd.argument) != 1)
    {
        fprintf(file, "Erreur : argument invalide\n");
        exit(1);
    }

    printf("Entrez le délai en secondes : ");
    if (scanf(" %d", &cmd.delay) != 1 || cmd.delay <= 0)
    {
        fprintf(file, "Erreur : le délai doit être un nombre entier positif\n");
        exit(1);
    }

    printf("Entrez le nombre d'itérations : ");
    if (scanf(" %d", &cmd.iterations) == 0)
    {
        fprintf(file, "Erreur : le nombre d'itérations doit être différent de 0\n");
        exit(1);
    }
    if (cmd.iterations < 0)
    {
        fprintf(file, "Le programme s'arrêtera lorsqu'un fichier .tmp sera trouvé dans le dossier Log/\n\n");
        fopen("end.tmp", "w");
    }

    fclose(file);
    Command *command = malloc(sizeof(Command));
    command[0] = cmd;
    execute_command(command, 1, log);
}


int check_end_file(char *path)
{
    char command[1024];
    sprintf(command, "ls %s*.tmp 2>/dev/null", path);
    FILE *fp = popen(command, "r");
    if (fp != NULL)
    {
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            buffer[strcspn(buffer, "\n")] = '\0'; // Supprimez le caractère de nouvelle ligne
            int rm_result = remove(buffer);
            if (rm_result != 0)
            {
                fprintf(stderr, "Erreur lors de la suppression du fichier : %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            return 0;
        }
        pclose(fp);
    }
    else
    {
        fprintf(stderr, "Erreur lors de l'exécution de la commande ls : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return 1;
}


void execute_command_aux(Command cmd, char *log)
{
    int iterer = 1;
    int i = 0;
    int remaining_time;

    FILE *file = fopen(log, "a");
    if (file == NULL)
    {
        perror("Erreur : impossible d'ouvrir le fichier");
        exit(1);
    }
    
    sleep(cmd.delay)
    // Enregistrer l'heure de la première exécution
    time_t start_time = time(NULL);

    while (iterer == 1)
    {
        // Enregistrer l'heure de début de la prochaine exécution
        time_t next_start_time = time(NULL);

        // Calculer le temps écoulé depuis la dernière exécution
        int time_since_last_start = next_start_time - start_time;

        // Calculer le temps restant jusqu'à la prochaine exécution
        if (cmd.delay != time_since_last_start)
        {
            remaining_time = cmd.delay - time_since_last_start;
        }
        else
        {
            remaining_time = cmd.delay;
        }

        // Si le temps restant est négatif, cela signifie que l'exécution précédente a pris plus de temps que prévu
        // Dans ce cas, nous ne devrions pas attendre avant de lancer la prochaine exécution
        if (remaining_time < 0)
        {
            remaining_time = 0;
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            fprintf(file, "Erreur : échec de la création du processus fils\n");
            return;
        }
        else if (pid == 0)
        {
            // Code exécuté par le processus fils (son)
            fprintf(file, "\nExecution numéro %d de la commande \"%s\" avec l'argument \"%s\"\n", i + 1, cmd.command, cmd.argument);
            fclose(file); // fermer le fichier dans le processus fils


            int status = execl(cmd.command, cmd.command, cmd.argument, (char *)NULL);
            if (status == -1)
            {
                file = fopen(log, "a");
                fprintf(file, "Erreur : échec de l'exécution de la commande\n\n");
                fclose(file);
                exit(1);
            }
        }
        else
        {
            // Code exécuté par le processus père (father)
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                // Réouvrir le fichier dans le processus père pour écrire le message d'erreur
                file = fopen(log, "a");
                fprintf(file, "Erreur : la commande s'est terminée avec le message d'erreur suivant :\n%s\n\n", strerror(WEXITSTATUS(status)));
                fclose(file);
                exit(1);
            }

            // Attendre le temps restant avant la prochaine exécution
            if (remaining_time > 0 && (i < cmd.iterations || cmd.iterations < 0))
            {
                sleep(remaining_time);
            }

            // Enregistrer le temps de début de la dernière exécution
            start_time = next_start_time;

            i++;
            if (i == cmd.iterations)
            {
                iterer = 0;
            } else if (cmd.iterations < 0)
            {
                char *path = "Log/";
                iterer = check_end_file(path);
            }
            if (iterer == 0)
            {
                file = fopen(log, "a");
                fprintf(file, "\n//\nFin de l'exécution de la commande \"%s\" avec l'argument \"%s\"\n//\n", cmd.command, cmd.argument);
                fclose(file);
            }
        }
    }
}


void execute_command(Command* cmd, int nb_cmd, char *log) {
    for(int i = 0; i < nb_cmd; i++){
        pid_t mon_pid;
        mon_pid = fork();
        if(mon_pid == 0){
            execute_command_aux(cmd[i], log);
            exit(0);
        }
    }   
}