#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>

double masse;       
double longueur_corde;    
double angle;       
double vitesse = 0.0; 
double gravite;           

gboolean dessine(GtkWidget *widget, cairo_t *cr, gpointer data) {
	guint largeur, hauteur;
	largeur = gtk_widget_get_allocated_width(widget);
    
	hauteur = gtk_widget_get_allocated_height(widget);
    
	double centre_x = largeur / 2.0;
    
	double centre_y = 0; 
	double echelle = 100.0; 

	double x_masse = centre_x + longueur_corde * echelle * sin(angle);
	double y_masse = centre_y + longueur_corde * echelle * cos(angle);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 2.0);
	cairo_move_to(cr, centre_x, centre_y);
	cairo_line_to(cr, x_masse, y_masse);
	cairo_stroke(cr);

        double rayon_masse = masse * 5.0 + 5.0; 
        if (rayon_masse > 20.0) rayon_masse = 20.0; 
        cairo_set_source_rgb(cr, 1, 0, 0);
        cairo_arc(cr, x_masse, y_masse, rayon_masse, 0, 2 * G_PI);
        cairo_fill(cr);

        return FALSE;
}

gboolean update_pendule(gpointer data) {
        GtkWidget *widget = GTK_WIDGET(data);

        double acceleration = -gravite / longueur_corde * sin(angle);
        double dt = 0.02; 
        vitesse += acceleration * dt;
        angle += vitesse * dt;
        vitesse *= 0.995;

        gtk_widget_queue_draw(widget);

        return TRUE;
}

int main(int argc, char *argv[]) {
        int choix_gravite;
        double angle_degres;

        printf("Entrez la masse du pendule (en kg) : ");
        scanf("%lf", &masse);
        printf("Entrez la longueur de la corde (en mètres) : ");
        scanf("%lf", &longueur_corde);
        printf("Entrez l'angle initial du pendule (entre -90 et 90) : ");
        scanf("%lf", &angle_degres);
        angle = angle_degres * G_PI / 180.0; 

        printf("Entrez la gravité (1 pour la Terre, 2 pour la Lune, 3 pour un intermédiaire) : ");
        scanf("%d", &choix_gravite);

        switch (choix_gravite) {
            case 1: gravite = 9.81; break;
            case 2: gravite = 1.62; break;
            case 3: gravite = 5.0;  break;
        }

        gtk_init(&argc, &argv);

        GtkWidget *fenetre;
    	GtkWidget *zone_dessin;
    	fenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    	gtk_window_set_title(GTK_WINDOW(fenetre), "Simulation de Pendule");
    	gtk_container_set_border_width(GTK_CONTAINER(fenetre), 10);
    	g_signal_connect(G_OBJECT(fenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    	zone_dessin = gtk_drawing_area_new();
    	gtk_widget_set_size_request(zone_dessin, 1920, 1080);
    	gtk_container_add(GTK_CONTAINER(fenetre), zone_dessin);
    	g_signal_connect(G_OBJECT(zone_dessin), "draw", G_CALLBACK(dessine), NULL);

    	g_timeout_add(20, update_pendule, zone_dessin);

    	gtk_widget_show_all(fenetre);
    	gtk_main();

    	return 0;
}
