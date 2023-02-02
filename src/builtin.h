/*
 
  Gestion des commandes internes du minishell.
 
  Fichier builtin.h : primitives des fonctions builtin
  Auteur : Sllmon & FameDoze2Point0
  Dépendances : processus.h
*/

#ifndef _BUILTIN_H
#define _BUILTIN_H

#include "processus.h"

int is_builtin(const char* cmd);
int builtin(processus_t* proc);
int cd(const char* path, int fderr);
int export(const char* var, const char* value, int fderr);
int unset_shell(const char* var, int fderr);
int exit_shell(int ret, int fderr);
int historique();
int type(const char* var, int fdout);

#endif
