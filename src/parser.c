/*

  Parsing de la ligne de commandes utilisateur (implémentation).

  Fichier parser.c : primitives des fonctions gérant les chaines de caractères (parsing des commandes) + le système d'autocomplétion
  Auteur : Sllmon & FameDoze2Point0
  Dépendances : parser.h
 */

#include "parser.h"

#define MAX_LINE_SIZE   1024
#define MAX_CMD_SIZE    256

/*
Fonction countc : Fonction qui va compter le nombre de caractère c dans la chaine str
    Paramètre str : Chaine de caractère en paramètre d'entrée, dans laquelle nous allons compter des occurrences d'un certain caractère
    Paramètre c : Caractère dont nous voulons trouver son nombre d'occurrence dans str
    Retourne le nombre d'occurrence du caractère présent dans c, dans la chaine str
*/
int countc(const char *str, char c){

    int occ = 0;    // occurrence de c dans str
    int sizeChaine; // longueur de str

    /*-------------------GESTION DES ERREURS-------------------*/
    if ((sizeChaine = strlen(str)) == 0){
        fprintf(stderr, "[ERROR countc] : La chaine \"str\" entrée est vide\n");
        return -1; // retourne l'erreur (la chaine est vide)
    }
    /*-----------------FIN GESTION DES ERREURS-----------------*/

    for (int i = 0; i < sizeChaine; i++)
        if (str[i] == c)
            occ++;
    return occ;
}

/*
Fonction countc_s : Fonction qui va compter le nombre d'occurrences de plusieurs caractères passés dans une chaine "c"
    Paramètre str : Chaine de caractères principale où l'on va vérifier le nombre d'occurrences des caractères présents dans c
    Paramètre c : Chaine de caractères comprenant différents caractères dont nous voulons le nombre d'occurrence au total
    Retourne le nombre d'occurrence des caractères présents dans la chaine c, dans la chaine str
*/
int countc_s(const char *str, const char *c)
{

    int occ = 0; // occurrence of each characters contained in str
    int sizeC;   // number of characters that we need to count

    /*-------------------GESTION DES ERREURS-------------------*/
    if (strlen(str) == 0)
    {
        fprintf(stderr, "[ERROR countc_s] : La chaine \"str\" entrée est vide\n");
        return -1; // return an error (the string is empty)
    }
    if ((sizeC = strlen(c)) == 0)
    {
        fprintf(stderr, "[ERROR countc_s] : La chaine \"c\" entrée est vide\n");
        return -2; // return an error (the string is empty)
    }
    /*-----------------FIN GESTION DES ERREURS-----------------*/

    for (int i = 0; i < sizeC; i++)
        occ += countc(str, c[i]);

    return occ;
}

/*
Fonction  searchc : Fonction qui va chercher la position d'un caractère c dans str et les enregistrer (dans la limite de max)
    Paramètre str : Chaine d'entrée, dans laquelle nous allons chercher un caractère
    Paramètre c : Caractère que nous voulons rechercher
    Paramètre position : Position enregistrée du caractère c dans str
    Paramètre max : Nombre maximum de position que nous pouvons enregistrer
    Retourne un code (erreur ou non), puis via l'adresse les positions de c dans str (dans position)
*/
int searchc(const char *str, const char c, char **position, int max){

    int occ = 0;
    int sizeChaine; // taille de la chaine str

    /*-------------------GESTION DES ERREURS-------------------*/
    if (max < 0) {
        fprintf(stderr, "[ERROR searchc] : La variable \"max\" doit être supérieure à 0 cccc\n");
        return -1;
    }
    if ((sizeChaine = strlen(str)) == 0) {

        fprintf(stderr, "[ERROR searchc] : La chaine \"str\" entrée est vide\n");
        return -2;
    }
    /*-----------------FIN GESTION DES ERREURS-----------------*/

    for (int i = 0; i < sizeChaine && occ < max; i++){
        if (str[i] == c){
            position[occ++] = (char*)&str[i];
        }
    }
    
    return occ;
}

