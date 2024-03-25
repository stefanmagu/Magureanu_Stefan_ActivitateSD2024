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
struct Masina* masiniPeste2000(struct Nod** nod, int* dimensiune) { // salvare in vector
    *dimensiune = 0; // dimensiune vector
    struct Nod* temp = *nod;

    while (temp != NULL) {

        if (temp->masina.anFabricatie > 2000) {
            (*dimensiune)++;
        }
        temp = temp->next;
    }

    struct Masina* m = (struct Masina*)malloc(sizeof(struct Masina) * (*dimensiune));
    int poz = 0;
    temp = *nod;

    while (temp != NULL) { // deep copy
        if (temp->masina.anFabricatie > 2000) {
            m[poz].anFabricatie = temp->masina.anFabricatie;
            m[poz].cod = temp->masina.cod;

            m[poz].denumire = (char*)malloc(sizeof(char) * (strlen(temp->masina.denumire) + 1));
            strcpy(m[poz].denumire, temp->masina.denumire);

            m[poz].pret = temp->masina.pret;
            poz++;
        }
        temp = temp->next;
    }

    return m;
}


void interschimbareNoduri(struct Nod* n1, struct Nod* n2) { // interschimbare oricare 2 noduri

    struct Masina temp;
    temp.anFabricatie = n1->masina.anFabricatie;
    temp.cod = n1->masina.cod;
    temp.denumire = (char*)malloc(sizeof(char) * (strlen(n1->masina.denumire) + 1));
    strcpy(temp.denumire, n1->masina.denumire);
    temp.pret = n1->masina.pret;

    n1->masina.anFabricatie = n2->masina.anFabricatie;
    n1->masina.cod = n2->masina.cod;
    n1->masina.denumire = (char*)malloc(sizeof(char) * (strlen(n2->masina.denumire) + 1));
    strcpy(n1->masina.denumire, n2->masina.denumire);
    n1->masina.pret = n2->masina.pret;

    n2->masina.anFabricatie = temp.anFabricatie;
    n2->masina.cod = temp.cod;
    n2->masina.denumire = (char*)malloc(sizeof(char) * (strlen(temp.denumire) + 1));
    strcpy(n2->masina.denumire, temp.denumire);
    n2->masina.pret = temp.pret;


}

void interschimbareElementeLista(struct Nod** nod, int pozitie1, int pozitie2) {
    int dimensiune = 0; // dimensiune vector
    struct Nod* temp1 = *nod;
    struct Nod* temp2 = *nod;

    while (temp1 != NULL) {
        dimensiune++;
        temp1 = temp1->next;
    }

    if (pozitie1 > dimensiune || pozitie2 > dimensiune || pozitie1 < 0 || pozitie2 < 0 || pozitie1 == pozitie2 || *nod == NULL) {
        return;
    }

    temp1 = *nod;

    for (int i = 0; i < pozitie1 - 1; i++) {
        temp1 = temp1->next;
    }
    for (int i = 0; i < pozitie2 - 1; i++) {
        temp2 = temp2->next;
    }

    // interschimbam:
    interschimbareNoduri(temp1, temp2);

}

void freeNod(struct Nod* nod) {
    struct Nod* current = nod;
    struct Nod* next;

    while (current != NULL) {
        next = current->next;
        free(current->masina.denumire);
        free(current);
        current = next;
    }

    nod = NULL;
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

    int dimVector = 0;
    struct Masina* m = NULL;
    m = masiniPeste2000(&nod, &dimVector);

    printf("\n\n");

    afiseazaMasini(m, dimVector);

    printf("\n\n");

    interschimbareElementeLista(&nod, 3, 5);
    afiseazaListaMasina(nod);


    printf("\n\n");



    freeNod(nod);
    // freeMasinaVector(&m, &dimVector);
}