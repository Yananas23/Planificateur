# Planificateur de tâches en ligne de commande

Ce programme est un planificateur de tâches en ligne de commande qui permet de lire l'entrée utilisateur pour une commande, un argument, un délai et un nombre d'itérations, et d'exécuter la commande avec l'argument, le délai et le nombre d'itérations spécifiés. Les résultats de chaque exécution sont enregistrés dans un fichier de log avec un horodatage dans le nom du fichier.

## Fonctionnalités

* Lit l'entrée utilisateur pour la commande, l'argument, le délai et le nombre d'itérations
* Exécute la commande avec l'argument, le délai et le nombre d'itérations spécifiés
* Enregistre les résultats de chaque exécution dans un fichier de log avec un horodatage dans le nom du fichier
* Vérifie si l'entrée utilisateur est valide avant de stocker les valeurs dans les paramètres
* Utilise la fonction execl() pour exécuter la commande et vérifie que l'exécution est réussie
* Permet de lire les commandes à exécuter depuis un fichier d'entrée

## Fonctions

Le programme fournit les fonctions suivantes :

* `read_user_input` : lit l'entrée utilisateur pour la commande, l'argument, le délai et le nombre d'itérations
* `execute_command` : exécute la commande saisie par l'utilisateur avec l'argument, le délai et le nombre d'itérations spécifiés
* `create_file` : crée un nouveau fichier de log et renvoie un pointeur vers celui-ci
* `check_end_file` : vérifie si le fichier de fin de boucle existe
* `read_file_input` : lit le fichier d'entrée et exécute les commandes qui y sont contenues
* `parse` : parse une ligne de commande et stocke les informations dans une structure `Command`

## Utilisation

Pour utiliser le programme, il suffit d'exécuter la commande `make all` pour compiler le programme et ensuite l'exécuter avec la commande `./planificateur`. Le programme demandera à l'utilisateur s'il souhaite entrer les commandes manuellement ou lire les commandes à partir d'un fichier texte. Si l'utilisateur choisit de lire les commandes à partir d'un fichier texte, il devra fournir le chemin d'accès au fichier.

## Exemple d'utilisation

### L'utilisateur choisi manuellement ses options:
```
$ ./planificateur
Voulez-vous entrer les commandes manuellement (1) ou lire les commandes à partir d'un fichier texte (2) ? 1
Entrez la commande : ls
Entrez l'argument : -l
Entrez le délai en secondes : 5
Entrez le nombre d'itérations : 3
```
Dans cet exemple, l'utilisateur a choisi d'entrer les commandes manuellement et a saisi la commande `ls -l` avec un délai de 5 secondes et un nombre d'itérations de 3. Le programme a ensuite exécuté la commande toutes les 5 secondes pendant 3 itérations et a enregistré les résultats dans un fichier de log avec un horodatage dans le nom du fichier.

### L'utilisateur indique un fichier d'exécution:
```
$ ./planificateur
Voulez-vous entrer les commandes manuellement (1) ou lire les commandes à partir d'un fichier texte (2) ? 2
Entrez le chemin d'accès au fichier texte : chemin/vers/lefichier.txt
```

Le fichier texte doit contenir les commandes à exécuter avec le chemin vers la commande et leurs argument, le délai et le nombre d'itérations, séparés par des espaces. Les lignes commençant par un caractère '#' sont considérées comme des commentaires et sont simplement inscritent dans le log.

#### Exemple de fichier texte :

```
# Exemple de fichier d'entrée
"/bin/ls -l" 1 3
"/bin/echo Bonjour" 2 2
"/bin/date +%Y-%m-%d" 5 1
```

### Répétition infinie

Si le nombre d'itération est une valeur négative, un message sera inscrit dans le fichier de log. Pour arrèter cette répétion il vous suffira de placer un fichier `.tmp` dans le dossier de log, celui ci sera consomé et arrêtera le processus en cours.

```
$ ./planificateur
Voulez-vous entrer les commandes manuellement (1) ou lire les commandes à partir d'un fichier texte (2) ? 1
Entrez la commande : ls
Entrez l'argument : -l
Entrez le délai en secondes : 5
Entrez le nombre d'itérations : -1
```