/*
Fonction  searchc_multi : Fonction qui va chercher la position d'une chaine de caractère c dans str et les enregistrer (dans la limite de max)
    Paramètre str : Chaine d'entrée, dans laquelle nous allons chercher un caractère
    Paramètre c : Chaine de caractère que nous voulons rechercher
    Paramètre position : Position enregistrée de la chaine de caractère c dans str
    Paramètre max : Nombre maximum de position que nous pouvons enregistrer
    Retourne un code (erreur ou non), puis via l'adresse les positions de c dans str (dans position)
*/
int searchc_multi(const char *str, const char *c, char **position, int max){

    int sizeChaine; // taille de la chaine str
    int sizeC;

    /*-------------------GESTION DES ERREURS-------------------*/
    if (max < 0) {
        fprintf(stderr, "[ERROR search_multi] : La variable \"max\" doit être supérieure à 0 cccc\n");
        return -1;
    }
    if ((sizeChaine = strlen(str)) == 0) {
        fprintf(stderr, "[ERROR search_multi] : La chaine \"str\" entrée est vide\n");
        return -2;
    }
    if ((sizeC = strlen(c)) == 0) {
        fprintf(stderr, "[ERROR search_multi] : La chaine \"c\" entrée est vide\n");
        return -3;
    }
    /*-----------------FIN GESTION DES ERREURS-----------------*/

    int occ = 0;
    char *res;

    for (int i = 0; i < sizeChaine && occ < max && (res = strstr(str+i,c)) != NULL; i = res - str + sizeC)
        position[occ++] = (char *)res;
    //i = res - str + sizeC // dans i on récupère l'adresse du caractère après notre motif trouvé.
    return occ;
}

/*
Fonction  searchc_s : Fonction qui va chercher la position d'un caractère c dans str et les enregistrer (dans la limite de max)
    Paramètre str : Chaine d'entrée, dans laquelle nous allons chercher des caractères
    Paramètre c : Chaines de caractères comportant les caractères que nous voulons rechercher
    Paramètre position : Position enregistrée des caractères c dans str
    Paramètre max : Nombre maximum de position que nous pouvons enregistrer
    Retourne un code (erreur ou non), puis via l'adresse les positions des caractères c dans str (dans position)
*/
int searchc_s(const char *str, const char *c, char **position, int max){

    int sizeChaine, sizeC; // taille de la chaine principale str et taille du tableau c
    int i = 0, occ = 0;

    /*-------------------GESTION DES ERREURS-------------------*/
    if (max < 0) {
        fprintf(stderr, "[ERROR searchc_s] : La variable \"max\" doit être supérieure à 0 azdazdazd\n");
        return -1;
    }
    if ((sizeChaine = strlen(str)) == 0) {
        fprintf(stderr, "[ERROR searchc_s] : La chaine \"str\" entrée est vide\n");
        return -2;
    }
    if ((sizeC = strlen(c)) == 0) {
        fprintf(stderr, "[ERROR searchc_s] : Le tableau de caractères \"c\" entré est vide\n");
        return -3;
    }
    /*-----------------FIN GESTION DES ERREURS-----------------*/

    char temp[sizeC]; // tableau temporaire qui va contenir les caractères que nous cherchons (il sert dans le cas ou nous avons des doublons)

    /*---------------------------------------
    Cette partie de l'algorithme permet d'enlever les doublons :
    Par exemple :
    Si nous avons comme chaine "antonin et simon" avec comme caractères "nn"
    nous allons avoir 8 n sans cette partie, alors qu'en réalité nous n'avons que 4 n
    */
    for (int j = 0; j < sizeC; j++)
        if (strchr(temp, c[j]) == NULL)
            temp[i++] = c[j];

    //---------------------------------------
    
    sizeC = strlen(temp); // potentielle nouvelle taille de sizeC

    for (i = 0; i < sizeChaine && occ < max; i++)
        for (int j = 0; j < sizeC; j++)
            if (str[i] == temp[j])
                position[occ++] = (char*)&str[i];
        
    return occ;

    
}

