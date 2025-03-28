#include <gtk/gtk.h>

/*
typedef struct {
    GList *images;
    GList *current;
    GtkWidget *image_widget;
} AppData;
*/
GList *images;
GList *current;
GtkWidget *image_widget;

static void load_image(AppData *data) {
    if (data->current) {
        gtk_image_set_from_file(GTK_IMAGE(data->image_widget), (char *)data->current->data);
    }
}

static void prev_image(GtkWidget *widget, AppData *data) {
    if (data->current && data->current->prev) {
        data->current = data->current->prev;
        load_image(data);
    }
}

static void next_image(GtkWidget *widget, AppData *data) {
    if (data->current && data->current->next) {
        data->current = data->current->next;
        load_image(data);
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GList *images = NULL;
    images = g_list_append(images, "aristide.png");
    images = g_list_append(images, "banane.jpg");

    AppData data = {images, images, gtk_image_new()};

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Images");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), data.image_widget, TRUE, TRUE, 0);

    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 0);

    GtkWidget *prev = gtk_button_new_with_label("Précédent");
    g_signal_connect(prev, "clicked", G_CALLBACK(prev_image), &data);
    gtk_box_pack_start(GTK_BOX(button_box), prev, TRUE, TRUE, 0);

    GtkWidget *next = gtk_button_new_with_label("Suivant");
    g_signal_connect(next, "clicked", G_CALLBACK(next_image), &data);
    gtk_box_pack_start(GTK_BOX(button_box), next, TRUE, TRUE, 0);

    load_image(&data);
    gtk_widget_show_all(window);
    gtk_main();

    g_list_free(images);
    return 0;
}
