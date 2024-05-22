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


int main()
{
    Command cmd;
    CommandList cmd_list;

    char *log = create_log();

    int choice;
    printf("Voulez-vous entrer les commandes manuellement (1) ou lire les commandes à partir d'un fichier texte (2) ? ");
    scanf(" %d", &choice);

    if (choice == 1)
    {
        cmd_list = read_user(cmd, log);
    }
    else if (choice == 2)
    {
        char file_path[100];
        printf("Entrez le chemin d'accès au fichier texte : ");
        scanf("%s", file_path);
        cmd_list = read_file(cmd, file_path, log);
    }
    else
    {
        printf("Choix invalide.\n");
    }

    execute_command(cmd_list, log);
    free(log);
    return 0;
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
    
    sleep(cmd.delay);
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
            error_log(log, 8, "");
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
                error_log(log, 9, "");
                fclose(file);
                exit(1);
            }
        }
        else
        {
            // Code exécuté par le processus père
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                // Réouvrir le fichier dans le processus père pour écrire le message d'erreur
                error_log(log, 10, strerror(WEXITSTATUS(status)));
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


void execute_command(CommandList cmd, char *log) {
    for(int i = 0; i < cmd.nb_cmd; i++){
        pid_t mon_pid;
        mon_pid = fork();
        if(mon_pid == 0){
            execute_command_aux(cmd.tasks[i], log);
            exit(0);
        }
    }   
}