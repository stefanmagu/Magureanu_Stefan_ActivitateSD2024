#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pacient Pacient;
struct Pacient {
	char* nume;
	int varsta;
	int grad;
};

typedef struct Heap Heap;

struct Heap {
	Pacient* vector;
	int dimensiune;
	int dimensiuneReala;
};

Pacient initPacient(char* nume, int grad, int varsta) {
	Pacient p;
	p.nume = (char*)malloc(sizeof(char) * strlen(nume));
	strcpy(p.nume, nume);
	p.grad = grad;
	p.varsta = varsta;

	return p;
}

Heap initHeap(int dimensiune) {
	Heap h;
	h.dimensiuneReala = dimensiune;
	h.dimensiune = dimensiune;
	h.vector = (Pacient*)malloc(sizeof(Pacient) * dimensiune);

	return h;
}

void afiseazaPacient(Pacient p) {
	printf("Nume:%s,Varsta:%d Grad:%d\n", p.nume, p.varsta, p.grad);
}
void afiseazaHeap(Heap h) {
	for (int i = 0; i < h.dimensiune; i++) {
		afiseazaPacient(h.vector[i]);
	}
}

void sorteazaHeap(Heap h, int poz) { // sortam cu heapify
	int copilStanga = 2 * poz + 1;
	int copilDreapta = 2 * poz + 2;
	int pozMax = poz;

	if (copilStanga < h.dimensiune && h.vector[copilStanga].grad > h.vector[pozMax].grad) {
		pozMax = copilStanga;
	}
	if (copilDreapta < h.dimensiune && h.vector[copilDreapta].grad>h.vector[pozMax].grad) {
		pozMax = copilDreapta;
	}

	if (pozMax != poz) { // avem vreunul din copii > decat parintele facem switch
		Pacient aux;
		aux = h.vector[poz];
		h.vector[poz] = h.vector[pozMax];
		h.vector[pozMax] = aux;

		//if ((pozMax * 2) + 1 < h.dimensiune) { // doar daca vrem sa sortam si nodurile descendente dupa nodulMax (nu este necesar, este MaxHeap si fara).
		//	sorteazaHeap(h, pozMax); // recursiv
		//}

	}
}

Pacient extragePacient(Heap* h) {
	Pacient aux;
	if (h->dimensiune > 0) {
		aux = h->vector[0];
		h->vector[0] = h->vector[h->dimensiune - 1];
		h->vector[h->dimensiune - 1] = aux;
		h->dimensiune--;
	}

	for (int i = h->dimensiune / 2 - 1; i >= 0; i--) {
		sorteazaHeap(*h, i);
	}

}



void main() {

	Heap h = initHeap(6);
	
	h.vector[0] = initPacient("Mihai", 3, 23);
	h.vector[1] = initPacient("Stefan", 8, 21);
	h.vector[2] = initPacient("Alexandru", 6, 42);
	h.vector[3] = initPacient("Matei", 2, 39);
	h.vector[4] = initPacient("Tudor", 9, 62);
	h.vector[5] = initPacient("Andreea", 4, 79);
	
	afiseazaHeap(h);
	printf("\n\n\n");

	for (int i = (h.dimensiune / 2) - 1; i >= 0; i--) {
		sorteazaHeap(h, i);
	}
	afiseazaHeap(h);
	printf("\n\n\n");
	extragePacient(&h);
	
	afiseazaHeap(h);
	printf("\n\n\n");
	extragePacient(&h);
	afiseazaHeap(h);
	printf("\n\n\n");

}