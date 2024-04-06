#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>


#include "planificateur.h"


int main() {
    char command[100];
    char argument[100];
    int delay, iterations;

    char *file_name = create_file();

    int choice;
    printf("Voulez-vous entrer les commandes manuellement (1) ou lire les commandes à partir d'un fichier texte (2) ? ");
    scanf(" %d", &choice);

    if (choice == 1) {
        read_user_input(command, argument, &delay, &iterations, file_name);
    } else if (choice == 2) {
        char file_path[100];
        printf("Entrez le chemin d'accès au fichier texte : ");
        scanf("%s", file_path);
        read_file_input(file_path, file_name);
    } else {
        printf("Choix invalide.\n");
    }

    free(file_name);
    return 0;
}


int parse(const char *line, Command *cmd) {
    // Recherche de la commande, du délai, des itérations et de l'argument à l'aide de sscanf
    int n = sscanf(line, "\"%9s %99[^\"]\" %d %d", cmd->command, cmd->argument, &cmd->delay, &cmd->iterations);
    if (n == 4) {
        // Si les 4 valeurs ont été correctement lues, renvoie 1
        return 1;
    } else {
        // Si une erreur s'est produite, renvoie 0
        return 0;
    }
}


void read_file_input(char *file_path, char *file_name) {
    FILE* file = fopen(file_name, "a");
    if (file == NULL) {
        perror("Erreur : impossible d'ouvrir le fichier");
        exit(1);
    }

    FILE* input_file = fopen(file_path, "r");
    if (input_file == NULL) {
        perror("Erreur : impossible d'ouvrir le fichier d'entrée");
        exit(1);
    }
    fprintf(file, "Fichier d'entrée : %s\n\n", file_path);
    char line[500];
    int line_number = 1;
    while (fgets(line, sizeof(line), input_file) != NULL) {
        Command cmd;
        if (line_number > 1) {
            fprintf(file, "\n");
        }

        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r' || line[0] == '\t'){
            if (line[0] == '#') {
                fprintf(file, "\t%s\n", line);
            }
            continue;
        }
        else if (parse(line, &cmd)) {
            // Exécutez la commande ici
            fprintf(file, "Commande %d : %s\n", line_number, line);
            if (cmd.iterations < 0) {
                fprintf(file, "Le programme s'arrêtera lorsqu'un fichier .tmp sera trouvé dans le dossier Log/\n\n");
                fopen("end.tmp", "w");
            }
            fclose(file);
            execute_command(cmd.command, cmd.argument, cmd.delay, cmd.iterations, file_name);
        } else {
            fprintf(file, "Erreur : entrée invalide : %s", line);
        }
        line_number++;
    }
    
    fprintf(file, "\nFin de l'exécution");
    fclose(input_file);
    fclose(file);
}


char *create_file() {
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);

    // Format the time as a string
    char time_str[16];
    strftime(time_str, sizeof(time_str), "%d%m%y_%H%M%S", current_time);

    // Allocate memory for the file name
    char *file_name = malloc(50);
    if (!file_name) {
        printf("Erreur : impossible d'allouer de la mémoire pour le nom de fichier\n");
        exit(1);
    }

    // Build the file name
    strcpy(file_name, "Log/");
    strcat(file_name, time_str);
    strcat(file_name, ".txt");

    // Open the file with the formatted time in the name
    FILE *file = fopen(file_name, "a");
    if (file == NULL) {
        printf("Erreur : impossible de créer le fichier\n");
        free(file_name);
        exit(1);
    }

    fprintf(file, "Fichier de logs créé le %s\n\n", time_str);
    fclose(file);

    return file_name;
}