/*
Fonction  replace : Fonction qui va remplacer dans la chaine str, le motif par la nouvelle chaine
    Paramètre str : Chaine d'entrée, dans laquelle nous allons chercher notre motif et peut être le remplacer
    Paramètre motif : motif que nous voulons rechercher et remplacer
    Paramètre Ntab : Chaine de caractères que l'on veut mettre à la place du motif
    Paramètre max : Taille maximum de str à la fin
    Retourne un code (erreur ou non)
*/
int replace(char *str, const char *motif, const char *Ntab, int max) {

    int sizeMotif, sizeTab, sizeNTab; // les différentes tailles des tableaux
    int pos;
    char *emp;

    /*-------------------GESTION DES ERREURS-------------------*/

    if ((sizeTab = strlen(str)) == 0) {
        fprintf(stderr, "[ERROR replace] : La chaine \"str\" entrée est vide\n");
        return 1;
    }
    if ((sizeMotif = strlen(motif)) == 0) {
        fprintf(stderr, "[ERROR replace] : La chaine\"motif\" entrée est vide\n");
        return 2;
    }
    if (max <= 0) {
        fprintf(stderr, "[ERROR replace] : Le max doit être supérieur à 0\n");
        return 4;
    }
    max = (MAX_LINE_SIZE < max)?MAX_LINE_SIZE:max;
    if ((emp = strstr(str, motif)) == NULL) {
        return 5;
    }
    /*-----------------FIN GESTION DES ERREURS-----------------*/
    sizeNTab = strlen(Ntab);
    pos = emp - str; // récupération de l'indice de la position du motif dans str

    /*  déplacement de la chaine après le motif
        Soit  strlen(motif) < strlen(Ntab) alors on déplace le bloc vers la droite pour laisser la place à Ntab
        Soit strlen(motif) > strlen(Ntab) alors on deplace le bloc vers la gauche car on a trop de caractères
    */
    memmove(str + pos + sizeNTab, str + pos + sizeMotif, sizeTab - pos - sizeMotif);

    // on place notre nouvelle chaine Ntab dans str
    strncpy(str + pos, Ntab, sizeNTab);
    
    if (sizeMotif > sizeNTab)
        str[sizeTab - sizeMotif + sizeNTab] = '\0'; // si on a un décalage vers la gauche, des lettres les plus à droite vont rester et ne pas être supprimées (car ça n'est qu'une copie de mémoire)
        
    if (sizeTab - sizeMotif + sizeNTab >= max)
        str[max] = '\0';
    return 0;
}

/*
Fonction trim : Fonction qui va enlever les espaces au début et à la fin de la chaine
    Paramètre str : chaine de caractère d'entrée (dont on va enlever les espaces avant et après)
    Retourne un entier correspondant à un code erreur ou non. Via l'adresse de la chaine, modifie cette dernière en enlevant tous les espaces placés au début et à la fin
*/
int trim(char *str) {
    int sizeChaine; // taille de la chaine passée en paramètre
    /*-------------------GESTION DES ERREURS-------------------*/
    if ((sizeChaine = strlen(str)) <= 0) {
        fprintf(stderr, "[ERROR trim] : La chaine de caractères entrée est vide\n");
        return -1;
    }
    /*-----------------FIN GESTION DES ERREURS-----------------*/
    int i = 0;
    for (; i < sizeChaine && str[i] == ' '; i++); // On compte tous les espaces en début de chaine
    sizeChaine -= i; // On connait le décalage que l'on doit faire vers la gauche (taille - nombre d'espaces)
    memmove(str, str + i, sizeChaine); // on déplace vers la gauche
    for (i = sizeChaine - 1; i > 0 && str[i] == ' '; i--); // On compte les espaces en fin de chaine
    str[i + 1] = '\0'; // on ne les considère plus en mettant le caractère de fin de chaine
    return 0;
}

