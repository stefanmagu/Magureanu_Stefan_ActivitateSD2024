#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Seminar 8 SDD
// 2p - testul

typedef struct Mesaj Mesaj;
typedef struct Heap Heap;

struct Mesaj {
	char* text;
	int prioritate;

};

struct Heap {
	Mesaj* vector;
	int dimensiune;// la filtrare de fapt ascundem elementele;
	int dimensiuneTotala;
};


Mesaj init(const char* text, int prioritate) {
	Mesaj m;
	m.text = malloc(sizeof(char) * (1 + strlen(text)));
	strcpy(m.text, text);
	m.prioritate = prioritate;
	return m;
}

Heap initHeap(int dimensiune) {
	Heap h;
	h.vector = malloc(sizeof(Mesaj) * dimensiune);
	h.dimensiune = 0;
	h.dimensiuneTotala = dimensiune;
	return h;
}

void afisareMesaj(Mesaj m) {
	printf("Mesajul cu textul %s si prioritatea %d\n", m.text, m.prioritate);
}

void afisareHeap(Heap h) {
	for (int i = 0; i < h.dimensiune; i++) {
		afisareMesaj(h.vector[i]);
	}
}

void filtrareHeap(Heap h, int poz) {
	int poz_stanga = poz * 2 + 1;
	int poz_dreapta = poz * 2 + 2;
	int poz_max = poz;
	if (poz_stanga < h.dimensiune && h.vector[poz_stanga].prioritate > h.vector[poz_max].prioritate) {
		poz_max = poz_stanga;
	}
	if (poz_dreapta < h.dimensiune && h.vector[poz_dreapta].prioritate > h.vector[poz_max].prioritate) {
		poz_max = poz_dreapta;
	}
	if (poz_max != poz) { // trebuie facuta interschimbarea

		Mesaj aux = h.vector[poz];
		h.vector[poz] = h.vector[poz_max];
		h.vector[poz_max] = aux;

		if ((poz_max * 2) + 1 < h.dimensiune) {
			filtrareHeap(h, poz_max);
		}
	}

}

Mesaj extragereHeap(Heap* h) { // facem cu ascundere( modificam h.dimensiune
	Mesaj aux;

	if (h->dimensiune > 0) {
		aux = h->vector[0];
		h->vector[0] = h->vector[h->dimensiune - 1];
		h->vector[h->dimensiune - 1] = aux;
		h->dimensiune--;

		for (int i = h->dimensiune / 2 - 1; i >= 0; i--) {
			filtrareHeap(*h, i);
		}

	}

	return aux;

}

void afisareHeapTotala(Heap h) {
	for (int i = 0; i < h.dimensiuneTotala; i++) {
		afisareMesaj(h.vector[i]);
	}
}

void freeHeap(Heap* h) {

	if (h->vector != NULL) {
		for (int i = 0; i < h->dimensiuneTotala; i++) {
			free(h->vector[i].text);
		}
		free(h->vector);
	}
}

void main() {
	Heap h;
	h = initHeap(6);

	h.vector[0] = init("Salut", 3);
	h.vector[1] = init("Salut ce faci", 8);
	h.vector[2] = init("Buna", 6);
	h.vector[3] = init("Noroc", 2);
	h.vector[4] = init("Ce mai faci", 9);
	h.vector[5] = init("Pa", 4);
	h.dimensiune = 6;

	afisareHeap(h);
	printf("Dupa filtrare:\n\n");

	for (int i = (h.dimensiune / 2) - 1; i >= 0; i--) {
		filtrareHeap(h, i);
	}
	afisareHeap(h);

	printf("Extragere heap\n\n");

	Mesaj mesajExtras = extragereHeap(&h);

	afisareMesaj(mesajExtras);
	printf("\n\n");
	afisareHeap(h);

	freeHeap(&h);

}