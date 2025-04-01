#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <gtk/gtk.h>





char phrase_morse[300];
    
       
     
char *morse[] = {".-", "-...", "-.-.", "-..", ".","..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};
     
     

void foncConvertion(const char *phrase){
	for( int i =0; i< strlen(phrase); i++){
		if(phrase[i] == ' ') {
			strcat(phrase_morse,"             ");
		}
		else if (isalpha(phrase[i])) {
			char c = tolower(phrase[i]);
			strcat(phrase_morse,morse[c - 'a']);
			if (i< strlen(phrase) - 1 && phrase[i+1] != ' '){
				strcat(phrase_morse, "   ");
			}
		}
	}

}

static void activate(GtkApplication *app){
	GtkWidget *window;
	GtkWidget *result;

    	window = gtk_application_window_new(app);
    	gtk_window_set_title(GTK_WINDOW(window),"Convertisseur Morse");
    	gtk_window_set_default_size(GTK_WINDOW(window), 350,100);
    	
    	result = gtk_label_new(phrase_morse);
    	gtk_container_add(GTK_CONTAINER(window), result);
    	
    	gtk_widget_show_all(window);
   
    } 	

  
int main(int argc, char **argv) {
    	char phrase[100];


    	printf("Veuillez entrer votre phrase : ");
    	fgets(phrase, sizeof(phrase), stdin);


    	foncConvertion(phrase);


    	GtkApplication *app = gtk_application_new("org.gtk.morse", G_APPLICATION_FLAGS_NONE);
    	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    	int status = g_application_run(G_APPLICATION(app), argc, argv);
    	g_object_unref(app);

    	return status;
}

