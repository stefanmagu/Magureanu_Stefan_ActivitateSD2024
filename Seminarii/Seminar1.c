#include <stdio.h>
#include <malloc.h>


struct Masina {
    char* marca;
    float capacitateCilindrica;
    int an;
};


void afisareMasina(const struct Masina* m) {

    if (m->marca != NULL) {
        printf("Marca: %s\n", m->marca);
    }
    printf("Capaitate cilindrica:%5.2f\n", m->capacitateCilindrica);
    printf("An:%d\n\n", m->an);

}

void dezalocareMasina(struct Masina* m) {
    free(m->marca);
    m->marca = NULL;
}

struct Masina initializareMasina(char* marca, float capacitateCilindrica, int an) {

    struct Masina m;
    m.an = an;
    m.capacitateCilindrica = capacitateCilindrica;
    if (marca != NULL) {
        m.marca = malloc(strlen(marca) + 1);
        strcpy(m.marca, marca);
    }
    else {
        m.marca = malloc(strlen("N/A") + 1);
        strcpy(m.marca, "N/A");
    }

    return m;

}


void main() {

    struct Masina masina1;
    //Dacia
    masina1.marca = (char*)malloc(sizeof(char) * strlen("Dacia") + 1);
    strcpy(masina1.marca, "Dacia");

    masina1.capacitateCilindrica = 1.9;
    masina1.an = 2023;

    afisareMasina(&masina1);

    struct Masina masina2 = initializareMasina("Ford", 2022, 2.49);
    afisareMasina(&masina2);

    dezalocareMasina(&masina1);
    dezalocareMasina(&masina2);

}