#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

	int score = 0;
    	int nombre_questions;
    	int reponse;
    	time_t debut, fin;
    	double temps_total;

    	printf("Test ta vitesse (attention le chrono s'active après ton choix)\n");
    	printf("Choisis ton nombre d'épreuves \n");

    	scanf("%d", &nombre_questions);
    	debut = time(NULL);


    	for (int i = 0; i < nombre_questions; i++) {

        	int a = (rand() % 20) + 1;
        	int b = (rand() % 20) + 1;
        	int operation = rand() % 3; 
        	int resultat_attendu;


        	switch (operation) {
            	case 0: 
                	resultat_attendu = a + b;
                	printf("\nQuestion %d : %d + %d = ? ", i + 1, a, b);
                	break;
            	case 1: 
            	    	resultat_attendu = a - b;
                	printf("\nQuestion %d : %d - %d = ? ", i + 1, a, b);
                	break;
            	case 2: 
                	resultat_attendu = a * b;
                	printf("\nQuestion %d : %d × %d = ? ", i + 1, a, b);
                	break;
        	}


        	scanf("%d", &reponse);


        	if (reponse == resultat_attendu) {
            		printf("Correct !\n");
            		score++;
        	} else {
            		printf("Faux ! La réponse était %d.\n", resultat_attendu);
        	}
    }


    	fin = time(NULL);
    	temps_total = difftime(fin, debut);


    	printf("\nTest terminé !\n");
    	printf("Score : %d/5\n", score);
    	printf("Temps total : %.0f secondes\n", temps_total);


    	if (score == nombre_questions && temps_total < nombre_questions*3) {
        	printf(" sans faute et rapide !\n");
    	} else if (score == nombre_questions) {
        	printf(" sans faute mais plus de vitesse ne serait pas de refus!\n");
    	} else if (score >= nombre_questions*0.6) {
        	printf(" pas trop mal mais tu peux t'améliorer\n");
    	} else {
        	printf("abandonne les maths\n");
    	}

    	return 0;
}
