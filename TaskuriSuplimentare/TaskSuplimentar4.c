#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


struct Masina { // Magureanu Stefan -> M si S
    int cod;
    char* denumire;
    int anFabricatie;
    float pret;
};
void afiseazaMasina(struct Masina m) {
    printf("Cod: %d\n", m.cod);
    printf("Denumire: %s\n", m.denumire);
    printf("An de fabricatie: %d\n", m.anFabricatie);
    printf("Pret: %.2f\n", m.pret);
}

void afiseazaMasini(struct Masina* masini, int dimensiune) {
    for (int i = 0; i < dimensiune; i++) {
        afiseazaMasina(masini[i]);
    }
}
void freeMasina(struct Masina* m) {
    free(m->denumire);
    m->denumire= NULL;
    free(m);
}
void freeMasinaVector(struct Masina** m, int* dim) {
    for (int i = 0; i < (*dim); i++) {
        freeMasina(m[i]);
    }
    free(m);
   // free(dim);
   // dim = NULL;
}
/// ////////////////////////////////////////////////// Task 4:
struct Masina* adaugaMasina(struct Masina* vector, struct Masina masina, int* dimensiune) {
    struct Masina* copie = (struct Masina*)malloc(sizeof(struct Masina) * ((*dimensiune) + 1));
    
    if (copie == NULL) {
        return NULL;
    }

    if (vector != NULL) {

        for (int i = 0; i < (*dimensiune); i++) {
            copie[i] = vector[i];
        }
    }
    copie[(*dimensiune)] = masina;
    (*dimensiune)++;



    return copie;
}

struct Masina* citireFisier(const char* numeFisier,int *dimensiune) {
    struct Masina* vector = NULL;

    FILE* fp = fopen(numeFisier, "r");
    if (fp == NULL) {
        printf("Fisierul nu a fost deschis\n");
        return NULL;
    }
    else {
        (*dimensiune) = 0;
        char buffer[50];

        while (fgets(buffer, 49, fp) != NULL) {
            struct Masina m;
            char delimitator[] = ",\n";

            char* element = strtok(buffer, delimitator);
            m.cod = atoi(element);

            element = strtok(NULL, delimitator);
            m.denumire = malloc(sizeof(char) * (strlen(element) + 1));
            strcpy(m.denumire, element);

            element = strtok(NULL, delimitator);
            m.anFabricatie = atoi(element);

            element = strtok(NULL, delimitator);
            m.pret = atof(element); // string to float
            
            vector = adaugaMasina(vector, m, dimensiune);
            
        }
        fclose(fp);
    }
    return vector;
}

void scrieFisier(struct Masina* vector, const char* numeFisier, int* dimensiune) {
    FILE* fp = fopen(numeFisier, "w+");

    if (fp == NULL) {
        printf("Fisierul nu a fost deschis\n");
    }
    else {
        for (int i = 0; i < (*dimensiune); i++) {
            fprintf(fp, "%d,%s,%d,%.2f", vector[i].cod, vector[i].denumire, vector[i].anFabricatie, vector[i].pret);
            fprintf(fp, "\n");
        }
        fclose(fp);
    }

}





struct Masina** copieazaVectorInMatrice(struct Masina* vector, int* liniiMatrice, int* colMatrice, int* dimVector) {
    struct Masina** copie = (struct Masina**)malloc(sizeof(struct Masina*) * 3);

    /*
    asezam elementele din vector in functie de anul fabricatiei
    linia 0: anFabricatie < 2000
    linia 1: 2000 <= anFabricatie <= 2005
    linia 2: anFabricatie > 2005
    */

    (*liniiMatrice) = 3;
    int nrElementeLinie[] = { 0, 0, 0 };

