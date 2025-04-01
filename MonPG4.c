#include <stdio.h>
#include <stdlib.h>
#include <time.h>





typedef struct {
	int cartes[25]; 
    	int num_cartes;        
    	int total;             
    	float solde;           
} Joueur;


int tirer_carte() {
	int carte = (rand() % 13) + 1;
    	if (carte > 10) return 10; 
    	if (carte == 1) return 11; 
    	return carte;
}


int calcul_score(Joueur* j) {
	int score = 0;
    	int nb_ace = 0;

    	for (int i = 0; i < j->num_cartes; i++) { 
        	score += j->cartes[i];
        	if (j->cartes[i] == 11) { 
            		nb_ace++;
        	}
    	}


    	while (score > 21 && nb_ace > 0) {
        	score -= 10;
        	nb_ace--;
    	}

    	return score;
}


void afficher_main(Joueur* j, int cacher_premiere) {
    	if (cacher_premiere) {
        	printf("Carte cachée");
        	for (int i = 1; i < j->num_cartes; i++) {
            	printf(", %d", j->cartes[i]);
        	}
        	printf("\n");
    	} else {
        	for (int i = 0; i < j->num_cartes; i++) {
            	printf("%d", j->cartes[i]);
            	if (i < j->num_cartes - 1) printf(", ");
        	}
        	printf("\n");
    	}
}


void jouer_blackjack(float solde_initial) {
	Joueur joueur = {{0}, 0, 0, solde_initial}; 
    	Joueur croupier = {{0}, 0, 0, 0};           
    	char choix;
    	float mise;

    	while (joueur.solde > 0) {

        	joueur.num_cartes = 0;
        	croupier.num_cartes = 0;

        	printf("\nSolde actuel : %.2f\n", joueur.solde);
        	do {
            	printf("Entrez votre mise : ");
            	scanf("%f", &mise);
            	if (mise > joueur.solde) printf("Mise trop élevée !\n");
            	if (mise <= 0) printf("La mise doit être positive !\n");
        	} while (mise > joueur.solde || mise <= 0);


        	joueur.cartes[joueur.num_cartes++] = tirer_carte();
        	croupier.cartes[croupier.num_cartes++] = tirer_carte();
        	joueur.cartes[joueur.num_cartes++] = tirer_carte();
        	croupier.cartes[croupier.num_cartes++] = tirer_carte();


        	printf("\nVotre main : ");
        	afficher_main(&joueur, 0);
        	joueur.total = calcul_score(&joueur);
        	printf("Score : %d\n", joueur.total);
        	printf("Main du croupier : ");
        	afficher_main(&croupier, 1);

        	while (joueur.total < 21) {
            	printf("Voulez-vous une carte ? (o/n) : ");
            	scanf(" %c", &choix);
            	if (choix == 'n') break;
            	if (choix == 'o') {
                	joueur.cartes[joueur.num_cartes++] = tirer_carte();
                	printf("Votre main : ");
                	afficher_main(&joueur, 0);
                	joueur.total = calcul_score(&joueur);
                	printf("Score : %d\n", joueur.total);
            	}
        }

        	if (joueur.total > 21) {
            	printf("Vous avez dépassé 21 ! Vous perdez.\n");
            	joueur.solde -= mise;
            	continue;
        	}


        	printf("\nMain complète du croupier : ");
        	afficher_main(&croupier, 0);
        	croupier.total = calcul_score(&croupier);
        	printf("Score du croupier : %d\n", croupier.total);

        	while (croupier.total < 17) {
            	croupier.cartes[croupier.num_cartes++] = tirer_carte();
            	printf("Le croupier tire : ");
            	afficher_main(&croupier, 0);
            	croupier.total = calcul_score(&croupier);
            	printf("Score du croupier : %d\n", croupier.total);
        	}


        	if (croupier.total > 21) {
            	printf("Le croupier dépasse 21 ! Vous gagnez %.2f\n", mise);
            	joueur.solde += mise;
        	} else if (joueur.total > croupier.total) {
            	printf("Vous gagnez %.2f avec %d contre %d !\n", mise, joueur.total, croupier.total);
            	joueur.solde += mise;
        	} else if (joueur.total < croupier.total) {
            	printf("Vous perdez avec %d contre %d\n", joueur.total, croupier.total);
            	joueur.solde -= mise;
        	} else {
            	printf("Égalité à %d\n", joueur.total);
        	}

        	printf("Continuer ? (o/n) : ");
        	scanf(" %c", &choix);
        	if (choix == 'n') break;
    	}

    	printf("\nPartie terminée. Solde final : %.2f\n", joueur.solde);
}


int main() {

    float solde_initial;
    printf("Entrez votre solde initial : ");
    scanf("%f", &solde_initial);

    if (solde_initial <= 0) {
        printf("Le solde doit être positif !\n");
        return 1;
    }

    jouer_blackjack(solde_initial);
    return 0;
}
