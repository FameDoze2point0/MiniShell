/*
  
  Gestion des processus.
 
  Fichier processus.h : primitives des fonctions gérant les processus de manière générale (lancement, initiation, les descripteurs)
  Auteur : Auteur : Sllmon & FameDoze2Point0
  Dépendances : signal.h unistd.h stdio.h string.h sys/types.h sys/stat.h fcntl.h stdlib.h
 */

#ifndef _PROCESSUS_H
#define _PROCESSUS_H

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#include "parser.h"


typedef struct processus_t {
  pid_t pid;
  int status;
  int stdin, stdout, stderr;
  int wait;
  int neg;
  char* path;
  char* argv[MAX_CMD_SIZE];
  int fdclose[MAX_CMD_SIZE];
  struct processus_t* next;
  struct processus_t* next_success;
  struct processus_t* next_failure;
} processus_t;

int exec_process(processus_t* p);
int init_process(processus_t* p);
int parse_cmd(char* tokens[], processus_t* processes, size_t max);
int add_fd(int* fdclose, int fd);
int merge_fdclose(int *fdclose, int * fdclose1);
int file_process(processus_t* current, int val);
void handler();

#endif