    // calculam nr de elemente pentru fiecare linie
    for (int i = 0; i < (*dimVector); i++) {
        int linieElement = 0;

        if (vector[i].anFabricatie < 2000)
            linieElement = 1;
        else if (vector[i].anFabricatie >= 2000 && vector[i].anFabricatie <= 2500)
            linieElement = 2;
        else
            linieElement = 3;

        nrElementeLinie[linieElement]++;
    }

    (*colMatrice) = 0;

    for (int i = 0; i < (*liniiMatrice); i++) { // aflam maximul de coloane
        if (nrElementeLinie[i] > (*colMatrice)) {
            (*colMatrice) = nrElementeLinie[i];
        }
    }

    for (int i = 0; i < (*liniiMatrice); i++) {
        copie[i] = (struct Masina*)malloc(sizeof(struct Masina) * (*colMatrice));
    }

    int tempNrElementeLinie[] = { 0, 0, 0 };

    // copiem elementele vectorului pe pozitia corespunzatoare
    for (int i = 0; i < (*dimVector); i++) {
        int linieElement = 0;

        if (vector[i].anFabricatie < 2000)
            linieElement = 1;
        else if (vector[i].anFabricatie >= 2000 && vector[i].anFabricatie <= 2500)
            linieElement = 2;
        else
            linieElement = 3;

        int cod;
        char* denumire;
        int anFabricatie;
        float pret;


        copie[linieElement][tempNrElementeLinie[linieElement]].denumire = (char*)malloc(sizeof(char) * (strlen(vector[i].denumire) + 1));
        strcpy(copie[linieElement][tempNrElementeLinie[linieElement]].cod, vector[i].cod);
        copie[linieElement][tempNrElementeLinie[linieElement]].anFabricatie = vector[i].anFabricatie;
        copie[linieElement][tempNrElementeLinie[linieElement]].pret = vector[i].pret;

        tempNrElementeLinie[linieElement]++;
    }
}

struct Masina** sorteazaMatriceDupaLinii(struct Masina** matriceMasina, int liniiMatrice, int colMatrice) {
    struct Masina** copie = (struct Masina**)malloc(sizeof(struct Masina*) * liniiMatrice);

    for (int i = 0; i < liniiMatrice; i++) {
        copie[i] = (struct Masina*)malloc(sizeof(struct Masina) * colMatrice);
    }

    for (int i = 0; i < liniiMatrice; i++) {
        for (int j = 0; j < colMatrice; j++) {
            copie[i][j] = matriceMasina[i][j];
        }
    }

    for (int i = 0; i < liniiMatrice - 1; i++) {
        for (int j = i + 1; j < liniiMatrice; j++) {
            int nrElementeI = 0;
            int nrElementeJ = 0;

            for (int k = 0; k < colMatrice; k++) {
                if (copie[i][k].denumire) {
                    nrElementeI++;
                }

                if (copie[j][k].denumire) {
                    nrElementeJ++;
                }
            }

            if (nrElementeI < nrElementeJ) {
                struct Masina* temp = copie[i];
                copie[i] = copie[j];
                copie[j] = temp;
            }
        }
    }

    return copie;
}

void afiseazaMatrice(struct Masina** matriceMasina, int liniiMatrice, int colMatrice) {
    printf("Linii %d, coloane %d\n", liniiMatrice, colMatrice);
    for (int i = 0; i < liniiMatrice; i++) {
        for (int j = 0; j < colMatrice; j++) {
            if (matriceMasina[i][j].denumire) {
                printf("Elementul %d %d\n", i, j);
                afiseazaMasina(matriceMasina[i][j]);
            }
        }
        printf("\n");
    }
}

void main() {

    char numeFisier[50];
    int dimensiune = 0;

 

    
    printf("Introduceti numele fisierului:");
    scanf("%s", &numeFisier);

    struct Masina* m1 = NULL;
    
    m1 = citireFisier(numeFisier, &dimensiune);
    scrieFisier(m1, numeFisier, &dimensiune);

    afiseazaMasini(m1,dimensiune);
   
    freeMasina(m1);
    

}