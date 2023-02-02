/*
 
  Gestion des processus (implémentation).
 
  Fichier processus.c : Fonctions gérant les processus de manière générale (lancement, initiation, les descripteurs)
  Auteur : Sllmon & FameDoze2Point0
  Dépendances : processus.h builtin.h sys/wait.h
 */

#include "processus.h"
#include "builtin.h"
#include <sys/wait.h>

#define CRouge "\x1B[31m"
#define CNormal "\x1B[0m"

/*
Fonction handler : fonction effectué lorsqu'un fils a terminé sa commande et qu'elle était mise en arrière plan
*/
void handler(){
	fprintf(stdout,"\nTERMINÉ\n");
}

/*
Fonction exec_process : Fonction qui permet de lancer les différentes commandes demandées
	Paramètre p : Processus que nous voulons exécuter
	retourne le code retour de builtin ou le code de retour de la commande effectué (0 = bon fonctionnement)
*/
int exec_process(processus_t* p) {

	if(is_builtin(p->path)!=1){ 

		//si la commande n'est pas une builtin
		if((p->pid = fork())==0){

			dup2(p->stdin, 0);
			dup2(p->stdout, 1);
			dup2(p->stderr, 2);

			for (int * fd = p->fdclose; *fd != -1; ++fd) close(*fd);
			
			execvp(p->path,p->argv);
			//la commande n'a pas été effectuée car inconnue ou potentiel problème avec des arguments 
        	fprintf(stderr,"La commande %s'%s'%s a echoué\n", CRouge, p->path, CNormal);
			exit(1); // le fils s'arrête
		}else{

			if (p->wait == 1){ // si le processus était en arrière plan, nous allons dire que dès que le fils a fini, il fait la fonction handler

				fprintf(stdout,"[%d] : DEBUT\n",p->pid);
				signal(SIGCHLD, handler);
			}else{

				signal(SIGCHLD,NULL); // on dit que lorsque le fils a terminé, il ne fait rien du tout
				if (p->wait == 0) // si la commande se fait en avant plan, nous attendons qu'elle ait fini
					waitpid(p->pid,&p->status,0);
				
			}	
			// Nous supprimons les descripteurs utilisé par la commande (sauf si c'est 0 1 2, sinon ça poserait problème pour les autres)
			if (p->stdin != 0 && p->stdin != 1 && p->stdin != 2) close(p->stdin);
			if (p->stdout != 0 && p->stdout != 1 && p->stdout != 2) close(p->stdout);
			if (p->stderr != 0 && p->stderr != 1 && p->stderr != 2) close(p->stderr);
		}
	}else{
		return builtin(p);
	}
	return p->status; 
}

/*
Fonction init_process : Fonction qui permet d'initialiser les valeurs d'un processus
	Paramètre p : processus que nous voulons initialiser
*/
int init_process(processus_t* p) {
	
	p[0].pid = 0;
	p[0].status = 0;
	p[0].stdin = 0;
	p[0].stdout = 1;
	p[0].stderr = 2;
	p[0].wait = 0;
	p[0].neg = 0;
	p[0].path = NULL;
	for (int j = 0; j < MAX_CMD_SIZE; j++){
		p[0].argv[j] = NULL;
		p[0].fdclose[j] = -1;
	}
	p[0].next = NULL;
	p[0].next_success = NULL;
	p[0].next_failure = NULL;
  return 0;
}

/*
Fonction add_fd : Fonction qui permet d'ajouter un descripteur à notre tableau des descripteurs enregistrés de notre processus
	Paramètre fdclose : Tableaux des descripteurs du processus
	Paramètre fd : descripteur que nous voulons ajouter au tableau
	retourne 0 si le descripteur a été ajouté sinon 1
*/
int add_fd(int* fdclose, int fd){
	int i = 0, flag = 1;

	while(fdclose[i] != -1){
		if(fdclose[i]==fd){
			flag = 0;
			break;
		}
		i++;
	}
	if(flag){
		fdclose[i] = fd;
		return 0;
	}else 
		return 1;
}

