/*
 
  Parsing de la ligne de commandes utilisateur.
 
  Fichier parser.h : primitives des fonctions gérant les chaines de caractères (parsing des commandes) + le système d'autocomplétion
  Auteur : Sllmon & FameDoze2Point0
  Dépendances : stddef.h stdio.h stdlib.h stdbool.h string.h dirent.h
 */

#ifndef _PARSER_H
#define _PARSER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>

#define MAX_LINE_SIZE   1024
#define MAX_CMD_SIZE    256
#define SIZE_STRING_MAX 1024

// Implémentation de couleurs
#define CNormal "\x1B[0m"
#define CRouge "\x1B[31m"
#define CVert "\x1B[32m"
#define CJaune "\x1B[33m"
#define CBleu "\x1B[34m"
#define CMagenta "\x1B[35m"
#define CCyan "\x1B[36m"
#define CBlanc "\x1B[37m"


int countc(const char* str, char c);
int countc_s(const char * str, const char * c);
int searchc(const char* str,const char c, char ** position, int max);
int searchc_s(const char* str,const char * c, char ** position, int max);
int replace(char* str, const char* motif, const char* Ntab, int max);
int trim(char* str);
int clean(char* str);
int strcut(char * str, char sep, char **token, size_t max);
int searchstr(char ** tab, const char* motif);
int substenv(char* str, size_t max);
int searchc_multi(const char *str, const char *c, char **position, int max);
// separate_s équivaut à separate_multi
int separate_multi(char *str, const char *c, size_t max);
char* word_in_string(char *cmdline);
int available_file_cwd(char *lastToken, int *flag_pc, int *nb_pc, int *nb_c_pc, char* word);
int completion(char* cmdline);


#endif