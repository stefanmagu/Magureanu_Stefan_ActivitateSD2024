// fisier aeroporturi.txt :

/*

1, Otopeni, 20
2, Baneasa, 10
3, Sibiu, 30

*/

///////////

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>


struct Aeroport {
	int id;
	char* nume;
	int nrTerminale;

};

struct Aeroport* adaugaAeroport(struct Aeroport* vector, struct Aeroport aeroport, int* dimensiune) { // shallow copy *vector, deep copy pt ultimul element pe care-l adaugam(struct Aeroport aeroport, mai putin *char nume care e alocat dinamic si e transmis prin referinta);

	struct Aeroport* copie = (struct Aeroport*)malloc(sizeof(struct Aeroport) * ((*dimensiune) + 1));
	copie[(*dimensiune)] = aeroport;

	for (int i = 0; i < (*dimensiune); i++) {
		copie[i] = vector[i];
	}

	if (vector) {
		free(vector);
	}
	(*dimensiune)++;
	return copie;
}

struct Aeroport* citireFisier(const char* numeFisier, int* dimensiune) {
	struct Aeroport* vectorAeroporturi;
	vectorAeroporturi = NULL;

	FILE* fp = fopen(numeFisier, "r");
	if (fp == NULL) {
		printf("Fisierul nu a fost deschis\n");
	}
	else {
		(*dimensiune) = 0;
		char buffer[100];
		while (fgets(buffer, 99, fp) != NULL) { // inseamna ca avem linie de citit (din fisier)
			struct Aeroport aeroport;
			char delimitator[] = ",\n"; // ?
			char* token = strtok(buffer, delimitator); // ? returneaza un char*
			aeroport.id = atoi(token);

			token = strtok(NULL, delimitator);
			aeroport.nume = malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(aeroport.nume, token);

			token = strtok(NULL, delimitator);
			aeroport.nrTerminale = atoi(token);

			vectorAeroporturi = adaugaAeroport(vectorAeroporturi, aeroport, dimensiune);

		}
		fclose(fp);
	}
	return vectorAeroporturi;
}

void afisareAeroport(struct Aeroport* aeroport) {
	printf("Aeroportul cu id:%d, are numele %s cu %d terminale\n", aeroport->id, aeroport->nume, aeroport->nrTerminale);

}

void afisareVector(struct Aeroport* aeroporturi, int dim) {
	printf("AFISARE VECTOR:\n");

	for (int i = 0; i < dim; i++) {
		afisareAeroport(&aeroporturi[i]);
	}

}

void dezalocareVector(struct Aeroport** aeroporturi, int* dim) {
	for (int i = 0; i < (*dim); i++) {
		free((*aeroporturi)[i].nume);
	}
	free(*aeroporturi);
	(*aeroporturi) = NULL;
	(*dim) = 0;
}

struct Aeroport idCautat(struct Aeroport* aeroporturi, int dim, int id) {
	for (int i = 0; i < dim; i++) {
		if (aeroporturi[i].nume == id) {
			return aeroporturi[i];
		}
	}
	struct Aeroport aeroport;
	aeroport.id = -1;
	aeroport.nrTerminale = -1;
	aeroport.nume = malloc(sizeof(char) * (1 + strlen("N/A")));
	return aeroport;
}


void main() {
	int dimensiune = 0;
	struct Aeroport* aeroporturi = NULL;

	aeroporturi = citireFisier("aeroporturi.txt", &dimensiune);
	afisareVector(aeroporturi, dimensiune);
	dezalocareVector(&aeroporturi, &dimensiune);
}