/*
Fonction clean : Fonction qui va enlever les espaces en doublons
    Paramètre str : chaine de caractères d'entrée (dont on va enlever les espaces doublons)
    Retourne un code (erreur ou non)
*/
int clean(char *str) {
    int sizeChaine;
    /*-------------------GESTION DES ERREURS-------------------*/
    if ((sizeChaine = strlen(str)) <= 0) {
        fprintf(stderr, "[ERROR clean] : La chaine de caractères entrée est vide\n");
        return -1;
    }
    /*-----------------FIN GESTION DES ERREURS-----------------*/

    sizeChaine -= 1; // on enlève 1 car on utilise str[i+1], on ne veut pas de dépassement de mémoire
    int i = 0;

    while (i < sizeChaine) { // on ignore le dernier caractères, cela peut être un espace (dans tous les cas on l'atteint avec i+1)
        if (str[i] == ' ' && str[i + 1] == ' ') {
            memmove(str + i, str + i + 1, sizeChaine - i); // on déplace de 1 vers la gauche
            sizeChaine--;                                  // on réduit de 1 la taille de la chaine au total
        }
        else  // on n'avance que si on ne retire pas, car lorsqu'on retire c'est comme si on avançait
            i++;
    }
    str[i + 1] = '\0'; // vu que nous faisons des déplacements vers la gauche, il est important de vérifier s'il n'y a pas des caractères non voulus
    return 0;
}

/*
Fonction separate_multi (separate_s) : Fonction qui va mettre des espaces autour d'une chaine de caractère dans la chaine str
    Paramètre str : Chaine de caractères d'entrée, celle où l'on va y placer les espaces si besoin autour de la chaine c
    Paramètre c : Chaine de caractères que nous voulons séparer par un espace
    Paramètre max : Nombre maximum de chaine de caractère c que nous pouvons séparer par un espace
    Retourne un code (erreur ou non)
*/
int separate_multi(char *str, const char *c, size_t max)
{
    int sizeChaine;
    /*-------------------GESTION DES ERREURS-------------------*/
    if ((sizeChaine = strlen(str)) == 0){
        fprintf(stderr, "[ERROR separate_multi] : La chaine \"str\" entrée est vide\n");
        return -1;
    }
    if ((sizeChaine = strlen(c)) == 0){
        fprintf(stderr, "[ERROR separate_multi] : La chaine \"c\" entrée est vide\n");
        return -1;
    }
    if (max <= 0){ 
        fprintf(stderr, "[ERROR separate_multi] : La variable max doit être supérieure à 0aaaa\n");
        return -3;
    }

    max = (max > MAX_LINE_SIZE)?MAX_LINE_SIZE:max;

    /*-----------------FIN GESTION DES ERREURS-----------------*/
   
    char *position[MAX_CMD_SIZE];
    int nb_occ = 0, i = 0, j = 0;
    int decalage, pos;

    int taille_chaine = strlen(c);

    int flag = 0; // s'incrémente de 1 pou

    // On va chercher le nombre d'occurence de la chaine c dans str, et placer un pointeur du début de chaque occurence dans le tableau position
    if ((nb_occ = searchc_multi(str, c, position, max)) > 0){
        // Pour toutes les occurences de la chaine c dans str
        for (i = 0; i < nb_occ; i++){
            // i*2 permet de prendre en compte les espaces ajoutés à l'incrémentation précédente
            // Par exemple si on a la chaine str pour i = 0 qui correspond à "echo chat&&echo lapin&&echo lion" et pour c la chaine "||"
            // Alors après la première occurence de "||" on ajoute 2 espaces et on doit donc se déplacer de 2 adresses supplémentaires
            // -j*2 permet de prendre en compte le cas où on n'ajoute pas d'espaces pour une occurence donnée
            pos = *(position+i)+i*2-j*2 - str;

            // Nous allons ici vérifier qu'on ne sépare pas des éléments qu'on souhaite pas séparer : 
                
                flag = 0; // S'incrémente de 1 après chaque vérification, si la variable ne s'est pas incrémenté une fois
                //alors on ne séapare pas la chaine avec des espaces 

                // On empêche de séparer des "<<"  ">>" "&&" avec c : "<" ou ">"  ou ...
                if(!(taille_chaine == 1 && (*(str+pos) == *(str+pos+1) || *(str+pos-1) == *(str+pos)))){
                    flag++;
                }

                // On empêche de séparer des "<<<" avec c : "<<"
                if(!(strcmp(c,"<<") == 0 && ((*(str+pos) == *(str+pos+1) && *(str+pos) == *(str+pos+2)) || (*(str+pos-1) == *(str+pos) && *(str+pos+1) == *(str+pos))))){
                    flag++;
                }

                // On empêche de séparer "2>" avec c : ">"   
                if(!(strcmp(c,">") == 0 && (*(str+pos-1) == '2' && *(str+pos) == '>'))){
                    flag++;
                }

                // On empêche de séparer "2>>" avec c : "2>"
                if(!(strcmp(c,"2>") == 0 && *(str+pos+2) == '>')){
                    flag++;
                }

                // On empêche de séparer "2>>" avec c : ">>"
                if(!(strcmp(c,">>") == 0 && (*(str+pos-1) == '2' && *(str+pos) == '>' && *(str+pos+1) == '>'))){
                    flag++;
                }

                // On empêche de séparer ">&2" ou ">&1" avec c : ">"
                if(!(strcmp(c,">") == 0 && (*(str+pos+1) == '&' && (*(str+pos+2) == '2' || *(str+pos+2) == '1') ))){
                    flag++;
                }
                // On empêche de séparer ">&2" ou ">&1" avec c : "&"
                if(!(strcmp(c,"&") == 0 && (*(str+pos-1) == '>' && (*(str+pos+1) == '2' || *(str+pos+1) == '1') ))){
                    flag++;
                }

                // On empêche de séparer "2>&2" ou "2>&1" avec c : ">&2" ou ">&1"
                if(!((strcmp(c,">&2") == 0 || strcmp(c,">&1") == 0) && (*(str+pos-1) == '2' || *(str+pos-1) == '1') )){
                    flag++;
                }

                // On empêche de séparer "2>&2" ou "2>&1" avec c : "2>"
                if(!(strcmp(c,"2>") == 0 && (*(str+pos+2) == '&' && (*(str+pos+3) == '1' || *(str+pos+3) == '2')))){
                    flag++;
                }

            if(flag == 9){  //On vérifie si les conditions en haut sont respectés
                // On sépare l'occurence de c dans str par des espaces
                    decalage = strlen(str)-pos+1;
                    memmove(str+pos+1,str+pos,decalage);
                    str[pos]=' ';
                    memmove(str+pos+2+taille_chaine,str+pos+1+taille_chaine,decalage-taille_chaine);
                    str[pos+1+taille_chaine]=' ';
            }
            else{
                j++;
            }
        }
    }
    clean(str);
    trim(str);
    return 0;
}

