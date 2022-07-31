/* Olli Häkli reverse.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 1000

typedef struct rivi { //Tietue linkitetylle listalle
    char teksti[MAX];
    struct rivi *pSeuraava;
} RIVI;

RIVI *vapautaMuisti(RIVI *pA) { //Muistin vapautus
    RIVI *ptr = pA;
    while (ptr != NULL) {
        pA = ptr->pSeuraava;
        free(ptr);
        ptr = pA;
    }
    return(pA);
}

void tulostus(RIVI *pA) { //Aliohjelma tekstin tulostukseen näytölle
    RIVI *ptr = pA;
    int i = 1;
    while (ptr != NULL) {
        printf("%s", ptr->teksti);
        if (i == 1) { //Lisätään rivinvaihtomerkki ensimmäisen tiedon jälkeen.
            printf("\n");
        }
        ptr = ptr->pSeuraava;
        i++;
    }
}

RIVI *tiedostoLuku(char tiedostoNimi[], RIVI *pA, RIVI *pL) {
    RIVI *pU;
    char row[MAX];

    FILE *tiedosto; 
    if ((tiedosto = fopen(tiedostoNimi, "r")) == NULL) { //Tiedoston avaus
        fprintf(stderr, "error: cannot open file '%s'\n", tiedostoNimi);
        exit(1);
    }
    while (fgets(row, MAX, tiedosto) != NULL) { //Tiedoston luku
        if ((pU = (RIVI*)malloc(sizeof(RIVI))) == NULL ){
            fprintf(stderr, "malloc failed\n");
            exit(1); 
        }
        strcpy(pU->teksti, row); //Tietojen lisäys linkitettyyn listaan
        pU->pSeuraava = NULL;
        if(pA == NULL) { //Jos lista tyhjä, lisätään alkuun
            pA = pU;
            pL = pU;
        }
        else { //Muuten listan loppuun
            pL->pSeuraava = pU;
            pL = pU;
        }
    }    
    fclose(tiedosto);
    return(pA);
}

RIVI *tiedostoKirjoitus(char tiedostoNimi[MAX], RIVI *pA) {
    RIVI *ptr = pA;
    char row[MAX];
    int i = 1;
    FILE *tiedosto; // Tiedoston kirjoitus
    if ((tiedosto = fopen(tiedostoNimi, "w")) == NULL) {
        fprintf(stderr, "error: cannot open file'%s'\n", tiedostoNimi);
        exit(1);
    }
    while (ptr != NULL) {
        strcpy(row, ptr->teksti);
        fprintf(tiedosto, "%s", row);
        if (i == 1) { //Lisätään rivinvaihtomerkki ensimmäisen tiedon jälkeen.
            fprintf(tiedosto, "\n");
        }
        ptr = ptr->pSeuraava;
        i++;
    }
    fclose(tiedosto);
    return(pA);
}

RIVI *listanKaanto(RIVI* pA) { 
    RIVI *pEdellinen, *ptr;
    if(pA != NULL) {
        pEdellinen = pA;
        ptr = pA->pSeuraava;
        pA = pA->pSeuraava;
        pEdellinen->pSeuraava = NULL;
        while(pA != NULL) {
            pA = pA->pSeuraava;
            ptr->pSeuraava = pEdellinen;
            pEdellinen = ptr;
            ptr = pA;
        }
        pA=pEdellinen;
    }
    return(pA);
}

//Pääohjelma
int main (int argc, char *argv[]) {

    //muuttujien alustus ja määrittely
    char inputfile[MAX];
    char outputfile[MAX];
    RIVI *pAlku = NULL, *pLoppu = NULL; // Osoittimia listan käyttöön

    //Toiminnallinen osuus
    if (argc < 2) {
        fgets(inputfile, MAX, stdin);
        inputfile[strlen(inputfile)-1] = '\0';
        pAlku = tiedostoLuku(inputfile, pAlku, pLoppu);
        pAlku = listanKaanto(pAlku);
        tulostus(pAlku);
    }
    else if (argc == 2) {
        strcpy(inputfile, argv[1]);
        pAlku = tiedostoLuku(inputfile, pAlku, pLoppu);
        pAlku = listanKaanto(pAlku);
        tulostus(pAlku);

    }
    else if (argc == 3) {
        strcpy(inputfile, argv[1]);
        strcpy(outputfile, argv[2]);
        if (strcmp(inputfile, outputfile)== 0) { // Testataan ovatko tiedoston nimet samat
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }
        pAlku = tiedostoLuku(inputfile, pAlku, pLoppu);
        pAlku = listanKaanto(pAlku);
        pAlku = tiedostoKirjoitus(outputfile, pAlku);
    }
    else {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }
    //Lopetus
    pAlku = vapautaMuisti(pAlku);
    return(0); 
}

/*
Lähteet:
Linkitetyn listan kääntämiseen käytetty lähdettä: https://quescol.com/interview-preparation/c-program-to-reverse-a-linked-list
*/

/*EOF*/
