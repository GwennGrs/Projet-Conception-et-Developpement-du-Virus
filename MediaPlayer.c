#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

#define MAX 20

// Je cherche une fonction qui peut lire les images dans mon répertoire
int image(const char *fichier){
    const char *extension[] = {".jpg" , ".png", ".jpeg", ".bmp",NULL};
    for (int i = 0; extension[i] != NULL; i++){
        if(strstr(fichier, extension[i])){
            return 1;
        }
    } return 0;
}

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

int main(){
    char *fichiers[MAX+1];
    fct_tmp(fichiers);
    for (int j=0; fichiers[j] != NULL; j++){
            printf("F%d : %s\n", j, fichiers[j]);
    }
    
    /**Je libère la mémoire*/
    for(int i=0; fichiers[i] != NULL; i++){
        free(fichiers[i]);
    }
    
    return 0;
}
