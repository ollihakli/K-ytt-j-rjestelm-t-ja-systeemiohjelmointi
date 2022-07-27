/* Olli Häkli my.grep.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 50

void search(char tiedostoNimi[MAX], char sana[MAX]) {
    char *ret;
    char *buffer; // lähde: https://linuxhint.com/getline-function-c/
    size_t bufsize = 32;
    size_t characters;
    buffer = (char *)malloc(bufsize * sizeof(char));
    if (buffer == NULL) {
        printf("error: malloc failed");
        exit(1);
    }
    FILE *tiedosto;
    if ((tiedosto = fopen(tiedostoNimi, "r")) == NULL) {
        fprintf(stderr, "my-cat: cannot open file\n");
            exit(1);
    }
    while (characters = getline(&buffer, &bufsize, tiedosto) != -1) {
        if ((ret = strstr(buffer, sana)) != NULL) {
            printf("%s", buffer);
            free(buffer);
        }
    }
    fclose(tiedosto);
}

//Pääohjelma
int main (int argc, char *argv[]) {
    
    //Muuttujien alustus jä määrittely
    char searchTerm[MAX];
    char inputfile[MAX];
    char row[MAX];
    char word[MAX];
    //Toiminnallinen osuus
    if (argc < 2) {
        printf("my-grep: searchterm [file ...]");
        exit(1);
    } else if (argc == 2) {
        strcpy(word, argv[1]);
        fgets(row, MAX, stdin);
        char *ret;
        char *buffer; // lähde: https://linuxhint.com/getline-function-c/
        size_t bufsize = 32;
        size_t characters;
        buffer = (char *)malloc(bufsize * sizeof(char));
        if (buffer == NULL) {
            printf("error: malloc failed");
            exit(1);
        }
        if ((ret = strstr(row, word)) != NULL) {
            printf("%s", row);
        }
    } else if (argc >= 3) {
        int i = 3;
        while (i <= argc) {
            strcpy(inputfile, argv[i-1]);
            strcpy(word, argv[1]);
            search(inputfile, word);
            i++;
        }
    } else {
        exit(1);
    }

    printf("Kiitos");
    return(0);
}
/*EOF*/