/*
Fonction strcut : Fonction qui va séparer une chaine grâce à des délimiteurs, et va mettre les morceaux coupés dans des tokens (à condition de ne pas dépasser le nombre de token autorisé).
Paramètre str : Chaine de caractères d'entrée, que nous allons découper en token
Paramètre sep : Caractère où l'on va découper notre chaine str en token
Paramètre token : Tableau d'enregistremen des tokens
Paramètre max : Nombre max de token demandé
Retourne un code (erreur ou non)
*/
int strcut(char *str, char sep, char **token, size_t max) {

    int sizeChaine;
    size_t nbSep = countc(str,sep), i = 0;
    /*-------------------GESTION DES ERREURS-------------------*/
    if ((sizeChaine = strlen(str)) == 0) {
        fprintf(stderr, "[ERROR strcut] : La chaine \"str\" entrée est vide\n");
        return -1;
    }
    if (nbSep >= max) {
        fprintf(stderr, "[ERROR strcut] : Max doit être supérieur ou égal au nombre de token voulu (en comptant NULL)\n");
        return -3;
    }
    /*-----------------FIN GESTION DES ERREURS-----------------*/

    char *position[nbSep];

    if (searchc(str, sep, position, nbSep) >= 0) {

        token[0] = str;
        for (i = 0; i < nbSep; i++) {
            position[i][0] = '\0';
            token[i + 1] = &position[i][0] + 1;
        }
    }
    token[nbSep + 1] = NULL;
    return nbSep + 1;
}

