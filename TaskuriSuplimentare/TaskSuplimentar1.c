#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct Masina { // Magureanu Stefan -> M si S
    int cod;
    char* denumire;
    int anFabricatie;
    float pret;
};

struct Masina* citesteMasina(struct Masina* m) {

    printf("Introduceti codul masinii:");
    scanf("%d", &m->cod);

    m->denumire = (char*)malloc(100 * sizeof(char));
    printf("Introduceti denumirea masinii:");
    scanf("%s", m->denumire);

    printf("Introduceti anul de fabricatie al masinii:");
    scanf("%d", &m->anFabricatie);

    printf("Introduceti pretul masinii:");
    scanf("%f", &m->pret);

    return m;
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

int main() {

    struct Masina m;
    citesteMasina(&m);

    printf("\nInformatii masina:\n");
    afiseazaMasina(m);


    modificaAnFabricatie(&m, 2022);

    printf("\nInformatii despre masina:\n");
    afiseazaMasina(m);
    printf("Discountul aplicat masinii:%.2f", discountVanzare(&m));

    printf("\nInformatii despre masina dupa aplicarea discountului:\n");
    afiseazaMasina(m);


    free(m.denumire);

    return 0;
}