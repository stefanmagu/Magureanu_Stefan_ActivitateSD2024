// seminar 6 SDD lista dublu inlantuita
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>


typedef struct Biblioteca Biblioteca;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;


struct Biblioteca {
    char* nume;
    int nrCarti;
    int nrCititori;
};

struct Nod {
    Nod* prev;
    Biblioteca info;
    Nod* next;
};

struct ListaDubla { // lista care memoreaza capul si coada 
    Nod* head;
    Nod* tail;
};

Biblioteca initializare(char* nume, int nrCarti, int nrCititori) {

    Biblioteca b;
    b.nume = (char*)malloc(sizeof(char) * strlen(nume) + 1);
    strcpy(b.nume, nume);
    b.nrCarti = nrCarti;
    b.nrCititori = nrCititori;

    return b;
}

void inserareInceput(ListaDubla* listaDubla, Biblioteca b) {

    Nod* nod = (Nod*)malloc(sizeof(Nod));
    nod->info = b; //shallow copy
    nod->next = listaDubla->head;
    nod->prev = NULL;

    if (listaDubla->head) {
        listaDubla->head->prev = nod;
    }
    else {
        listaDubla->head = nod;
        listaDubla->tail = nod;
    }
    listaDubla->head = nod;

}


void afisareListaInceput(ListaDubla listaDubla) {
    while (listaDubla.head) {
        printf("Biblioteca: %s; Numar cititori: %d; Numar carti: %d\n",
            listaDubla.head->info.nume, listaDubla.head->info.nrCititori, listaDubla.head->info.nrCarti);

        listaDubla.head = listaDubla.head->next;
    }
    printf("\n");
}

void stergereNodNume(ListaDubla* listaDubla, const char* nume) {

    Nod* aux = listaDubla->head;

    while (aux && strcmp(nume, aux->info.nume) != 0) {

        aux = aux->next;
    }

    if (aux == listaDubla->head) { // cazul in care elementul este inceputul listei

        if (aux == listaDubla->tail) {
            listaDubla->head = NULL;
            listaDubla->tail = NULL;
        }
        else {
            listaDubla->head = listaDubla->head->next;
            listaDubla->head->prev = NULL;
        }

    }
    else if (aux == listaDubla->tail) { // cazul in care elementul este ultimul element al listei
        aux->prev->next = NULL;
        listaDubla->tail = aux->prev;
    }
    else { // e la mijloc, nu modificam nici head nici tail
        aux->next->prev = aux->prev;
        aux->prev->next = aux->next;

        free(aux->info.nume);
        free(aux);
    }

}

void stergereLista(ListaDubla* listaDubla) {
    while (listaDubla->head) {

        free(listaDubla->head->info.nume);
        Nod* aux = listaDubla->head;
        listaDubla->head = listaDubla->head->next;
        free(aux);
    }
    listaDubla->head = NULL;
    listaDubla->tail = NULL;
}

int nrCartiTotal(ListaDubla listaDubla) { // numaram cartile din toata lista, cu parcurgere de la final la inceput
    int rezultat = 0;
    while (listaDubla.tail) {
        rezultat += listaDubla.tail->info.nrCarti;
        listaDubla.tail = listaDubla.tail->prev;
    }

    return rezultat;

}

void main() {
    Biblioteca b1 = initializare("Mihai Eminescu", 150, 20);
    Biblioteca b2 = initializare("Ioan Slavici", 200, 30);
    Biblioteca b3 = initializare("Tudor Arghezi", 150, 15);

    ListaDubla listaDubla;
    listaDubla.head = NULL;
    listaDubla.tail = NULL;


    inserareInceput(&listaDubla, b1);
    inserareInceput(&listaDubla, b2);
    inserareInceput(&listaDubla, b3);

    afisareListaInceput(listaDubla);
    printf("\n");


    //  stergereNodNume(&listaDubla, "Ioan Slavici");
    //  afisareListaInceput(listaDubla);

    //  stergereNodNume(&listaDubla, "Mihai Eminescu");
    //  afisareListaInceput(listaDubla);

    //  stergereNodNume(&listaDubla, "Tudor Arghezi");
    //  afisareListaInceput(listaDubla);

    //  stergereLista(&listaDubla);
    //  afisareListaInceput(listaDubla);

    int result = nrCartiTotal(listaDubla);
    printf("Nr carti : %d", result);

}