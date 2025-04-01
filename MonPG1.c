#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>

typedef struct {
	int coefA, coefB, constante;
    	double racine1, racine2;
    	int nombreRacines;
} Fonction;

Fonction fonction;

void calculRacines(Fonction *fonc) {
    	double delta = pow(fonc->coefB, 2) - (4 * fonc->coefA * fonc->constante);
    	if (delta > 0) {
        	fonc->racine1 = (-fonc->coefB - sqrt(delta)) / (2.0 * fonc->coefA);
        	fonc->racine2 = (-fonc->coefB + sqrt(delta)) / (2.0 * fonc->coefA);
        	fonc->nombreRacines = 2;
    	} else if (delta == 0) {
        	fonc->racine1 = -fonc->coefB / (2.0 * fonc->coefA);
        	fonc->nombreRacines = 1;
    	} else {
        	fonc->nombreRacines = 0;
    	}
}

static gboolean dessinCourbe(GtkWidget *widget, cairo_t *cr, gpointer valeurs_utilisateur) {
    	Fonction *fonc = (Fonction *)valeurs_utilisateur;
    	int largeur = gtk_widget_get_allocated_width(widget);
    	int hauteur = gtk_widget_get_allocated_height(widget);

    	cairo_set_source_rgb(cr, 1, 1, 1);
    	cairo_paint(cr);

    	//Axe X,Y
    	cairo_set_source_rgb(cr, 0, 0, 0);
    	cairo_set_line_width(cr, 1.0);
    	cairo_move_to(cr, 0, hauteur / 2);
    	cairo_line_to(cr, largeur, hauteur / 2);
    	cairo_move_to(cr, largeur / 2, 0);
    	cairo_line_to(cr, largeur / 2, hauteur);
    	cairo_stroke(cr);

    	//Permet de convertir les valeurs obtenus en emplacement sur le graphique
    	cairo_set_source_rgb(cr, 0, 0, 1);
    	double axeX = 20.0;
    	double axeY = 20.0;
    
    	//Place au bon endroit
    	cairo_move_to(cr, 0, hauteur / 2 - (fonc->constante / axeY));

    	//Dessine la courbe
    	for (int xPixel = 0; xPixel <= largeur; xPixel++) {
        	double x = (xPixel - largeur / 2) / axeX;
        	double y = fonc->coefA * x * x + fonc->coefB * x + fonc->constante;
        	double yPixel = hauteur / 2 - (y * axeY);
        	cairo_line_to(cr, xPixel, yPixel);
    	}
    	cairo_stroke(cr);

    	return FALSE;
}

static void activate(GtkApplication *app) {
    	GtkWidget *fenetre, *boite, *etiquetteResultat, *zoneDessin;
    	char resultat[256];

    	fenetre = gtk_application_window_new(app);
    	gtk_window_set_title(GTK_WINDOW(fenetre), "Quadratique");
    	gtk_window_set_default_size(GTK_WINDOW(fenetre), 400, 400);


    	boite = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 
    	gtk_container_add(GTK_CONTAINER(fenetre), boite);


    	if (fonction.nombreRacines == 2) {
        	snprintf(resultat, sizeof(resultat), "Équations: %dx² + %dx + %d\nRacines: %.2f et %.2f", fonction.coefA, fonction.coefB, fonction.constante, fonction.racine1, fonction.racine2);
    	} else if (fonction.nombreRacines == 1) {
        	snprintf(resultat, sizeof(resultat), "Équations: %dx² + %dx + %d\nRacine: %.2f", fonction.coefA, fonction.coefB, fonction.constante, fonction.racine1);
    	} else {
        	snprintf(resultat, sizeof(resultat), "Équations: %dx² + %dx + %d\nPas de racines réelles", fonction.coefA, fonction.coefB, fonction.constante);
    	}


    	etiquetteResultat = gtk_label_new(resultat);
    	gtk_box_pack_start(GTK_BOX(boite), etiquetteResultat, FALSE, FALSE, 0);


    	zoneDessin = gtk_drawing_area_new();
    	gtk_widget_set_size_request(zoneDessin, 400, 300);
    	gtk_box_pack_start(GTK_BOX(boite), zoneDessin, TRUE, TRUE, 0);

    	g_signal_connect(zoneDessin, "draw", G_CALLBACK(dessinCourbe), &fonction);

    	gtk_widget_show_all(fenetre);
}

int main(int argc, char **argv) {
    	printf("Entrez le coefficient a: ");
    	scanf("%d", &fonction.coefA);
    	printf("Entrez le coefficient b: ");
    	scanf("%d", &fonction.coefB);
    	printf("Entrez la constante c: ");
    	scanf("%d", &fonction.constante);

    	calculRacines(&fonction);

    	GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    	int status = g_application_run(G_APPLICATION(app), argc, argv);
    	g_object_unref(app);

    	return status;
}