/*
Fonction searchstr : Fonction qui va chercher une chaine de caractère dans un tableau de chaine de caractères (terminé par NULL)
Paramètre tab : Tableau de chaines de caractères
Paramètre motif : Chaine de caractères que nous recherchons
Retourne un code (erreur ou non)
*/
int searchstr(char **tab, const char *motif)
{
    int pos = -1;

    /*-------------------GESTION DES ERREURS-------------------*/
    if (strlen(motif) == 0) {
        fprintf(stderr, "[ERROR searchstr] : La chaine \"motif\" entrée est vide\n");
        return -2;
    }
    /*-----------------FIN GESTION DES ERREURS-----------------*/

    for (int i = 0; tab[i] != NULL ; i++)
        if (strcmp(tab[i], motif) == 0) {
            pos = i;
            break;
        }

    return pos;
}

/*
Fonction substenv : Fonction qui va prendre une chaine de caractère et remplacer les variables d'environnements présentes dedans par leur valeur
Paramètre str : Chaine de caractères d'entrée, où l'on va remplacer les variables d'environnement pas leur valeur
Paramètre max : taille maximale de la chaine de caractère str à la fin
Retourne un code (erreur ou non)
*/
int substenv(char *str, size_t max) {
    
    char *c, *resEnv;
    int i = 0, 
        size = 0, // taille du motif sans {} ni $
        flagAccolade = 0; // Flag disant si on a affaires avec une variable avec ou sans accolade
    char motif[MAX_LINE_SIZE];
    char motifRes[MAX_LINE_SIZE];

    while((c = strchr(str+i,'$')) != NULL && strlen(str) < max){
        
        memset(motif,0,sizeof(char)*MAX_LINE_SIZE); // On vide le motif pour la prochaine variable à traiter
        flagAccolade = 0; 
        i = c - str + 1; // On place le i à la prochaine occurence possible de $

        if (c[1] == '{'){ // Si on a une variable d'environnement avec {
            flagAccolade = 1;
            for (size = 0; c[size] != '}' && c[size] != ' ' && c[size] != '\0'; size++); // On récupère la taille du motif
            if (c[size] != '}') // Si le dernier caractère trouvé n'est pas un }, alors on ne traite pas la variable d'environnement 
                continue;
        }else{ 
            for (size = 0; c[size] != ' ' && c[size] != '\0'; size++); // On récupère la taille du motif
        }
        size--;
        memmove(motifRes,c+1+flagAccolade,size-flagAccolade); // Ici on ne compte pas la dernière } s'il y a
        memmove(motif,c,size+1+flagAccolade);

        if((resEnv = getenv(motifRes)) == NULL) // Si on ne trouve pas la variable d'environnement, on remplace le motif par rien
            replace(str,motif,"",MAX_LINE_SIZE);
        else
            replace(str,motif,resEnv,MAX_LINE_SIZE);
    }
    return 0;
}

/*
Fonction word_in_string : Fonction qui va prendre une chaines de caractère 'cmdline' et retourne un pointeur vers le dernier mot de 'cmdline'; ce pointeur sera stocké dans 'lastToken'
Paramètre cmdline : Pointeur vers une chaine de caractère qui pointe vers la chaine sur laquelle on effectue le travail de recherche du dernier mot
Retourne un pointeur vers le dernier mot
*/
char* word_in_string(char *cmdline){

    char cmdlinetmp1[MAX_LINE_SIZE];
    char *strToken;
    char *lastToken; // pointeur vers le dernier mot

    // On recupère le dernier mot saisie dans cmdline (mot distingué par l'intermédiaire de la séparation par espace)
    strcpy(cmdlinetmp1,cmdline);
    strToken = strtok (cmdlinetmp1," ");
    while (strToken != NULL) {
        lastToken = strToken;
        strToken = strtok(NULL," ");
    }
    if(lastToken == NULL){
        lastToken = cmdline;
    }
    // On retourne le pointeur vers le dernier mot
    return lastToken;
}

