#include<stdio.h>
#include<stdlib.h>
/*#include<windows.h>*/
#include <unistd.h>

void Mapping(int Co[2], char Map[10][10], int Bloc[4], int Key[3], int vie[2], int V[1]);
int Blocage (int Co[2], int Objet[2][10], int Bloc , int j);
int Cles(int Co[2], int key[3], int Porte[2]);
int Combat(int Mo[5], int vie[2], int V[1]);

int main (){
    int Partie ;
    int D ;
    int M ;
    int i ;
    int V[1] ;
    int vie[2] ;
    int Bloc[4] ;
    int Co[2] ;
    int Key [3] ;
    char Map[10][10] ;

    M = 0 ;
    Co [0] = 1 ;
    Co [1] = 1 ;
    Key [2] = 0 ;
    Partie = 1 ;
    vie[0] = 10 ;
    for (i = 0 ; i < 4 ; i = i + 1){
            Bloc[i] = 0 ;
    }
    V[0] = 0 ;

    while ( Partie == 1){

    /* initialisation de la map */

    Mapping(Co, Map, Bloc, Key, vie, V);

    /* End Game */

    if (vie[0] == 0){
            Partie = 0;
    }


    /* Debug Mode */

  /* for (i = 0 ; i < 4 ; i = i + 1){
        printf("%d " , Bloc[i]);
    }
    printf("\n") ;
    for (i = 0 ; i < 2 ; i = i + 1){
        printf("%d " , Co[i]);
    }
    printf("\n") ; */

   /*Procédure Déplacement */

    printf("key : %d \n", Key[2]);
    printf("Pv : %d \n", vie[0]);
    printf("(pour arreter taper 0)Ou voulez vous vous déplacer :");
    scanf("%d", &D);

    switch (D) {
            case 8 :
                if (Bloc[1] == 1){
                        break ;
                }
                else {
                    Co[1] = Co[1] - 1;
                    if (Co[1] < 1) {
					Co[1] =  Co[1] + 1;
                    }
                    break;
                }
			case 6 :
			    if (Bloc[0] == 1){
                    break ;
			    }
			    else {
                    Co[0] = Co[0] + 1;
                    if (Co[0] > 8) {
					Co[0] = Co[0] - 1;
                    }
                    break;
			    }
			case 4 :
			    if (Bloc[3] == 1){
                    break ;
			    }
			    else {
                    Co[0] = Co[0] - 1;
                    if (Co[0] < 1) {
                    Co[0] = Co[0] + 1;
                    }
                    break;
			    }
			case 2 :
			    if (Bloc[2] == 1){
                    break ;
			    }
			    else {
                    Co[1] = Co[1] + 1;
                    if (Co[1] > 8) {
					Co[1] = Co[1] - 1;
                    }
				break;
			    }

            case 0 :
                Partie = 0;
                break;
        }

        system("clear"); /* "clear" sous linux et "cls" sous windows" */
    }
    return 0 ;
}


int Cles (int Co[2], int key[3], int Porte[2]){
    if (key[2] == 0){
        if ((key[0] == Co[0]) && (key[1] == Co[1])){
            key[2] = key[2] + 1;
        }
    }

    if ((Co [0] == Porte[0]) && (Co[1] == Porte[1] + 1) && (key[2] == 1)){
        key[2] = key[2] + 1;
    }
return key[2];
}

int Blocage (int Co[2], int Objet[2][10], int Bloc , int j){
    int move[4] ; /*0 = rien ; 0 = gauche ; 1 = haut ; 2 = bas ; 3 = droite */
    int i;
    for (i=0 ; i < 4 ; i = i + 1){
        move[i] = 0 ;
    }
    if ((Co[0] == Objet[0][j]-1) && (Co[1] == Objet[1][j])){
        move[0] = 1 ;
    }
    if ((Co[0] == Objet[0][j]+1) && (Co[1] == Objet[1][j])){
        move[3] = 1 ;
    }
    if ((Co[0] == Objet[0][j]) && (Co[1] == Objet[1][j]-1)){
        move[2] = 1 ;
    }
    if ((Co[0] == Objet[0][j]) && (Co[1] == Objet[1][j]+1)){
        move[1] = 1 ;
    }
return move[Bloc];
}

