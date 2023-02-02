# MiniShell

## Auteurs : 
@Sllmon 

@FameDoze2Point0

## Informations sur le minishell
Listes des fonctionnalités que nous avons implémentés :
    Built-in :
    
        - cd : permet de se déplacer dans les répertoires
                cd [chemin]
                [chemin] : obligatoire
                
        - export : permet d'initialiser une variable d'environnement ou de changer la valeur de [nom]
                export [nom]=[value]
                [nom]   :   obligatoire
                [value] :   obligatoire

        - unset : permet de supprimer une variable d'environnement
                unset [nom]
                [nom]   :   obligatoire

        - exit : permet de quitter le terminal shell
                exit [numero]
                [numero] : option

        - historique : permet d'afficher l'historique des dernières commandes utilisées (dans une certaines limites) et de lancer une commande à nouveau ou non
                historique

        - type : permet de savoir si une commande est builtin ou non
                type [commande]
                [commande] : obligatoire

    Redirection : 
        - > 
        - >> 
        - 2> 
        - 2>>
        - >&2
        - 2>&1
        - <
        - |
        - << (BONUS)
        - <<< (BONUS)
        
       [IMPORTANT] Il n'est pas nécessaire de mettre des espaces autour des redirections, le programme les reconnait tout seul

    - commande en avant plan (sans '&')

    - commande en arrière plan (avec '&')

    - le lancement de plusieurs commandes à la fois via (possibilité d'en combiner plusieurs):
        - ;
        - &&
        - ||
        
        [IMPORTANT] Il n'est pas nécessaire de mettre des espaces autour, le programme les reconnait tout seul

    - l'utilisation des variables d'environnement
            À noter que nous pouvons faire de deux façons différentes :
                - export var=universite (sans guillemets)
                - export var="universite de picardie jules vernes" (avec guillemets et donc espaces possibles)

            Dans les deux cas, tout doit être "collé" au caractère '='. De plus pour la seconde méthode, il faut qu'il y ait au moins un caractère autre que ' ' (espace) après " ou '

    - l'ajout d'un historique (BONUS)
            L'historique ne renvoie que les 10 dernières commandes déjà utilisées (si nous avons fais que 3 commandes, alors il n'y aura que 3 par exemple)


            En tapant "historique" dans le terminal, les dernières commandes vont apparaître avec un numéro.
            Vous n'aurez qu'à entrer le numéro de la commande voulue ou -1 pour quitter l'historique.
            À noter qu'une sécurité a été effectuée, vous pouvez entrer une valeur interdite cela ne pose pas de problème.

            Suite à ça, vous pourrez compléter la commande que vous avez choisie (uniquement en ajout, pas de suppression possible);

    - l'ajout de l'auto-complétion (BONUS)
            L'autocomplétion ne fonctionne que sur les fichiers.
            
            Imaginons qu'on ait 3 fichiers : test.txt test2.txt tomate.txt
            Si nous faisons : cat t 
            puis que nous tapons sur entrée/return, nous allons voir les différentes possibilité de complétion (soit les 3 fichiers)
            
            J'ai 2 possibilités :
                - Je retape entrée/return : je valide donc la commande "cat t"
                - Je continue ma saisi, si nous entrons donc "cat toma"

            J'aurais à nouveau 2 possibilités : 
                - Taper entrée/return pour valider ma saisie
                - Taper tabulation puis entrée/return pour valider l'autocomplétion. (Cela n'est possible que lorsque nous n'avons qu'un choix possible)
            
    - l'ajout de couleurs dans le terminal (BONUS)

    - l'ajout du dossier courant dans le prompt (cwd) (BONUS)
    
Il est important de noter que notre shell fonctionne mieux sur Ubuntu (y compris WSL) que sur MacOS. En effet, l'auto-complétion comporte un petit bug sur la version MacOS. De plus, pour certaines commandes, vous risquez de ne pas avoir le même résultat sur Ubuntu et sur MacOS, nous pensons que cela est dû au fait que bash et zsh gèrent différement certaines commandes.

## Exemple de commandes

  CD : 
  
    cd ..
    
    cd /
    
    cd ../../..

  EXIT :
  
    exit
    
    exit 3
    
  Commandes simples :
  
    touch file
  
    ls file
    
    rm file

  Commande arrière plan :
    
    echo bonjour &
    
    sleep 5 &

  Utilisation de plusieurs commandes en même temps:
    
    echo salut;echo     bonjour;  echo hello &

  Redirections :
    
    echo BONJOUR > file
    
    cat < file
    
    ls AAAAAA 2> file.error
    
    cat < file.error
    
    echo BONJOUR > file2
    
    echo HELLO >> file2
    
    cat < file2
    
    echo Bonsoir >&2
    
    ls AAAAAA 2>&1
    
    ls AAAAA 1> test 2>&1 ; 
    
    cat test

  Utilisation des pipes/tubes :
    
    echo abcdefg | grep a | wc
    
    echo abcdefg | wc -l
    
    ls -al | head -n 2 | tr '[a-z]' '[A-Z]'

  Composition de toutes ces redirections :
    
    echo bonjour > file;cat < file | wc > file2

  Utilisation des variables d'environnement :
    
    export var="universite de picardie jules verne"
    
    echo $var
    
    unset var

  Utilisation des ET/&& et OU/||
    
    echo test &&echo d ecriture||echo  test;
    
    echo test||echo d ecriture||echo  test2;
    
    echo test||echo d ecriture&&echo  test2;

  inversion de valeur de retour "!"
    
    ! echo test||echo d ecriture&&echo  test;
    
    ! echo test||!echo d ecriture||echo  test2||echo test3 && echo test4;

  [Information] : Pour les commandes << et <<<, il est impératif de les rentrer manuellement pour un meilleur rendu visuel

  commande "tag" <<

    cat << a
    >Salut
    >les
    >amis
    >a

    cat << a > fichier
    >Salut
    >les
    >amis
    >a

    cat << a > fichier; cat < fichier
    >Salut
    >les
    >amis
    >a

    cat << EOF | tr '[a-z]' '[A-Z]'
    >Bonjour
    >monsieur
    >Dupont
    >EOF

  commande "<<<"
    
    cat <<< "Ceci
    >est
    >un
    >exemple"

    cat <<< bonjour | tr o a

    cat <<< "k | sort -u | wc
    >b
    >c
    >a
    >a"c

  historique
  
    historique

  [Information] : complétion automatique
    Vous pouvez consulter la documentation pour savoir comment utiliser la complétion automatique ainsi que tout autre commande
