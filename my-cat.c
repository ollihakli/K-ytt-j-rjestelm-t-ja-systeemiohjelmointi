/* Olli Häkli my-cat.c*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 200

int main(int argc, char *argv[]) {

    //Muuttujien määrittely
    char inputfile[MAX];
    char row[MAX];
    int i = 1;

    //Toiminnallinen osuus
    if (argc < 2) { //Ei luettavaa tiedostoa, lopetus
        exit(0);
    }

    while (i < argc) {

        strcpy(inputfile, argv[i]);
        
        FILE *tiedosto;
        if((tiedosto = fopen(inputfile, "r")) == NULL) { // Tiedoston avaus
            fprintf(stderr, "my-cat: cannot open file\n");
            exit(1);
        }

        while (fgets(row, MAX, tiedosto) != NULL) {
            printf("%s", row);
        }
        fclose(tiedosto);
        i++;
    }
    printf("Kiitos");
    return(0);
}
/*EOF*/