/*
Fonction word_in_string : Fonction qui va prendre une chaines de caractère 'lastToken' et permet d'afficher dans le répertoire actuel le nom des fichiers pour lesquels il y a le début de leur chaine qui correspond à la chaine 'lastToken', permet également de modifier les valeurs des variables utile au bon fonctionnement de la fonction completion
Paramètre lastToken : Pointeur vers une chaine de caractère qui correspond au mot à rechercher dans le répertoire actuel et de le comparer au nom des fichiers
Paramètre flag_pc : Pointeur vers un entier qui permet d'indiquer s'il y a une complétion possible (= 1 sinon 0)
Paramètre nb_pc : Pointeur vers un entier qui permet d'indiquer le nombre de complétion possible
Paramètre nb_c_pc : Pointeur vers un entier qui permet d'indiquer le nombre de caractères qui peuvent être complété (exemple si l'utilisateur saisie 'Fra' et qu'il existe un fichier 'France' alors nb_c_pc vaudra 4 (ance)) 
Paramètre word : Pointeur vers une chaine de caractère qui permettra de stocker le nom du dernier fichier correspondant aux critères décrit ci-dessus 
Retourne un code (erreur ou non)
*/
int available_file_cwd(char *lastToken, int *flag_pc, int *nb_pc, int *nb_c_pc, char* word){
    
    DIR *directory;
    struct dirent *dirent;
    char *str;
    int txt = 0; // Variable pour affichage unique de texte

    directory = opendir((char *)getenv("PWD"));

    // Pour tous les fichiers dans le répertoire actuel
    while((dirent = readdir(directory))!=NULL){
        // S'il y a un fichier qui correspond au nom du dernier mot de la commande (dernier token)
        if((str = strstr(dirent->d_name,lastToken))!=0){
            // Si le nom du fichier est plus grand que le mot (sinon la complétion est inutile) et que le début du nom du fichier correspond au mot alors on a trouvé une complétion possible
            if(strlen(dirent->d_name)!=strlen(lastToken) && dirent->d_name - str == 0){
                if(txt==0) {printf("  %sSuggestion de complétion\t",CMagenta);}
                printf("%s%s  ",CCyan,str); // On affiche les complétion possible
                (*flag_pc) = 1; // Une complétion est possible
                (*nb_pc)++; // On incrémente de 1 le nombre de complétion possible
                strcpy(word,dirent->d_name); // On place dans 'word' le mot qui peut être complété
                (*nb_c_pc) = strlen(word) - strlen(lastToken);  // On cherche le nombre de caractères qui peuvent être complété (exemple si l'utilisateur saisie 'Fra' et qu'il existe un fichier 'France' alors nb_c_pc vaudra 4 (ance))
                txt = 1; // E'vite de récrire plusieurs fois le message "Suggestion de complétion"
            }
        }
    }
    return 0;
}

