/*

  Interface du minishell.

    Fichier main.c : fichier principal du mini-shell
    Auteur : Sllmon & FameDoze2Point0
    Dépendances : stdio.h stdlib.h string.h parser.h processus.h

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "processus.h"

// Utile pour la gestion de l'historique des commandes (les 10 dernières commandes seront enregistrées dans le cas présenté ici)
#define HISTORICAL_SIZE 10

int main(){
    char cmdline[MAX_LINE_SIZE]; // Buffer des lignes de commandes
    char *cmdtoks[MAX_CMD_SIZE]; // "mots" de la ligne de commandes
    processus_t processes[MAX_CMD_SIZE]; // Tableau de la structure de représentation d'un processus à lancer.

    int end = 0; //

    // Variables et tableaux utilisées pour la gestion de l'historique des commandes
    char historical[HISTORICAL_SIZE][MAX_LINE_SIZE], // Tableau qui contient l'historique des 'HISTORICAL_SIZE' dernières commandes saisie au maximum; Géré comme une liste circulaire
        input[10]; // Contiendra la saisie de l'utilisateur concernant le choix d'accès à l'historique des commandes
    int indexHistorical1 = 0, // Permet de faire la gestion de la liste circulaire 
        indexHistorical2 = 0, // variable temporaire qui permettra de stocker un indice du tableau 'historical'
        requetUserHistorical = -1; // Contiendra la valeur de l'indice du tableau 'saisi' pour lequel l'utilisateur voudra accéder; -1 si l'utilisateur ne veut pas accéder à une commande enregistré dans l'historique

    int strlen_cmdline = 0;

    // On supprime le buffer de stdout
    setvbuf(stdout,NULL, _IONBF, 0);

    while (1){

        // On applique un délai de 0.01 secondes après chaque saisie de commande
        usleep(10000);

        // On efface les contenus de 'input' (pour l'historique de commande), 'cmdline', 'cmdtoks' et 'processes'
        memset(input, 0, sizeof(char) * 10);
        memset(cmdline, 0, sizeof(char) * MAX_LINE_SIZE);
        memset(cmdtoks, 0, sizeof(char *) * MAX_CMD_SIZE);
        memset(processes, 0, sizeof(processus_t) * MAX_CMD_SIZE);

        // On initialise les valeurs par défaut dans processes
        for (size_t i = 0; i < MAX_CMD_SIZE; i++)
            init_process(processes + i);

        //  Gestion de l'historique des commandes, lorque la commande "historique" est saisi dans le "mini_shell" la variable 'end' reçoit la valeur entière 777 
        if (end == 777){
            // On affiche toutes les commandes de l'historique des commandes
            for (indexHistorical2 = 0; indexHistorical2 < HISTORICAL_SIZE && strcmp(historical[indexHistorical2],"") != 0; indexHistorical2++)
                printf("[%d] = %s\n",indexHistorical2,historical[indexHistorical2]);

            // On demande à l'utilisateur quelle commande il souhaite utilisé parmi celle de l'historique
            fprintf(stdout,"Quelle commande voulez-vous effectuer ? (Tapez le numéro correspondant ou -1 pour annuler)\nSaisi : ");
            scanf("%s",input);

            // On convertit l'input (char*) en int
            end = sscanf(input,"%d",&requetUserHistorical);

            // Tant que la saisie est incorrecte ou que l'utilisateur n'a pas saisie '-1' pour annuler, on lui redemande son choix
            while (requetUserHistorical < -1 || requetUserHistorical > indexHistorical2 - 1  || end != 1){
                fprintf(stdout,"Saisi : ");
                scanf("%s",input);
                end = sscanf(input,"%d",&requetUserHistorical);
            }
            // Du fait que nous manipulons des chaines de caractères, lorsque nous transformons une chaine en entier, il reste le caractère \n à la fin
            // Nous purgeons le buffer avec getchar pour enlever \n (c'est principalement pour l'affichage)
            getchar();
            end = 0;
        }

        // Si l'utilisateur ne souhaite pas utiliser une commande de l'historique
        if (requetUserHistorical == -1){

            // Afficher un prompt
            printf("%sAntonin@Simon: %s%s%s $ %s", CBleu, CVert, getenv("PWD"), CRouge, CNormal);

            // Lire une ligne dans cmdline
            if (fgets(cmdline, MAX_LINE_SIZE - 1, stdin) == NULL)
                break;
            // On retire le \n enregistré dans le fgets
            strlen_cmdline = strlen(cmdline);
            cmdline[strlen_cmdline - 1] = '\0';
            strlen_cmdline--;
            
            // Si la commande saisie n'est pas vide
            if (strcmp(cmdline,"") != 0){
                // On enregistre dans l'historique des commandes la nouvelle commande saisie
                memset(historical[indexHistorical1],0,sizeof(char)*MAX_LINE_SIZE);
                strncpy(historical[indexHistorical1],cmdline,strlen_cmdline);
                indexHistorical1 = (indexHistorical1+1)%HISTORICAL_SIZE; // gestion de la liste circulaire de l'historique
            }
        }else{  // Si l'utilisateur souhaite utiliser une commande de l'historique
            // On place dans le tableau cmdline la commande souhaité par l'utilisateur et enregistré dans l'historique des commandes
            strcpy(cmdline,historical[requetUserHistorical]);


             // Afficher un prompt
            printf("%sAntonin@Simon: %s%s%s $ %s", CBleu, CVert, getenv("PWD"), CRouge, CNormal);
            printf("%s",cmdline);

            char cmdlinetmp[MAX_LINE_SIZE];
            // Lire une ligne dans cmdline
            if (fgets(cmdlinetmp, MAX_LINE_SIZE - 1, stdin) == NULL)
                break;
            
            // On retire le \n enregistré dans le fgets
            cmdlinetmp[strlen(cmdlinetmp) - 1] = '\0';

            strcat(cmdline,cmdlinetmp);
            requetUserHistorical = -1;
        }

        // Si l'utilisateur à saisie la touche Entrée sans avoir saisie autre chose
        if (strcmp(cmdline,"") == 0)
            continue;

        // On vérifie ici si on peut aider l'utilisateur à compléter le mot qu'il est en train de saisir
        // Et on continue à saisir des mots dans cmdline, jusqu'à ce que l'utilisateur le souhaite
        if(strlen_cmdline!=0 && end!= 777){
            completion(cmdline);
        }


        // Traiter la ligne de commande
            // Nous supprimons les espaces en début et en fin de ligne
            trim(cmdline);

            // On vérifie les éventuelles erreurs syntaxique
            if (strcmp(cmdline,"") == 0 || cmdline[0] == '|' || cmdline[0] == '&' || cmdline[0] == ';') {
                fprintf(stderr,"%s[ERROR] : erreur syntaxique !%s\n",CRouge,CNormal);
                continue;
            }
            
            // On ajoute d'éventuels espaces autour de ; ! || && & ...
            char *Tab[100]={";","!","|","&","||","&&",">","<","<<",">>","2>","2>>",">&2","2>&1","!","<<<",NULL};
            int a =  0;
            while(Tab[a]!=NULL){
                separate_multi(cmdline, Tab[a], 55);
                a++;
            }

        
            // On supprime les doublons d'espaces
            clean(cmdline);
        
            // On traite les variables d'environnement
            substenv(cmdline, MAX_LINE_SIZE - 1);
            
            // On ajoute un point virgule à la fin de la commande
            if (cmdline[strlen(cmdline) - 1] != ';')
                strncat(cmdline, " ;", MAX_LINE_SIZE - 1);

        // On découpe la ligne dans cmdtoks
        strcut(cmdline,' ', cmdtoks, MAX_CMD_SIZE); // Met le dernier élément de cmdline à NULL

        // On traduit la ligne en structures processus_t dans processes
        parse_cmd(cmdtoks, processes, MAX_CMD_SIZE);

        // Les commandes sont chaînées en fonction des séparateurs
        //   - next -> exécution inconditionnelle
        //   - next_success -> exécution si la commande précédente réussit
        //   - next_failure -> exécution si la commande précédente échoue
        
        // On exécute les commandes dans l'ordre en fonction des opérateurs
        // de flux

        end = file_process(processes, 0);

        // On supprime les fichiers cachés générés pendant l'exécution du programme (lors de l'utilisation d'un pipe en commande shell ou de <<)
        remove(".temp");
        remove(".temp2");
    }
    fprintf(stderr, "\nGood bye!\n");
    return 0;
}