/*
 
  Gestion des commandes internes du minishell (implémentation).

    Fichier builtin.c : fichier comportant les fonctions builtin, reconnaissance builtin, lancement builtin
    Auteur : Sllmon & FameDoze2Point0
    Dépendances : processus.h
 */


#include "processus.h"

/*
Fonction cd : Fonction qui permet de se déplacer dans des répertoires
    Paramètre argv  : argument de la fonction, permet de savoir le chemin désiré
    Paramètre fderr : sortie d'erreur, en cas d'une erreur de déplacement (repertoire non trouvé, etc...)
    retourne -1 en cas d'erreur sinon 0
*/
int cd(const char* argv, int fderr) {

    char pwd[SIZE_STRING_MAX]; 

    //On change la valeur de la variable d'environnement OLDPWD avec la valeur du cwd actuel
    getcwd(pwd,sizeof(pwd));
    setenv("OLDPWD",pwd,1);

    //on change le cwd
    if (chdir(argv)==-1) {
        write(fderr, "Erreur de changement de repertoire\n", 35);
        return -1;
    }
    
    //On change la valeur de la variable d'environnement PWD avec la valeur du nouveau cwd
    getcwd(pwd,sizeof(pwd));
    setenv("PWD",pwd,1);
    return 0;
}

/*
Fonction export : Fonction qui permet de donner une valeur à une variable d'environnement donnée (on l'a crée si elle n'existe pas)
    Paramètre var   : nom de la variable d'environnement
    Paramètre value : valeur de la variable d'environnement
    Paramètre fderr : sortie d'erreur
    retourne -1 en cas d'erreur sinon 0
*/
int export(const char* var, const char* value, int fderr) {
    //on écrit sur la variable d'environnement var, la chaine value (on écrase la chaine d'avant)
    if (setenv(var,value,1) == -1){
        write(fderr, "Erreur de changement de variable d'environnement\n", 49);
        return -1;
    }
    return 0;
}

/*
Fonction unset_shell : Fonction qui permet de supprimer la variable d'environnement
    Paramètre var   : nom de la variable d'environnement
    Paramètre fderr : sortie d'erreur
    retourne -1 en cas d'erreur sinon 0
*/
int unset_shell(const char* var, int fderr) {
    //on écrit sur la variable d'environnement var, la chaine value (on écrase la chaine d'avant)
    if (unsetenv(var) == -1){
        write(fderr, "Erreur de changement de variable d'environnement\n", 49);
        return -1;
    }
    return 0;
}

/*
Fonction exit_shell : Fonction qui permet de quitter le shell
    Paramètre ret   : valeur de sortie du shell
    Paramètre fderr : sortie d'erreur
    retourne -1 en cas d'erreur sinon 0
*/
int exit_shell(int ret, int fderr) {
    exit(ret);
    write(fderr, "Erreur de sortie de programme\n", 30); // ne s'affiche que si exit(ret) a échoué
    return -1;
}
/*
Fonction historique : Fonction qui permet d'accéder à l'historique des commandes effectuées
    retourne un code (777), code qui signifie que nous essayons d'accéder à l'historique
*/
int historique(){
    return 777;
}

/*
Fonction is_builtin : Fonction qui permet de s'avoir si une commande est built-in ou non
    Paramètre cmd   : commande dont on veut savoir si elle est builtin ou non
    retourne 1 si la commande est builtin sinon 0
*/
int is_builtin(const char* cmd) {
    char * builtIn[] = {"cd","export","exit","unset","historique","type",NULL};
    int res = searchstr(builtIn,cmd);
    return (res>=0); //retourne 1 si vrai
}

/*
Fonction type : Fonction qui permet de s'avoir si une commande est built-in ou non (fait par l'utilisateur)
    Paramètre cmd   : commande dont on veut savoir si elle est builtin ou non
    Paramètre fdout : sortie standard (où l'on écrit la réponse) 
    retourne 1 si la commande est builtin sinon 0
*/
int type(const char* cmd, int fdout){
    char res[MAX_LINE_SIZE];
    strncpy(res,cmd,MAX_LINE_SIZE-1); // simplement pour de l'affichage
    if (is_builtin(cmd))
        strncat(res," est un builtin du shell\n",MAX_LINE_SIZE-1);
    else
        strncat(res," n'est un builtin du shell\n",MAX_LINE_SIZE-1);

    write(fdout,res,strlen(res));
    return 0;
}

/*
Fonction builtin : Fonction qui permet de lancer les commandes builtin
    Paramètre cmd   : commande dont on veut savoir si elle est builtin ou non
    Paramètre fdout : sortie standard (où l'on écrit la réponse) 
    retourne le retour de la fonction builtin ou -1 si la fonction builtin n'a pas été trouvé
*/
int builtin(processus_t* proc) {

    if (strcmp(proc->path,"cd") == 0)
        return cd(proc->argv[1],proc->stderr);

    if (strcmp(proc->path,"export") == 0){

        char var[MAX_LINE_SIZE] = {'\0'}, value[MAX_LINE_SIZE] = {'\0'}, *cara, c;
        if (proc->argv[1][0] == ';' || (cara=strchr(proc->argv[1],'=')) == NULL){
            write(proc->stdout,"Erreur de la commande export\n",29);
            return -1;
        }     
        memmove(var,proc->argv[1],cara-proc->argv[1]);

        c = cara[1];
        if (c == '\'' || c == '"'){

            int flag = 1, arg = 0;
            for (size_t i = 2; i < strlen(cara) && flag; i++) {
                if (cara[i] == c)
                    flag = 0;
                else{
                    value[arg++] = cara[i];
                } 
            }
            if (flag)
            {
                value[arg++] = ' ';
                for (size_t i = 2; proc->argv[i] != NULL && flag; i++) {
                    
                    for (size_t j = 0; j < strlen(proc->argv[i]) && flag; j++)
                    {
                        if (proc->argv[i][j] == c)
                            flag = 0;
                        else{
                            value[arg++] = proc->argv[i][j];
                        }   
                    }  
                    value[arg++] = ' '; 
                }   
            }
        }else{
            memmove(value,cara+1,strlen(proc->argv[1])-(cara-proc->argv[1]));
        }
        
        return export(var,value,proc->stderr);

    }

    if (strcmp(proc->path,"unset") == 0)
        return unset_shell(proc->argv[1],proc->stderr);
    
    if (strcmp(proc->path,"exit") == 0)
        return exit_shell(0,proc->stderr);

    if (strcmp(proc->path,"historique") == 0)
        return historique();

    if (strcmp(proc->path,"type") == 0)
        return type(proc->argv[1],proc->stdout);
    
    return -1;
}