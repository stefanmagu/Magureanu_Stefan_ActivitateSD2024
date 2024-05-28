//seminar 12
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Metrou Metrou;

struct Metrou {
	int serie;
	int nrStatii;
	char* magistrala;
};

typedef struct NodPrincipal NodPrincipal;

typedef struct NodSecundar NodSecundar;

struct NodPrincipal {
	Metrou info;
	NodPrincipal* next;
	NodSecundar* vecini;
};

struct NodSecundar {
	NodPrincipal* nod;
	NodSecundar* next;
};

// functie initMetrou

Metrou creareMetrou(int serie, int nrStatii, char* magistrala) {
	Metrou m;
	m.serie = serie;
	m.nrStatii = nrStatii;
	m.magistrala = (char*)malloc(sizeof(char) * (strlen(magistrala) + 1));
	strcpy(m.magistrala, magistrala);
	return m;
}

// inserare la inceput in lista principala

void inserarePrincipala(NodPrincipal** cap, Metrou m) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = m;
	nou->next = *cap;
	nou->vecini = NULL;
	*cap = nou;
}

// functie de cautare Nod dupa serie

NodPrincipal* cautaNodDupaSerie(NodPrincipal* graf, int serie) {
	while (graf && graf->info.serie != serie) {
		graf = graf->next;
	}
	return graf;
}

// inserare la final in lista secundara
NodSecundar* inserareSecundara(NodSecundar* cap, NodPrincipal* nod) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->next = NULL;
	nou->nod = nod;
	if (cap) {
		NodSecundar* p = cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
		return cap;
	}
	else {
		return nou;
	}
}
// functie de inserare muchie

void inserareMuchie(NodPrincipal* graf, int serieStart, int serieStop) {
	NodPrincipal* nodStart = cautaNodDupaSerie(graf, serieStart);
	NodPrincipal* nodStop = cautaNodDupaSerie(graf, serieStop);

	if (nodStart && nodStop) {
		nodStart->vecini = inserareSecundara(nodStart->vecini, nodStop);
		nodStop->vecini = inserareSecundara(nodStop->vecini, nodStart);
	}

}
// functia de dezalocare


void stergeVecini(NodSecundar** vecini) {
	while (*vecini) {
		NodSecundar* temp = *vecini;
		*vecini = temp->next;
		free(temp);
	}
}

void dezalocareGraf(NodPrincipal** graf) {
	while (*graf) {
		NodPrincipal* aux = *graf;
		free(aux->info.magistrala);
		stergeVecini(&(aux->vecini));
		*graf = aux->next;
		free(aux);
	}
}

/// <summary>
/// 
/// Seminar 13
/// 
/// </summary>



#pragma region Coada
//definin regiune coada
typedef struct NodCoada NodCoada;

struct NodCoada {
	int serie;
	NodCoada* next;
};

//facem inserarea la capat(dreapta), extragerea la stanga
void inserareCoada(NodCoada** cap, int serie) {
	NodCoada* nodNou = (NodCoada*)malloc(sizeof(NodCoada));
	nodNou->serie = serie;
	nodNou->next = NULL;

	if (*cap) {
		NodCoada* temp = *cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nodNou;
	}
	else {
		*cap = nodNou;
	}
}

int extragereCoada(NodCoada** cap) {
	//este extras cap

	if (*cap) {
		int rezultat = (*cap)->serie;
		NodCoada* aux = (*cap);
		*cap = (*cap)->next;
 
		free(aux);

		return rezultat;
	}
	else {
		return -1;
	}

 }
#pragma endregion

int getNumarNoduri(NodPrincipal* graf) {
	int nr = 0;
	while (graf) {
		nr++;
		graf = graf->next;
	}

	return nr;
}

//afisare metrou:
void afisareMetrou(Metrou m) {
	printf("%d. numar statii:%d magistrala:%s\n", m.serie, m.nrStatii, m.magistrala);
}


// afisare parcurgere latime:
void afisareParcurgereLatime(NodPrincipal* graf, int serie) {

	if (graf) {

		NodCoada* coada = NULL;
		int nrNoduri = getNumarNoduri(graf);
		char* vizitate = (char*)malloc(nrNoduri);

		for (int i = 0; i < nrNoduri; i++) {
			vizitate[i] = 0;
		}

		inserareCoada(&coada, serie);
		vizitate[serie] = 1;

		while (coada) {
			int serieNou = extragereCoada(&coada);
			NodPrincipal* nou = cautaNodDupaSerie(graf, serieNou);
			afisareMetrou(nou->info);

			NodSecundar* temp = nou->vecini;
			while (temp) {
				if (vizitate[temp->nod->info.serie] == 0) {

					vizitate[temp->nod->info.serie] = 1;
					inserareCoada(&coada, temp->nod->info.serie);

				}
				temp = temp->next;
			}
		}
		if (vizitate) {
			free(vizitate);
		}
	}
}




void main() {
	NodPrincipal* graf = NULL;

	inserarePrincipala(&graf, creareMetrou(4, 6, "M2"));
	inserarePrincipala(&graf, creareMetrou(3, 7, "M1"));
	inserarePrincipala(&graf, creareMetrou(2, 8, "M4"));
	inserarePrincipala(&graf, creareMetrou(1, 12, "M5"));
	inserarePrincipala(&graf, creareMetrou(0, 4, "M6"));

	inserareMuchie(graf, 0, 1);
	inserareMuchie(graf, 1, 2);
	inserareMuchie(graf, 1, 3);
	inserareMuchie(graf, 1, 4);
	inserareMuchie(graf, 2, 3);
	
	afisareParcurgereLatime(graf, 0);
	dezalocareGraf(&graf);
	afisareParcurgereLatime(graf, 0);
}