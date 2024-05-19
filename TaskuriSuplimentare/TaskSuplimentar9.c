#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structura avion luata din video 00 Arbori binari de cautare - FULL GreenCode https://www.youtube.com/watch?v=rfmDpLvTzZ8

typedef struct Avion Avion;
struct Avion {
	char* model;
	int nrLocuri;
	int nrLocuriOcupate;
	float* preturiBilete;
};

typedef struct Nod Nod;
struct Nod {
	Avion info;
	Nod* st;
	Nod* dr;
};

Avion initAvion(char* model, int nrLocuri, int nrLocuriOcupate, float* preturiBilete) {
	Avion avion;

	avion.model = (char*)malloc(sizeof(char) * (strlen(model) + 1));
	strcpy(avion.model, model);
	avion.nrLocuri = nrLocuri;
	avion.nrLocuriOcupate = nrLocuriOcupate;
	avion.preturiBilete = (float*)malloc(sizeof(float)*nrLocuriOcupate);

	for (int i = 0; i < nrLocuriOcupate; i++) {
		avion.preturiBilete[i] = preturiBilete[i];
	}
	return avion;
}

void afisareAvion(Avion avion) {
	printf("Avionul %s are %d locuri, dar au fost ocupate doar %d: ", avion.model,avion.nrLocuri,avion.nrLocuriOcupate);

	for (int i = 0; i < avion.nrLocuriOcupate; i++) {
		printf("%.2f,", avion.preturiBilete[i]);
	}
	printf("\n");
}

Avion citesteAvionDinFisier(FILE* fp) {
	Avion avion;
	char buffer[20];
	fscanf(fp, "%s", buffer);
	avion.model = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(avion.model, buffer);

	fscanf(fp, "%d", &avion.nrLocuri);
	fscanf(fp, "%d", &avion.nrLocuriOcupate);
	avion.preturiBilete = (float*)malloc(sizeof(float) * avion.nrLocuriOcupate);
	for (int i = 0; i < avion.nrLocuriOcupate; i++) {
		fscanf(fp, "%f", &avion.preturiBilete[i]);
	}

	return avion;
}

Nod* inserareAvionInArbore(Nod* radacina, Avion a) {
	if (radacina) { // daca exista
		if (radacina->info.nrLocuri > a.nrLocuri) {
			radacina->st = inserareAvionInArbore(radacina->st, a);
		}
		else {
			radacina->dr = inserareAvionInArbore(radacina->dr, a);
		}
		return radacina;
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = a; // shallow copy
		nou->dr = NULL;
		nou->st = NULL;

		return nou;
	}
}

void afiseazaArbore(Nod* radacina) { // afisare inOrder SRD
	if (radacina) {
		afiseazaArbore(radacina->st);
		afisareAvion(radacina->info);
		afiseazaArbore(radacina->dr);
	}
}


Avion cautareAvionInArboreDupaNrLocuri(Nod* radacina, int nrLocuri) {
	Avion a;

	if (radacina) {
		
		if (radacina->info.nrLocuri == nrLocuri) {
			return radacina->info;
		}
		if (radacina->info.nrLocuri > nrLocuri) {
			a = cautareAvionInArboreDupaNrLocuri(radacina->st, nrLocuri);
		}
		else {
			a = cautareAvionInArboreDupaNrLocuri(radacina->dr, nrLocuri);
		}

		return a;

		
	} else return initAvion("nu exista", -1, -1, 0);

}

void eliberareNod(Nod** nod) {
	free((*nod)->info.model);
	free((*nod)->info.preturiBilete);
	free((*nod));
	(*nod) = NULL;
}


Nod* stergeNodDupaNrLocuri(Nod* radacina, int nrlocuri) {
	Avion a = cautareAvionInArboreDupaNrLocuri(radacina, nrlocuri);

	if (a.nrLocuri == -1) {
		return NULL;
	}
	else if (radacina->info.nrLocuri > nrlocuri) {
		radacina = stergeNodDupaNrLocuri(radacina->dr, nrlocuri);
	}
	else if (radacina->info.nrLocuri < nrlocuri) {
		radacina = stergeNodDupaNrLocuri(radacina->st, nrlocuri);
	}
	else { // l-am gasit, suntem pe el.

		//caz frunza
		if (radacina->st == NULL && radacina->dr == NULL) {
			eliberareNod(&radacina);
		}
		else if(radacina->st && radacina->dr == NULL){ // caz 1 nod stanga
			Nod* aux = radacina;
			radacina = radacina->st;
			eliberareNod(&aux);
		}
		else if (radacina->st == NULL && radacina->dr) {
			Nod* aux = radacina;
			radacina = radacina->dr;
			eliberareNod(&aux);
		}
		else {
			// de implementat
		}
	}
	
}

void main() {

	Nod* rad = NULL;
	FILE* fp = fopen("avioane.txt", "r");
	if (fp) {
		
		int nrAvioane = 0;
		fscanf(fp, "%d", &nrAvioane);
		for (int i = 0; i < nrAvioane; i++) {
			rad = inserareAvionInArbore(rad, citesteAvionDinFisier(fp));
		}

		fclose(fp);
	}

	afiseazaArbore(rad);

	printf("\n\n");
	Avion a = cautareAvionInArboreDupaNrLocuri(rad, 30);
	afisareAvion(a);

}

/*
4
A330
30
10
10 10 10 20 20 20 30 30 30 40
B717
10
7
10 10 10 20 20 20 30
B747
50
8
10 10 10 20 20 20 30 30
A350
40
6
10 10 10 20 20 20
*/