/*
Fonction merge_fdclose : Fonction qui permet de synchroniser tous les tableaux de descripteurs des processus 
	Paramètre fdclose : Tableaux des descripteurs du processus 1
	Paramètre fdclose1 : Tableaux des descripteurs du processus 2
	retourne 0 
*/
int merge_fdclose(int *fdclose, int * fdclose1){
	
	//on mets les descripteurs de fdclose1 dans fdclose (s'il n'est pas déjà présent)
	for (int i = 0; fdclose[i] != -1 ; i++)
		add_fd(fdclose1,fdclose[i]);

	//on mets les descripteurs de fdclose dans fdclose1 (s'il n'est pas déjà présent)	
	for (int i = 0; fdclose1[i] != -1 ; i++)
		add_fd(fdclose,fdclose1[i]);
	
	return 0;
}

/*
Fonction parse_cmd : Fonction qui permet de traduire une ligne de commande en processus
	Paramètre tokens : Ligne de commande découpée en tokens
	Paramètre processes : Tableaux des processus
	Paramètre max : Le nombre de processus maximum
	retourne -1 en cas d'erreur sinon 0
*/
int parse_cmd(char* tokens[], processus_t* processes, size_t max) {

	size_t 	idx_arg = 0, // indice de l'argument
			idx_tok = 0, // indice du token
			idx_proc = 0, // nombre de processus (commande)
			nbToken = 0; // nombre de token

	for (nbToken = 0; tokens[nbToken] != NULL; nbToken++); // boucle qui compte le nombre de token que nous avons
	
	for (idx_tok = 0; tokens[idx_tok] != NULL && idx_proc < max; idx_tok++){

		if (strcmp(";", tokens[idx_tok]) == 0 || strcmp("||", tokens[idx_tok]) == 0 || strcmp("&&", tokens[idx_tok]) == 0 || strcmp("|", tokens[idx_tok]) == 0){
				
			if (strcmp(";", tokens[idx_tok])==0){ // Commande suivante
				if (idx_tok == nbToken - 1) // S'il n' y a plus de commande a exécuté
					processes[idx_proc].next=NULL;
				else
					processes[idx_proc].next=&processes[idx_proc+1];
			}
			
			if (strcmp("||", tokens[idx_tok]) == 0){  // Commande suivante si failure
				if(processes[idx_proc].neg!=1)
					processes[idx_proc].next_failure=&processes[idx_proc+1]; 
				else
					processes[idx_proc].next_success=&processes[idx_proc+1]; 
			}
			
			if (strcmp("&&", tokens[idx_tok]) == 0){ // Commande suivante si succes
				if(processes[idx_proc].neg!=1)
					processes[idx_proc].next_success=&processes[idx_proc+1]; 
				else
					processes[idx_proc].next_failure=&processes[idx_proc+1]; 
			}
			
			if(strcmp("|", tokens[idx_tok]) == 0) {//commande s'il y a un pipe

				processes[idx_proc].next = &processes[idx_proc+1]; // on considère le pipe comme un ";" spécial
				processes[idx_proc].argv[idx_arg]= NULL;

				int tube[2];
				pipe(tube);
				processes[idx_proc].stdout=tube[1];
				processes[idx_proc+1].stdin=tube[0];

				add_fd(processes[idx_proc].fdclose, tube[1]);
				add_fd(processes[idx_proc+1].fdclose, tube[0]);

				//--------------Autre façon de faire----------------------------
				// possible car A | B = A > temp; B < temp
				 
				// int tempOut = open(".temp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
				// int tempIn = open(".temp", O_RDONLY, 0644);
				// if (tempOut == -1 || tempIn == -1) {
				// 	perror("Erreur de redirection :");
				// 	return -1;
				// }

				// processes[idx_proc].stdout=tempOut;
				// processes[idx_proc+1].stdin=tempIn;
				// add_fd(processes[idx_proc].fdclose, tempOut);
				// add_fd(processes[idx_proc+1].fdclose, tempIn);
			}
			++idx_proc;
			idx_arg=0;
			continue; // Token traité
		}
			
		if (strcmp(">", tokens[idx_tok]) == 0) {
			int fdout = open(tokens[idx_tok+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fdout==-1) {
				perror("Erreur de redirection :");
				return -1;
			}
			processes[idx_proc].stdout=fdout;
			add_fd(processes[idx_proc].fdclose, fdout);
			idx_tok++; // Pour prendre en compte le nom de fichier 
			continue; // Token traité
		}
		if (strcmp(">>", tokens[idx_tok]) == 0) {
			int fdout = open(tokens[idx_tok+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fdout==-1) {
				perror("Erreur de redirection :");
				return -1;
			}
			processes[idx_proc].stdout=fdout;
			add_fd(processes[idx_proc].fdclose, fdout);
			idx_tok++; // Pour prendre en compte le nom de fichier 
			continue; // Token traité
		}
		if (strcmp("2>", tokens[idx_tok]) == 0) {
			int fderr = open(tokens[idx_tok+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fderr==-1) {
				perror("Erreur de redirection :");
				return -1;
			}
			processes[idx_proc].stderr=fderr;
			add_fd(processes[idx_proc].fdclose, fderr);
			idx_tok++; // Pour prendre en compte le nom de fichier
			continue; // Token traité 
		}
		if (strcmp("2>>", tokens[idx_tok]) == 0) {
			int fderr = open(tokens[idx_tok+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fderr==-1) {
				perror("Erreur de redirection :");
				return -1;
			}
			processes[idx_proc].stderr=fderr;
			add_fd(processes[idx_proc].fdclose, fderr);
			idx_tok++; // Pour prendre en compte le nom de fichier
			continue; // Token traité 
		}
		if (strcmp("<", tokens[idx_tok]) == 0) {
			int fdin = open(tokens[idx_tok+1], O_RDONLY, 0644);
			if (fdin==-1) {
				perror("Erreur de redirection :");
				return -1;
			}
			processes[idx_proc].stdin=fdin;
			add_fd(processes[idx_proc].fdclose, fdin);
			idx_tok++; // Pour prendre en compte le nom de fichier
			continue; // Token traité 
		}
		if (strcmp("<<", tokens[idx_tok]) == 0) {
			
			char saisi[MAX_LINE_SIZE];

			int fdout = open(".temp",O_WRONLY | O_CREAT | O_TRUNC, 0644);
			
			int fdin = open(".temp", O_RDONLY, 0644);
			if (fdout == -1 || fdin == -1) {
				perror("Erreur de redirection :");
				return -1;
			}
			
			do{
				fprintf(stdout,"> ");
				if (fgets(saisi,MAX_LINE_SIZE - 1,stdin) == NULL)
					break;

				if (strncmp(saisi,tokens[idx_tok+1],strlen(tokens[idx_tok+1])) == 0 && strlen(saisi)-1 == strlen(tokens[idx_tok+1]))
					break;
				write(fdout,saisi,strlen(saisi));
			} while (1);

			processes[idx_proc].stdin=fdin;
			add_fd(processes[idx_proc].fdclose, fdin);
			close(fdout);
			idx_tok++; // Pour prendre en compte le nom de fichier
			continue; // Token traité 

		}
		if (strcmp("<<<",tokens[idx_tok]) == 0) {

			char saisi[MAX_LINE_SIZE];
			char c;
			size_t i, flag = 1;
			int fdout = open(".temp2",O_WRONLY | O_CREAT | O_TRUNC, 0644);
			int fdin = open(".temp2", O_RDONLY, 0644);

			if (fdout == -1 || fdin == -1) {
				perror("Erreur de redirection :");
				return -1;
			}
			
			c = tokens[idx_tok + 1][0]; // on recupère la première lettre de l'argument après <<<
			if ( c == ';') { // ; est toujours à la fin, cela signifie qu'il n'y a pas d'argument derrière <<<
				write(fdout,"\n",1);
			}else if (c != '\"' && c != '\''){ // si nous avons ni " ni ' alors nous affichons normalement la chaine
				write(fdout,tokens[idx_tok + 1],strlen(tokens[idx_tok + 1]));
				write(fdout,"\n",1);
			}else{ // nous sommes dans le cas où nous avons soit " soit '
				// Nous regardons d'abord ce qu'il y a dans l'argument après <<<
				for (i = 1; i < strlen(tokens[idx_tok + 1]) && flag; i++) {
					if (tokens[idx_tok +1][i] == c)
						flag = 0;
					else
						write(fdout,&tokens[idx_tok + 1][i],1);
				}
				// nous revenons à la ligne
				write(fdout,"\n",1);
				while (flag) { //on met flag à la place de "1" pour faire office de test avant même qu'on rentre dedans par rapport à ce qu'il y a avant

					fprintf(stdout,"> ");
					if (fgets(saisi,MAX_LINE_SIZE - 1,stdin) == NULL)
						break;

					if (strchr(saisi,c) != NULL) {  // si on a écrit au moins une fois " ou ' (dépend des cas) alors on s'arrête en mettant le flag à 0
						flag = 0;
						for (size_t j = 0; j < strlen(saisi); j++){
							if (saisi[j] == c){
								memmove(saisi+j,saisi+j+1,strlen(saisi)-j-1);
								saisi[strlen(saisi)-1] = '\0';
							}	
						}
						
					}
						
					write(fdout,saisi,strlen(saisi));
				}
			}

			processes[idx_proc].stdin=fdin;
			add_fd(processes[idx_proc].fdclose, fdin);
			close(fdout);
			idx_tok++; // Pour prendre en compte le nom de fichier
			continue; // Token traité
		}
		if (strcmp(">&2", tokens[idx_tok]) == 0 || strcmp("1>&2", tokens[idx_tok]) == 0) {
			//redirection de stdout vers stderr
			processes[idx_proc].stdout=processes[idx_proc].stderr;
			continue; // Token traité
		}
		if (strcmp("2>&1", tokens[idx_tok]) == 0) {
			// redirection de stderr vers stdout
			processes[idx_proc].stderr=processes[idx_proc].stdout;
			continue; // Token traité
		}
		if (strcmp("&", tokens[idx_tok]) == 0) {
			processes[idx_proc].wait = 1;
			continue; // Token traité
		}
		if (strcmp("!", tokens[idx_tok]) == 0 && idx_arg == 0) {
			processes[idx_proc].neg = 1;
			continue; // Token traité
		}
		
		//Si nouveau processes
		if(!idx_arg){
			processes[idx_proc].path = tokens[idx_tok]; 
		}
		processes[idx_proc].argv[idx_arg] = tokens[idx_tok];
		idx_arg++;
	}
	// dans la première boucle, nous mettons tous les descripteurs connus dans le tableau du premier processus
	for (size_t i = 1; i < idx_proc; i++)
		merge_fdclose(processes[0].fdclose,processes[i].fdclose);

	// dans la deuxième boucle, vu que le tableau du premier processus a tous les descripteurs, on va les mettres dans chaque processus
	for (size_t i = 1; i < idx_proc; i++)
		merge_fdclose(processes[0].fdclose,processes[i].fdclose);
	
	return 0;
}

/*
Fonction file_process : Fonction récursive qui permet de lancer les différentes commandes les unes après les autres si à un moment donné la fonction retourne 1, tous les processus sont annulés
	Paramètre current : Processus que nous voulons lancer
	Paramètre val : paramètre permettant de savoir si nous avons effectuer un ET (&&) ou un OU (||)
					0 = ET &&
					1 = OU ||
	Retourne :
		- 1 s'il n'y a pas/plus de commande à faire
		- le code erreur de la commande effectuée
		- 777 si nous voulons voir l'historique des commandes effectuées
*/
int file_process(processus_t* current, int val){

	// Si l'appel retourne 1 
	int erreur = 0;
	//condition d'arret
	if(current == NULL)
		return 1; // on s'arrête
	else{

		if(val != 1){
			erreur = exec_process(current);
			if(erreur)
				return erreur;	
		}

		if (current->next!=NULL) {  

			if(file_process(current->next,0))
				return 1;
			else
				return 0;

		}else if (current->next_success!=NULL) {  // et

			if(file_process(current->next_success,0))
				return 1;
			else
				return 0;

		}else if (current->next_failure!=NULL) {

			if(file_process(current->next_failure,1))
				return 1;
			else
				return 0;
		} else 
			file_process(NULL,0);
		
		return 1; // plus de commande à effectuer
	}
	return 0;
}