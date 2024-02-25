#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct Masina { // Magureanu Stefan -> M si S
    int cod;
    char* denumire;
    int anFabricatie;
    float pret;
};

void citesteMasina(struct Masina* m) {

    printf("Introduceti codul masinii:");
    scanf("%d", &m->cod);

    m->denumire = (char*)malloc(100 * sizeof(char));
    printf("Introduceti denumirea masinii:");
    scanf("%s", m->denumire);

    printf("Introduceti anul de fabricatie al masinii:");
    scanf("%d", &m->anFabricatie);

    printf("Introduceti pretul masinii:");
    scanf("%f", &m->pret);

  
}

float discountVanzare(struct Masina* m) {

    float discount = 0.0;

    if (m->anFabricatie >= 2000) {
        m->pret = m->pret - (m->pret * 0.1);
        discount = m->pret * 0.1;
    }
    else {
        m->pret = m->pret - (m->pret * 0.2);
        discount = m->pret * 0.2;
    }

    return discount;

}

void modificaAnFabricatie(struct Masina* m, int noulAnFabricatie) {
    m->anFabricatie = noulAnFabricatie;
}

void afiseazaMasina(struct Masina m) {
    printf("Cod: %d\n", m.cod);
    printf("Denumire: %s\n", m.denumire);
    printf("An de fabricatie: %d\n", m.anFabricatie);
    printf("Pret: %.2f\n", m.pret);
}


// TaskSuplimentar 2:

struct Masina* copiazaMasiniPeste2000(struct Masina* masini, int numarMasini) {

    int numarMasiniPeste2000 = 0;


    for (int i = 0; i < numarMasini; i++) {
        if (masini[i].anFabricatie >= 2000) {
            numarMasiniPeste2000++;
        }
    }

    struct Masina* masiniPeste2000 = (struct Masina*)malloc(numarMasiniPeste2000 * sizeof(struct Masina));

    int index = 0;
    for (int i = 0; i < numarMasini; i++) {
        if (masini[i].anFabricatie >= 2000) {
            masiniPeste2000[index] = masini[i];
            index++;
        }
    }

    return masiniPeste2000;
}

struct Masina* mutaMasiniSub2000(struct Masina* masini, int numarMasini) {
    int nrMasiniMutate = 0;
    for (int i = 0; i < numarMasini; i++) {
        if (masini[i].anFabricatie < 2000) {
            nrMasiniMutate++;
        }
    }

    struct Masina* m = (struct Masina*)malloc(nrMasiniMutate * sizeof(struct Masina));

    int j = 0;
    for (int i = 0; i < nrMasiniMutate; i++) {
        if (masini[i].anFabricatie < 2000) {
            m[j] = masini[i];
            j++;
        }
    }
    
    struct Masina* temp = (struct Masina*)malloc((numarMasini - nrMasiniMutate) * sizeof(struct Masina));

    for (int i = 0; i < numarMasini; i++) {

        if (masini[i].anFabricatie >= 2000) {
            temp[i] = masini[i];
        }
    }
    
    masini = &temp;

    return m;
}

void freeMemory(struct Masina *m) {
	free(m->denumire);
    free(m);
}

struct Masina* concateneazaMasini(struct Masina* masini1, int dimensiuneMasini1, struct Masina* masini2, int dimensiuneMasini2) {

    struct Masina* temp = (struct Masina*)malloc((dimensiuneMasini1 + dimensiuneMasini2) * sizeof(struct Masina));

    int index = 0;

    for (int i = 0; i < dimensiuneMasini1; i++) {
        temp[index] = masini1[i];
        index++;
    }

    for (int i = 0; i < dimensiuneMasini2; i++) {
        temp[index] = masini2[i];
        index++;
    }

    return temp;

}

void afiseazaMasini(struct Masina* masini, int dimensiune) {
	for (int i = 0; i < dimensiune; i++) {
		afiseazaMasina(masini[i]);
	}
}

void main() {



    struct Masina* masini = (struct Masina*)malloc(5 * sizeof(struct Masina));

    for (int i = 0; i < 5; i++) {
        printf("Introdceti informatii despre masina %d:\n", i + 1);
        citesteMasina(&masini[i]);
    }

    for (int i = 0; i < 5; i++) {
        printf("\nInformatii masina %d:\n", i + 1);
        afiseazaMasina(masini[i]);
        printf("Discountul aplicat masinii: %.2f\n", discountVanzare(&masini[i]));
    }

    struct Masina* masiniDupa2000 = copiazaMasiniPeste2000(masini, 5);

    free(masini);
    free(masiniDupa2000);

    struct Masina* masiniInainte2000 = mutaMasiniSub2000(masini, 5);

    struct Masina* m2 = (struct Masina*)malloc(5 * sizeof(struct Masina));
    struct Masina* m3 = (struct Masina*)malloc(5 * sizeof(struct Masina));


    for (int i = 0; i < 5; i++) {
        printf("Introdceti informatii despre masina %d:\n", i + 1);
        citesteMasina(&m2[i]);
    }
    for (int i = 0; i < 5; i++) {
        printf("Introdceti informatii despre masina %d:\n", i + 1);
        citesteMasina(&m3[i]);
    }
    


    struct Masina* masiniConcatenate = concateneazaMasini(m2, 5, m3, 5);

   afiseazaMasini(masiniConcatenate, 10);

   free(masiniInainte2000);
   free(m2);
   free(m3);
   free(masiniConcatenate);
   
}