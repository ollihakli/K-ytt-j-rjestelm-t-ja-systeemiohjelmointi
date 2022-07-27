#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 50

typedef struct rivi { //Tietue linkitetylle listalle
    char teksti[MAX];
    struct rivi *pSeuraava;
} RIVI;

RIVI *vapautaMuisti(RIVI *pA) {
    RIVI *ptr = pA;
    while (ptr != NULL) {
        pA = ptr->pSeuraava;
        free(ptr);
        ptr = pA;
    }
    return(pA);
}

void tulostus(RIVI *pA) {
    RIVI *ptr = pA;
    while (ptr != NULL) {
        printf("%s", ptr->teksti);
        ptr = ptr->pSeuraava;
    }
}

RIVI *tiedostoLuku(char tiedostoNimi[], RIVI *pA, RIVI *pL) {
    //RIVI *ptr = pA;
    RIVI *pU;
    char row[MAX];

    FILE *tiedosto; 
    if ((tiedosto = fopen(tiedostoNimi, "r")) == NULL) {
        fprintf(stderr, "error: cannot open file '%s'\n", tiedostoNimi);
        exit(1);
    }
    while (fgets(row, MAX, tiedosto) != NULL) { //Tiedoston luku
        //printf("%s", row);
        if ((pU = (RIVI*)malloc(sizeof(RIVI))) == NULL ){
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        strcpy(pU->teksti, row);
        pU->pSeuraava = NULL;
        if(pA == NULL) {
            pA = pU;
            pL = pU;
        }
        else {
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
        if (i == 1) { //Lisätään rivin vaihtomerkki ensimmäisen tiedon jälkeen.
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
    //char row[MAX];
    RIVI *pAlku = NULL, *pLoppu = NULL; // Osoittimia listan käyttöön
    //RIVI *pUusi, *ptr; // Apuosoitin muistin varaukseen ja liukuri

    //strcpy(inputfile, argv[1]);
    //strcpy(outputfile, argv[2]);

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
    /*FILE *tiedostoLuku; //Tiedoston luku
    tiedostoLuku = fopen(inputfile, "r"); 
    while (fgets(row, 50, tiedostoLuku) != NULL) {
        printf("%s", row);
        if ((pUusi = (RIVI*)malloc(sizeof(RIVI))) == NULL ){
            perror("Muistin varaus epäonnistui");
            exit(1);
        }
        strcpy(pUusi->teksti, row);
        pUusi->pSeuraava = NULL;
        if(pAlku == NULL) {
            pAlku = pUusi;
            pLoppu = pUusi;
        }
        else {
            pLoppu->pSeuraava = pUusi;
            pLoppu = pUusi;
        }
    }    
    fclose(tiedostoLuku);*/

    /*// Listan läpikäynti
    ptr = pAlku;
    while (ptr != NULL) {
        printf("%s", ptr->teksti);
        ptr = ptr->pSeuraava;
    }*/

    /*FILE *tiedostoKirjoitus; // Tiedoston kirjoitus
    tiedostoKirjoitus = fopen(outputfile, "w");
    fgets(row, 50, stdin);
    row[strlen(row)-1] = '\0';
    fprintf(tiedostoKirjoitus, "%s\n", row);
    fclose(tiedostoKirjoitus);*/

    /*// Muistin vapauttaminen 
    ptr = pAlku;
    while (ptr != NULL) {
        pAlku = ptr->pSeuraava;
        free(ptr);
        ptr = pAlku;
    }*/
    //Lopetus
    pAlku = vapautaMuisti(pAlku);
    return(0); 
}

/*
Lähteet:
https://quescol.com/interview-preparation/c-program-to-reverse-a-linked-list
*/
