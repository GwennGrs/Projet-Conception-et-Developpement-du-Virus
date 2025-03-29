#include <stdio.h>
main()
{
    FILE *fp1, *fp2;
    char ch;
  
    // ouvrir le fichier en lecture
    fp1 = fopen("pg1.c", "r");
    // ouvrir le fichier en écriture
    fp2 = fopen("file2.txt", "w");
  
    // Lire le contenu du fichier
    while((ch = getc(fp1)) != EOF)
        putc(ch, fp2);
  
    fclose(fp1);
    fclose(fp2);
}
