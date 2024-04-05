#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct Cladire Cladire;

struct Cladire {
	int id;
	int an;
	char* numeDezvoltator;
};

typedef struct Nod Nod;

struct Nod {
	Cladire info;
	Nod* next;
};

typedef struct HashTable HashTable;

struct HashTable {
	Nod** vector;
	int dim;
};

Cladire initCladire(int id, int anConstruire, char* numeDezvoltator) {
	Cladire cladire;
	cladire.id = id;
	cladire.an = anConstruire;
	cladire.numeDezvoltator = (char*)malloc(sizeof(char) * strlen(numeDezvoltator) + 1);
	strcpy(cladire.numeDezvoltator, numeDezvoltator);

	return cladire;
}


HashTable initHashTable(int dim) {
	HashTable hashTable;
	hashTable.dim = dim;
	hashTable.vector = (Nod**)malloc(sizeof(Nod*) * dim);

	for (int i = 0; i < dim; i++) {
		hashTable.vector[i] = NULL;
	}

	return hashTable;
}

int calculeazaHash(int an, int dim) {
	int pozitie = (an * 13) % dim;
	return pozitie;
}

void insereazaCladire(HashTable table, Cladire cladire) { // inseram la final
	int pozitia = calculeazaHash(cladire.an, table.dim);

	Nod* adaugat = (Nod*)malloc(sizeof(Nod));
	adaugat->info = cladire; // shallow
	adaugat->next = NULL;

	if (table.vector[pozitia] != NULL) { // avem coliziune
		Nod* temp = table.vector[pozitia];

		while (temp->next) {
			temp = temp->next;
		}
		temp->next = adaugat;
	} else {
		table.vector[pozitia] = adaugat;
	}

}

void afiseazaCladire(Cladire cladire) {
	printf("Cladirea cu id %d, construita in anul %d de dezvoltatorul %s\n", cladire.id, cladire.an, cladire.numeDezvoltator);
}


void afiseazaLista(Nod* nod) {
	while (nod) {
		afiseazaCladire(nod->info);
		nod = nod->next;
	}
}

void afiseazaHashTable(HashTable hashTable) {
	for (int i = 0; i < hashTable.dim; i++) {
		printf("Clusterul de %d:\n", i);
		afiseazaLista(hashTable.vector[i]);
	}
}

void afiseazaCluster(HashTable hashTable, int an) {
	int pozitia = calculeazaHash(an, hashTable.dim);
	afiseazaLista(hashTable.vector[pozitia]);
}

void stergeCladireCluster(Nod** nod, int id) {
	Nod* temp = *nod;
	
	if (temp != NULL) {
		if (temp->info.id == id) { // primul element?
			free(temp->info.numeDezvoltator);
			temp = temp->next;
			(*nod) = temp; // !!!
		}
		else { // la mijloc sau la final
			Nod* prev = NULL;
			while (temp->next && temp->info.id != id) {
				prev = temp;
				temp = temp->next;
			}
			if (temp->info.id == id) { // l-am gasit
				free(temp->info.numeDezvoltator);
				prev->next = temp->next;
				
			}
		}
		
	}
}

void stergeCladire(HashTable hashTable, int id, int an) {
	int pozitia = calculeazaHash(an, hashTable.dim);
	stergeCladireCluster(&hashTable.vector[pozitia], id);
}

void stergeCladireDupaId(HashTable hashTable, int id) {
	// Nestiind pe ce clsuter trebuie sa ne pozitionam(hashcode-ul lui "an"), vom cauta in fiecare cluster dupa element;
	for (int i = 0; i < hashTable.dim; i++) {
		if (hashTable.vector[i] != NULL) {
			stergeCladireCluster(&hashTable.vector[i], id);
		}
	}
}

Cladire* hashTableToVector(HashTable hashTable, int an, int *dim) {
	int pozitie = calculeazaHash(an, hashTable.dim);

	*dim = 0;

	Nod* temp = hashTable.vector[pozitie];
	

	while (temp) {
		if (temp->info.an == an) {
			(*dim)++;
		}
		temp = temp->next;
	}
	Cladire* cladiri = (Cladire*)malloc(sizeof(Cladire) * (*dim));

	temp = hashTable.vector[pozitie]; // ne mutam in head
	int index = 0;

	while (temp) {
		if (temp->info.an == an) { //deep copy

			cladiri[index].numeDezvoltator = (char*)malloc(sizeof(char) * strlen(temp->info.numeDezvoltator) + 1);
			strcpy(cladiri[index].numeDezvoltator, temp->info.numeDezvoltator);
			cladiri[index].id = temp->info.id;
			cladiri[index].an = temp->info.an;

			index++;
		}
		temp = temp->next;
	}

	return cladiri;

}
void modificaAnCladire(HashTable hashTable, int id, int an, int noul_an) {

	int pozitie = calculeazaHash(an, hashTable.dim);
	//parcug clusterul pana gasesc id-ul, si copiez
	Nod* temp = hashTable.vector[pozitie];
	Cladire cladire;

	while (temp) {
		if (temp->info.id == id) {
			cladire.numeDezvoltator = (char*)malloc(sizeof(char) * strlen(temp->info.numeDezvoltator) + 1);
			strcpy(cladire.numeDezvoltator, temp->info.numeDezvoltator);
		}
		temp = temp->next;
	}

	cladire.id = id;
	cladire.an = noul_an;

	stergeCladire(hashTable, id, an);
	insereazaCladire(hashTable, cladire);
}



void main() {
	Cladire cladiri[7];

	 cladiri[0] = initCladire(1, 2000, "Gigel");
	 cladiri[1] = initCladire(2, 2001, "Mario");
	 cladiri[2] = initCladire(3, 2002, "Mihai");
	 cladiri[3] = initCladire(4, 2000, "Stefan");
	 cladiri[4] = initCladire(5, 2012, "Andrada");
	 cladiri[5] = initCladire(6, 2023, "Nely");
	 cladiri[6] = initCladire(7, 2023, "Radu");

	for (int i = 0; i < 7; i++) {
		printf("%d, %d %s %d\n", cladiri[i].an, cladiri[i].id, cladiri[i].numeDezvoltator, calculeazaHash(cladiri[i].an, 7));

	}

	HashTable hashTable = initHashTable(6);

	for (int i = 0; i < 7; i++) {
		insereazaCladire(hashTable, cladiri[i]);
	}
	printf("Afisare HashTable:\n");
	afiseazaHashTable(hashTable);
	printf("\n\n\n");


	printf("Afisare cluster mapat dupa has-ul anului %d:\n", 2000);
	afiseazaCluster(hashTable,2000);
	printf("\n\n\n");

	printf("Stergere cladire id 3, an 2000:\n");
	stergeCladire(hashTable, 3, 2000);
	afiseazaHashTable(hashTable);
	printf("\n\n\n");

	printf("Stergem cladire id 3, an necunoscut:\n");
	stergeCladireDupaId(hashTable, 3);
	afiseazaHashTable(hashTable);
	printf("\n\n\n");

	int* dim = 0;
	Cladire* vector = hashTableToVector(hashTable, 2000, &dim);

	for (int i = 0; i < dim; i++) {
		afiseazaCladire(vector[i]);
	}
	printf("\n\n\n");

	
	printf("Modificare an 2023->2000 pentru id 7:");
	modificaAnCladire(hashTable, 7, 2023, 2000);
	afiseazaHashTable(hashTable);
	printf("\n\n\n");
}