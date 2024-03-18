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

struct Nod {
    struct Masina masina; // continut
    struct Nod* next; // legatura
};

// masini1.txt
/*

1,civic,2000,199.99
2,duster,2001,299.99
3,passat,1997,999.99
4,kuga,2008,543.99
5,kona,1999,673.99
6,talisman,2006,784.99

*/
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
    m->denumire = NULL;
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

struct Masina* citireFisier(const char* numeFisier, int* dimensiune) {
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
/////////////////////// Task 5:


void adaugaMasinaInLista(struct Nod** nod, struct Masina masina) {
    struct Nod* nodAdaugat = (struct Nod*) malloc(sizeof(struct Nod));
    nodAdaugat->masina = masina; // shallow copy, "mutam" masina in lista (daca modificam masina data din parametru se va modifica si din lista)
    nodAdaugat->next = NULL;

    if (*nod == NULL) {
        *nod = nodAdaugat;
    }
    else {
        struct Nod* temp = *nod;
        while (temp->next != NULL) { // iteram pana ajungem la ultimul nod
            temp = temp->next; 
        }
        temp->next = nodAdaugat;
    }

}

void afiseazaListaMasina(struct Nod* nod) {
    while (nod != NULL) {
        afiseazaMasina(nod->masina);
        nod = nod->next;
    }
}

void stergeMasinaDinLista(struct Nod** nod, int pozitie) {// dublu ** pentru ca lucram cu adresa de memorie a lui nod (s-ar putea sa modificam primul element)
    struct Nod* current = *nod;
    struct Nod* prev = *nod;

    int numarNoduri = 0;
    while (current != NULL) {
        numarNoduri++;
        current = current->next;
    }

    if (pozitie > numarNoduri) {
        return;
    }

    current = *nod; //reinitializam (dupa ce numaram nr de noduri)
    if (*nod == NULL || pozitie < 1) {
        return; // lista nu exista / index invalid
    }
    

    if (pozitie == 1) { // capul listei, lista contine 1 singur element si acela se vrea sters
        *nod = current->next; // NULL
        free(&(current->masina));
        return;
    }

    
    while (pozitie != 1) {
        prev = current;
        current = current->next;
        pozitie--;
    }
    

    prev->next = current->next;
    freeMasina(&(current->masina));


}



void main() {

    char numeFisier[50];
    int dimensiune = 0;

    printf("Introduceti numele fisierului:");
    scanf("%s", &numeFisier);

    struct Masina* m1 = NULL;
    m1 = citireFisier(numeFisier, &dimensiune);
 // afiseazaMasini(m1, dimensiune);

    struct Nod* nod = NULL;

    for (int i = 0; i < dimensiune; i++) {
        adaugaMasinaInLista(&nod, m1[i]);
    }

    stergeMasinaDinLista(&nod, 2);
    afiseazaListaMasina(nod);


    freeMasina(m1);
}