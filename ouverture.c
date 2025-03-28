#include <stdlib.h>
#include <gtk/gtk.h>

/* GtkWidget chargerImage(char* image){
	return pImage;
}; */

int main(int argc, char **argv)
{
    GtkWidget *pWindow;
    GtkWidget *pVBox;
    GtkWidget *pImage;
    GtkWidget *pQuitImage;
    GtkWidget *pQuitBtn;
 
 
    gtk_init(&argc, &argv);
 
 /* Declaration des variables */
char **nomsImages = (char **)malloc(3 * sizeof(char *)); 
 	nomsImages[0] = "banane.jpg";
 	nomsImages[1] = "aristide.png";
 	nomsImages[2] = NULL;

printf("File : %s", nomsImages[0]);
 
    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 200);
    gtk_window_set_title(GTK_WINDOW(pWindow), "GtkImage");
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);
 
    pVBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(pWindow), pVBox);
 
    /* Chargement d'une image à partir d'un fichier */
    pImage = gtk_image_new_from_file(nomsImages[0]);
    gtk_box_pack_start(GTK_BOX(pVBox), pImage, FALSE, FALSE, 5);
    
    /* UNREF */
    
    /* Destruction du widget (marche pas) 
    GtkWidget *parent = gtk_widget_get_parent(pImage);
    parent = gtk_widget_get_parent(parent);
    gtk_widget_destroy(parent); */
    
    pQuitBtn = gtk_button_new();
    gtk_box_pack_start(GTK_BOX(pVBox), pQuitBtn, TRUE, FALSE, 5);
    g_signal_connect(G_OBJECT(pQuitBtn), "clicked", G_CALLBACK(gtk_main_quit), NULL);
 
    /* Chargement d'une image à partir d'un GtkStockItem */
    pQuitImage = gtk_image_new_from_stock(GTK_STOCK_QUIT, GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_container_add(GTK_CONTAINER(pQuitBtn), pQuitImage);
 
    gtk_widget_show_all(pWindow);
 
    gtk_main();
 
    return EXIT_SUCCESS;
}