void read_user_input(char *command, char *argument, int *delay, int *iterations, char *file_name) {
    FILE* file = fopen(file_name, "a");
    if (file == NULL) {
        perror("Erreur : impossible d'ouvrir le fichier");
        exit(1);
    }

    printf("Entrez la commande : ");
    if(scanf(" %99[^\n]", command) != 1) {
        fprintf(file, "Erreur : commande invalide\n");
        exit(1);
    }

    printf("Entrez l'argument : ");
    if(scanf(" %99[^\n]", argument) != 1) {
        fprintf(file, "Erreur : argument invalide\n");
        exit(1);
    }

    printf("Entrez le délai en secondes : ");
    if(scanf(" %d", delay) != 1 || *delay <= 0) {
        fprintf(file,"Erreur : le délai doit être un nombre entier positif\n");
        exit(1);
    }

    printf("Entrez le nombre d'itérations : ");
    if(scanf(" %d", iterations) == 0) {
        fprintf(file, "Erreur : le nombre d'itérations doit être différent de 0\n");
        exit(1);
    }
    if (*iterations < 0) {
        fprintf(file, "Le programme s'arrêtera lorsqu'un fichier .tmp sera trouvé dans le dossier Log/\n\n");
        fopen("end.tmp", "w");
    }

    execute_command(command, argument, *delay, *iterations, file_name);
    fprintf(file, "\nFin de l'exécution");
    fclose(file);
}


int check_end_file(char *path){
    char command[1024];
    sprintf(command, "ls %s*.tmp 2>/dev/null", path);
    FILE *fp = popen(command, "r");
    if (fp != NULL) {
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0'; // Supprimez le caractère de nouvelle ligne
            int rm_result = remove(buffer);
            if (rm_result != 0) {
                fprintf(stderr, "Erreur lors de la suppression du fichier : %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            return 0;
        }
        pclose(fp);
    } else {
        fprintf(stderr, "Erreur lors de l'exécution de la commande ls : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return 1;
}


void execute_command(char *command, char *argument, int delay, int iterations, char *file_name) {
    int iterer = 1;
    int i = 0;

    FILE* file = fopen(file_name, "a");
    if (file == NULL) {
        perror("Erreur : impossible d'ouvrir le fichier");
        exit(1);
    }

    // Enregistrer l'heure de la première exécution
    time_t start_time = time(NULL);

    while(iterer == 1) {
        // Enregistrer l'heure de début de la prochaine exécution
        time_t next_start_time = time(NULL);

        // Calculer le temps écoulé depuis la dernière exécution
        int time_since_last_start = next_start_time - start_time;

        // Calculer le temps restant jusqu'à la prochaine exécution
        int remaining_time = delay - time_since_last_start;

        // Si le temps restant est négatif, cela signifie que l'exécution précédente a pris plus de temps que prévu
        // Dans ce cas, nous ne devrions pas attendre avant de lancer la prochaine exécution
        if (remaining_time < 0) {
            remaining_time = 0;
        }

        pid_t pid = fork();
        if(pid == -1) {
            fprintf(file, "Erreur : échec de la création du processus fils\n");
            return;
        } else if(pid == 0) {
            // Code exécuté par le processus fils
            fprintf(file, "Execution numéro %d de la commande \"%s\" avec l'argument \"%s\"\n", i+1, command, argument);
            fclose(file); // fermer le fichier dans le processus fils
            int status = execl(command, command, argument, (char *)NULL);
            if (status == -1) {
                file = fopen(file_name, "a");
                fprintf(file, "Erreur : échec de l'exécution de la commande\n\n");
                fclose(file);
                exit(1);
            }
        } else {
            // Code exécuté par le processus père
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                // Réouvrir le fichier dans le processus père pour écrire le message d'erreur
                file = fopen(file_name, "a");
                fprintf(file, "Erreur : la commande s'est terminée avec le message d'erreur suivant :\n%s\n\n", strerror(WEXITSTATUS(status)));
                fclose(file);
                exit(1);
            }

            if (iterations < 0) {
                char* path = "Log/";
                iterer = check_end_file(path);
            }

            // Attendre le temps restant avant la prochaine exécution
            if (remaining_time > 0 && (i < iterations || iterations < 0)){
                sleep(remaining_time);
            }

            // Enregistrer le temps de début de la dernière exécution
            start_time = next_start_time;

            // Rouvrir le fichier dans le processus père
            file = fopen(file_name, "a");
            i++;
            if (i == iterations) {
                iterer = 0;
            }
        }
    }
}