// /*
// Fonction completion: Fonction qui va permettre de compléter un mot pendant la saisie d'une commande
// Paramètre cmline : Pointeur vers une chaine de caractère qui correspond à la commande qui est en train d'être saisie
// Retourne un code (erreur ou non)
// */
int completion(char* cmdline){

   char cmdlinetmp[MAX_LINE_SIZE] = "", // cmdline temporaire
        *lastToken = NULL, // Pointeur sur le dernier mot saisie par l'utilisateur
        word[MAX_LINE_SIZE]; //
    int flag_pc = 0, // 0 s'il n'y a pas de complétion possible, 1 sinon
        nb_pc = 0, // Nombre de complétion possible
        nb_c_pc = 0, // Nombre de caractères qui peuvent être complété (exemple si l'utilisateur saisie 'Fra' et qu'il existe un fichier 'France' alors nb_c_pc vaudra 4 (ance))
        nbcaracLastToken = 0; // Vaut la valeur de strlen(lastToken)

    // On recupère le dernier mot saisie dans 'cmdline' dans 'lastToken' (mot distingué par l'intermédiaire de la séparation par espace)
    lastToken =  word_in_string(cmdline);
    nbcaracLastToken = strlen(lastToken);

    // On va ici rechercher si dans le répertoire actuel il y a le début d'un nom de fichier qui correspond au mot récupéré dans 'lastToken'
    // Si c'est le cas flag_pc vaudra 1 en sortie de fonction
    available_file_cwd(lastToken,&flag_pc,&nb_pc,&nb_c_pc,word);

    // S'il y a une complétion possible du dernier mot tapé
    if(flag_pc == 1){
        // On vérifie les mots jusqu'à ce qu'il n'y a plus de complétion possible, ou bien jusqu'à ce que l'utilisateur tape 2 fois sur la touche Entrée consécutivement
        do{
            // S'il y a une seule complétion possible alors on peut compléter le mot
            if(nb_pc == 1){

                // On indique à l'utilisateur que s'il saisie Tab puis Entrée le mot qu'il est en train de taper sera complété puis on affiche le prompt
                printf("%s\nTapé sur tabulation puis entrée pour compléter\n%sAntonin@Simon: %s%s%s $ %s", CMagenta,CBleu, CVert, getenv("PWD"), CRouge, CNormal);
                
                // On affiche la commande qui est en train d'être saisie et dans la couleur jaune la suggestion de complétion
                fprintf(stdout,"%s%s",CNormal,cmdline);
                int pointer = strlen(word) - nb_c_pc; // Pointeur qui pointe sur les caractères qui peuvent être complété
                fprintf(stdout,"%s%s%s",CJaune,word+pointer ,CBlanc);

                // On demande à l'utilisateur de saisir son choix, Tab pour compléter automatique son mot, Entrée pour valider la saisie de sa commande
                if (fgets(cmdlinetmp, MAX_LINE_SIZE - 1, stdin) == NULL)
                    break;
                // Si l'utilisateur à saisie Tab
                if(strlen(cmdlinetmp)==2 && cmdlinetmp[0]=='\t' && cmdlinetmp[1]=='\n'){
                    cmdline[strlen(cmdline)-nbcaracLastToken]='\0'; // On retire le début de ce qui a doit être tabulé
                    strcat(cmdline,word); // On rajoute ce qui a été tabulé à cmdline
                    // Je veux pouvoir encore ecrire après une tabulation
                }
                else{
                    // Si l'utilisateur à saisie entrée
                    if(cmdlinetmp[0]=='\n')
                        break;
                    //else
                        // On continue l'exécution
                }

            }
            else{
                printf("\n");
            }

            // On reset les variables pour pouvoir continuer la saisie et pouvoir compléter à nouveau sur la même commande
            nb_pc = 0;
            flag_pc = 0;

            // On affiche le prompt avec la commande qui est en cours de saisie
            printf("%sAntonin@Simon: %s%s%s $ %s", CBleu, CVert, getenv("PWD"), CRouge, CNormal);
            fprintf(stdout,"%s",cmdline);

            // On continue la saisie de la commande
            if (fgets(cmdlinetmp, MAX_LINE_SIZE - 1, stdin) == NULL)
                return 1;
            cmdlinetmp[strlen(cmdlinetmp) - 1] = '\0';            
            // Si l'utilisateur à saisie Entrée sans nouveaux caractères il souhaite lancer sa commande, dans ce cas on quitte la saisie actuelle
            if(strlen(cmdlinetmp)==0) return 1;

            // On ajoute la saisie actuelle à la commande en cours de saisie
            strcat(cmdline,cmdlinetmp);

            // On recupère le dernier mot saisie dans 'cmdline' dans 'lastToken' (mot distingué par l'intermédiaire de la séparation par espace)
            lastToken = word_in_string(lastToken);
            nbcaracLastToken = strlen(lastToken);
            
            // On va ici rechercher si dans le répertoire actuel il y a le début d'un nom de fichier qui correspond au mot récupéré dans 'lastToken'
            // Si c'est le cas flag_pc vaudra 1 en sortie de fonction
            available_file_cwd(lastToken,&flag_pc,&nb_pc,&nb_c_pc,word);
            
        }while(flag_pc);
    }
    return 0;
}