void Mapping(int Co[2],char Map[10][10],int Bloc[4] , int Key[3], int vie[2], int V[1]){

    int i ;
    int j ;

    /* Initialisation d'objets ou d'enemies */

    int Roche[2][10] ;
    int Arbre[2][10] ;
    int Piege[2][3] ;
    int Porte[2] ;
    int Monstre[5] ;

    /* Les bordures et fond de map */

    for(j = 0 ; j < 10 ; j = j + 1){
        for(i = 0 ; i < 10 ; i = i + 1){
            Map[j][i] = ' ' ;
        }
    }

    for(j = 0 ; j < 10 ; j = j + 1){
        for(i = 0 ; i < 10 ; i = i + 1){
            if ((j == 0) ||( j == 9)){
                Map[j][i] = 'X' ;
            }
            else if ((i == 0) || (i == 9)) {
                Map[j][i] = 'X' ;
            }
        }
    }

    /* Clés et système */

    Key[0] = 4 ;
    Key[1] = 4 ;
    if (Key[2] == 0){Map[4][4] = 'K' ;}

    Key[2] = Cles(Co , Key , Porte);

    /* Positionnement  */

        /* Piège */

        Piege[0][0] = 1 ;
        Piege[1][0] = 8 ;

        Piege[0][1] = 1 ;
        Piege[1][1] = 7 ;

        for(j = 0 ; j < 2; j = j + 1){
        if ((Co[1] == Piege[1][j]) && (Co[0] == Piege[0][j])){
                Co[1] = 1 ;
                Co[0] = 1 ;
                vie[0] = vie[0] - 1 ;
        }
        }

        /* Enemies */

        Monstre[0] = 2 ;
        Monstre[1] = 3 ;

        Monstre[2] = 5 ;

        Monstre[3] = 2 ;
        Monstre[4] = 1 ;

        if ( V[0] == 0){
          Map[Monstre[1]][Monstre[0]] = 'M' ;
          if ((Co[1] == Monstre[1]) && (Co[0] == Monstre [0]) && (Monstre[2] > 0)){
            printf("combat");
            Combat(Monstre, vie, V);
          }
        }
        else{
          Map[Monstre[1]][Monstre[0]] = ' ' ;
        }

        /* Joueur */

        Map[Co[1]][Co[0]] = 'q' ;

        /* Objet */

        Roche[0][0] = 7 ;
        Roche[1][0] = 7 ;

        Roche[0][1] = 7 ;
        Roche[1][1] = 7 ;

        Arbre[0][0] = 6 ;
        Arbre[1][0] = 6 ;

        Arbre[0][1] = 5 ;
        Arbre[1][1] = 6 ;

        for(j = 0 ; j < 2 ; j = j + 1){
            i = 0 ;
            Map[Arbre[i + 1][j]][Arbre[i][j]] = 'A';
            Map[Roche[i + 1][j]][Roche[i][j]] = 'R';
            Map[Piege[i + 1][j]][Piege[i][j]] = 'T';
        }

    /* Bloc */

    for (i = 0; i < 4 ; i = i + 1){
        for (j = 0 ; j < 2 ; j = j + 1){
            Bloc[i] = Blocage(Co, Arbre, i, j) + Blocage(Co , Roche, i , j) ;
                if (Bloc[i] != 0){
                        break ;
                }
        }
    }

    /* MAPPING */

    for (j = 0; j < 10 ; j = j + 1){
        for (i = 0 ; i < 10 ; i = i + 1) {
            printf("%c ", Map[j][i]);

        }
        printf("\n");
    }
}

int Combat(int Mo[5], int vie[2], int V[1]){
	int i ;
	int j ;
	int t ;
	int delta ;
	int AD ; /* si = 1 atk ; si = 2 def */
	int Joueur[2] ;

	t = 1 ;

	Joueur[0] = 3 ;
	Joueur[1] = 1 ;

	system("clear");

	printf("Un monstre apparait !");

	sleep(1) ;  /* unsigned int sleep (unsigned int nb_sec); */

	system("clear") ;

	for(i = 0 ; i < 10 ; i = i + 1){
		for(j = 0 ; j < 10 ; j = j + 1){
			printf("X ");
		}
		printf("\n");
	}

	system("clear");

	while ((Mo[2] > 0) && (vie[0] > 0)){
    for(j = 0 ; j < 6 ; j = j + 1){
			for(i = 0 ; i < 10 ; i = i + 1){
        if (((i == 0) || (i == 9)) && (j < 4) ){
					printf("X ");
				}
				else if((j == 0) || (j == 3)){
					printf("X ");
				}
				else if((i == 2) && (j == 2)){
					printf("q ");
				}
				else if((i == 7) && (j == 2)){
					printf("M ");
				}
				else if((i == 0) && (j == 4)){
					printf("Pv:%d " , vie[0]);
				}
				else if((i == 5) && (j == 4)){
					printf("Pv:%d " , Mo[2]);
				}
				else {
				printf("  ");
				}
				if(t == 1){
          sleep(0) ;
				}
			}
			printf("\n");
		}


		printf("Tour %d \n" , t);
		printf("Attaquer (1) et Defendre (2) :");
		scanf("%d" , &AD);

		if (AD == 1){
			if ( Joueur[0] - Mo[4] <= 0){
				printf("Ca n'a aucun effet sur M");
			}
			else {
			delta = (Joueur[0] - Mo[4]) ;
			Mo[2] = Mo[2] - delta ;
			printf("Le perso attaque M et lui enlève %d de vie \n", delta);
			}
		}
		if (AD == 2){
			Joueur[1] = Joueur[1] * 2 ;
		}

		delta = Mo[3] - Joueur[1] ;
		vie[0] = vie[0] - delta ;
		printf ("Le monstre vous attaque et vous enleve %d Hp \n", delta) ;

		sleep(4);
		t = t + 1 ;
		system ("clear") ;
	}
	V[0] = 1 ;
	system("clear");
	return V[0] ;
}
