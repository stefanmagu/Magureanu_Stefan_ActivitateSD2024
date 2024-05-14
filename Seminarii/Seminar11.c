//seminar 11 SDD
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct Elev Elev;

struct Elev {
	int id;
	int nrMedii;
	float* medii;
};

typedef struct Nod Nod;

struct Nod {
	Elev info;
	Nod* st;
	Nod* dr;
};



Nod* rotireDreapta(Nod* radacina) {
	Nod* nod = radacina->st;
	radacina->st = nod->dr;
	nod->dr = radacina;

	return nod;
}

Nod* rotireStanga(Nod* radacina) {
	Nod* nod;
	nod = radacina->dr;
	radacina->dr = nod->st;
	nod->st = radacina;

	return nod;
}

int calculeazaInaltime(Nod* radacina) {
	int inaltime = 0;

	if (radacina == NULL) {
		return 0;
	}
	else {
		int inaltimeStanga = calculeazaInaltime(radacina->st);
		int inaltimeDreapta = calculeazaInaltime(radacina->dr);

		return 1 + (inaltimeStanga > inaltimeDreapta? inaltimeStanga : inaltimeDreapta);
	}
}

int calculeazaGradEchilibru(Nod* radacina) {

	int inaltimeStanga = calculeazaInaltime(radacina->st);
	int inaltimeDreapta = calculeazaInaltime(radacina->dr);

	return inaltimeStanga - inaltimeDreapta;
}


void inserareArbore(Nod** radacina, Elev e) {

	// de revizizuit acasa...

	if (*radacina != NULL) {

		if ((*radacina)->info.id > e.id) {
			inserareArbore(&((*radacina)->st), e);
		}
		else {
			inserareArbore(&((*radacina)->dr), e);
		}

		int grad = calculeazaGradEchilibru((*radacina));

		if (grad == 2) {
			if (calculeazaGradEchilibru((*radacina)->st) == 1) {
				(*radacina) = rotireDreapta((*radacina));
			}
			else {
				(*radacina)->st = rotireStanga((*radacina)->st);
				(*radacina) = rotireDreapta((*radacina));
			}
		}
		else if (grad == -2) {
			if (calculeazaGradEchilibru((*radacina)->dr) == 1) {
				(*radacina)->dr = rotireDreapta((*radacina)->dr);
			}
			(*radacina) = rotireStanga((*radacina));
		}

	}
	else {
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = e;
		nod->st = NULL;
		nod->dr = NULL;

		*radacina = nod;
	}
}


Elev citireElev(FILE* fp) {
	Elev e;
	fscanf(fp, "%d", &e.id);
	fscanf(fp, "%d", &e.nrMedii);
	e.medii = (float*)malloc(sizeof(float) * e.nrMedii);

	for (int i = 0; i < e.nrMedii; i++) {
		fscanf(fp, "%f", &e.medii[i]);
	}

	return e;
}

Nod* citireFisier(char* numeFisier) {
	Nod* arbore = NULL;
	if (!numeFisier) {
		return NULL;
	}
	else {
		FILE* fp = fopen(numeFisier, "r");

		int nrElevi;
		fscanf(fp, "%d", &nrElevi);

		for (int i = 0; i < nrElevi; i++) {
			Elev e = citireElev(fp);
			inserareArbore(&arbore, e);
		}

		fclose(fp);
	}
	return arbore;
}

void afisareElev(Elev e) {

	printf("Elevul cu id-ul %d are mediile:", e.id);

	for (int i = 0; i < e.nrMedii; i++) {
		printf("%5.2f ", e.medii[i]);
	}
	printf("\n");
}


// in ordine - SRD
// pre ordine - RSD		R = rad, S = stanga, D = dreapta
// post ordine - SDR


void afisareInOrder(Nod* arbore) { // sortare binara
	if (arbore != NULL) {
		afisareInOrder(arbore->st);
		afisareElev(arbore->info);
		afisareInOrder(arbore->dr);
	}
}


float calculeazaMedieElev(Elev e) {
	float suma = 0;
	for (int i = 0; i < e.nrMedii; i++)
	{
		suma += e.medii[i];
	}
	return suma / e.nrMedii;
}


float calculeazaSumaNote(Nod* arbore, int* nrNote) { // media generala a tuturor elevilor
	if (arbore != NULL) {
		float suma = 0;
		//r
		for (int i = 0; i < arbore->info.nrMedii; i++)
		{
			suma += arbore->info.medii[i];
		}
		(*nrNote) += arbore->info.nrMedii;
		//s
		float sumaStanga = calculeazaSumaNote(arbore->st, nrNote);
		//d
		float sumaDreapta = calculeazaSumaNote(arbore->dr, nrNote);
		return suma + sumaStanga + sumaDreapta;
	}
	else {
		return 0;
	}
}

float calculeazaMedieGenerala(Nod* arbore) {
	int nrNote = 0;
	float suma = calculeazaSumaNote(arbore, &nrNote);
	if (nrNote > 0) {
		return suma / nrNote;
	}
	else {
		return 0;
	}
	return suma / nrNote;
}



void afisareInPreorder(Nod* radacina) {
	if (radacina != NULL) {
		afisareElev(radacina->info);
		afisareInPreorder(radacina->st);
		afisareInPreorder(radacina->dr);
	}
}

void stergeArbore(Nod** radacina) {
	if (*radacina) {
		stergeArbore(&((*radacina)->st));
		stergeArbore(&((*radacina)->dr));
	}

	free((*radacina)->info.medii);
	free(radacina);
	*radacina = NULL;

}

void main() {

	Nod* arbore = citireFisier("Elevi.txt");
	afisareInOrder(arbore);
	printf("\n\n\n");
	
	afisareInPreorder(arbore);


	printf("Media este %5.2f", calculeazaMedieGenerala(arbore));

	stergeArbore(&arbore); // aici e eroare ???




}

/*Elevi.txt
6
6
3
5 10 8
3
2
9 7
5
1
8
9
3
8 9 6.8
7
4
10 8 9 5
11
3
7 9 4.5*/