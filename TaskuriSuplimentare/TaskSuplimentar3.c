#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>



struct Masina { // Magureanu Stefan -> M si S
    int cod;
    char* denumire;
    int anFabricatie;
    float pret;
};

void scrieInFisier(char numeFisier[50], int nrMasini, struct Masina* masini) {

    FILE* fp = fopen(numeFisier, "w+");
    fprintf(fp, "%d\n", nrMasini);

    for (int i = 0; i < nrMasini; i++) {
        printf("Introduceti codul masinii %d: ", i + 1);
        scanf("%d", &masini[i].cod);

        masini[i].denumire = (char*)malloc(50 * sizeof(char));

        printf("Introduceti denumirea masinii %d:", i + 1);
        getchar(); // mancam enter-ul
        scanf("%s", masini[i].denumire);

        printf("Introduceti anul fabricatiei masinii %d:", i + 1);
        scanf("%d", &masini[i].anFabricatie);

        printf("Introduceti pretul masinii %d:", i + 1);
        scanf("%f", &masini[i].pret);

        fprintf(fp, "%d %s %d %.2f\n", masini[i].cod, masini[i].denumire, masini[i].anFabricatie, masini[i].pret);
    }
    fclose(fp);
}

void salveazaMasina(struct Masina* masina, int index,char numeFisier[50]) {
    FILE* fp = fopen(numeFisier, "w+");
    if (fp == NULL) {
        printf("Eroare la deschiderea fisierului\n");
        exit(1);
    }

    fprintf(fp, "%d\n", masina[index].cod);
    fprintf(fp, "%s\n", masina[index].denumire);
    fprintf(fp, "%d\n", masina[index].anFabricatie);
    fprintf(fp, "%.2f\n", masina[index].pret);

    fclose(fp);
}

void citesteDinFisier(char numeFisier[50],int *nrMasini, struct Masina* masini) {
    FILE* fp = fopen(numeFisier, "r+");

    fscanf(fp,"%d", &*nrMasini);

    for (int i = 0; i < *nrMasini; i++) {
        fscanf(fp,"%d", &masini[i].cod);
        if (masini[i].denumire != NULL) {
            free(masini[i].denumire);
            masini[i].denumire = NULL;
        }
        masini[i].denumire = (char*)malloc(50 * sizeof(char));
        fscanf(fp,"%s", masini[i].denumire);
        fscanf(fp,"%d", &masini[i].anFabricatie);
        fscanf(fp,"%f", &masini[i].pret);

    }

    fclose(fp);

}

void afisareMasini(struct Masina* masini, int nrMasini) {
    for (int i = 0; i < nrMasini; i++) {
        printf("%d, %s, %d, %.2f\n", masini[i].cod, masini[i].denumire, masini[i].anFabricatie, masini[i].pret);
    }

}



void main() {
    char numeFisier[50];
    printf("Introduceti numele fisierlui:");
    scanf("%s", &numeFisier);
   
    int nrMasini;
    printf("Introduceti nr de masini(>10):");
    scanf("%d", &nrMasini);

    struct Masina* masini = (struct Masina*)malloc(sizeof(struct Masina) * nrMasini);



    scrieInFisier(numeFisier, nrMasini, masini);
    citesteDinFisier(numeFisier,&nrMasini,masini);
    afisareMasini(masini, nrMasini);


    for (int i = 0; i < nrMasini; i++) {
        free(masini[i].denumire);
        masini[i].denumire = NULL;
    }
    free(masini);


}