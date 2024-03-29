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

struct Nod { // lista dublu inlantuita

    struct Nod* prev; // legatura prev
    struct Masina masina; // continut
    struct Nod* next; // legatura next
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
/////////////////////// Task 6:

void adaugaMasinaInListaDubla(struct Nod** head, struct Masina masina) { // deep-copy
    if ((*head) == NULL) {
        struct Nod* nodAdaugat = (struct Nod*)malloc(sizeof(struct Nod));
        nodAdaugat->masina.anFabricatie = masina.anFabricatie;
        nodAdaugat->masina.cod = masina.cod;
        nodAdaugat->masina.pret = masina.pret;
        nodAdaugat->masina.denumire = (char*)malloc(sizeof(char) * strlen(masina.denumire) + 1);
        strcpy(nodAdaugat->masina.denumire, masina.denumire);
        nodAdaugat->prev = NULL;
        nodAdaugat->next = NULL;
        (*head) = nodAdaugat;
        return;
    }

    struct Nod* temp = *head;

    while (temp->next != NULL) {
        temp = temp->next;
    }

    struct Nod* nodAdaugat = (struct Nod*)malloc(sizeof(struct Nod));
    nodAdaugat->next = NULL;
    nodAdaugat->prev = temp;
    nodAdaugat->masina.anFabricatie = masina.anFabricatie;
    nodAdaugat->masina.cod = masina.cod;
    nodAdaugat->masina.pret = masina.pret;
    nodAdaugat->masina.denumire = (char*)malloc(sizeof(char) * strlen(masina.denumire) + 1);
    strcpy(nodAdaugat->masina.denumire, masina.denumire);

    temp->next = nodAdaugat;

}

void afiseazaListaDublaMasina(struct Nod* head) {
    while (head != NULL) {
        afiseazaMasina(head->masina);
        head = head->next;
    }
}


void stergeMasinaDinListaDubla(struct Nod** head, int pozitie) {

    if (pozitie < 0) {
        return;
    }

    if (*head == NULL) {
        printf("Lista este goala.\n");
        return;
    }

    struct Nod* temp = *head;
    int index = 0;

    while (temp != NULL && index < (pozitie-1)) {
        temp = temp->next;
        index++;
    }

    // pozitie > dimensiune
    if (temp == NULL) {
        printf("Pozitia specificata depaseste lungimea listei.\n");
        return;
    }

    // capul nodului
    if (temp == *head) {
        *head = temp->next;
        if (*head != NULL) {
            (*head)->prev = NULL;
        }
        free(temp->masina.denumire);
        free(temp);
        printf("Masina de pe pozitia %d a fost stearsa.\n", pozitie);
        return;
    }

    //stergerea nodului
    temp->prev->next = temp->next;
    if (temp->next != NULL) { // daca nu este ultimul nod
        temp->next->prev = temp->prev;
    }

    //stergem nodul
    free(temp->masina.denumire);
    free(temp);
    printf("Masina de pe pozitia %d a fost stearsa.\n", pozitie);
}

void interschimbareListaDubla(struct Nod** head, int poz1, int poz2) { //deep copy

    if (poz1 < 0 || poz2 < 0) {
        return;
    }

    struct Nod* temp = (*head);

    int dimensiuneLista = 0;
    while (temp) {
        dimensiuneLista++;
        temp = temp->next;
    }

    if (poz1 > dimensiuneLista || poz2 > dimensiuneLista) {
        return;
    }


    struct Nod* temp1 = (*head);
    int index1 = 0;
    struct Nod* temp2 = (*head);
    int index2 = 0;
    struct Masina tempM;

    while (index1 < (poz1 - 1)) {
        temp1 = temp1->next;
        index1++;
    }
    while (index2 < (poz2 - 1)) {
        temp2 = temp2->next;
        index2++;
    }

    tempM.anFabricatie = temp1->masina.anFabricatie;
    tempM.cod = temp1->masina.cod;
    tempM.pret = temp1->masina.pret;
    tempM.denumire = (char*)malloc(sizeof(char) * strlen(temp1->masina.denumire) + 1);
    strcpy(tempM.denumire, temp1->masina.denumire);

    temp1->masina.anFabricatie = temp2->masina.anFabricatie;
    temp1->masina.cod = temp2->masina.cod;
    temp1->masina.pret = temp2->masina.pret;
    temp1->masina.denumire = (char*)malloc(sizeof(char) * strlen(temp2->masina.denumire));
    strcpy(temp1->masina.denumire, temp2->masina.denumire);

    temp2->masina.anFabricatie = tempM.anFabricatie;
    temp2->masina.cod = tempM.cod;
    temp2->masina.pret = tempM.pret;
    temp2->masina.denumire = (char*)malloc(sizeof(char) * strlen(tempM.denumire));
    strcpy(temp2->masina.denumire, tempM.denumire);
    
}

void freeNod(struct Nod** nod) {
    struct Nod* current = (*nod);
    struct Nod* next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current->masina.denumire);
        free(current->prev);
        free(current);
        current = next;
    }

    nod = NULL;
}


void main() {

    char numeFisier[50];
    printf("Introduceti numele fisierului:");
    scanf("%s", &numeFisier);

    struct Masina* masini = NULL;
    int dimensiune = 0;
    masini = citireFisier(numeFisier, &dimensiune);

   // afiseazaMasini(masini, dimensiune);

    struct Nod* head = NULL;

    for (int i = 0; i < dimensiune; i++) {
        adaugaMasinaInListaDubla(&head, masini[i]);
    }

    printf("\n\n");
    afiseazaListaDublaMasina(head);


    printf("\n\n");
    stergeMasinaDinListaDubla(&head, -10);
    afiseazaListaDublaMasina(head);

    printf("\n\n");
    interschimbareListaDubla(&head, 2, 5);
    afiseazaListaDublaMasina(head);

   // freeNod(&head);

}