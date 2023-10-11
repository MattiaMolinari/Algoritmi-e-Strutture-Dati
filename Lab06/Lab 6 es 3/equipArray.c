#include "equipArray.h"

struct equipArray_s {
    inv_t **vettEq;
    int inUso;
    int nMax;
};

equipArray_t equipArray_init() {
    equipArray_t p = (equipArray_t) malloc(sizeof(* p));
    p->inUso = 0;
    p->nMax = EQUIP_SLOT;
    return p;
}

void equipArray_free(equipArray_t equipArray) {
    free(equipArray->vettEq);
    free(equipArray);
}

int equipArray_inUse(equipArray_t equipArray) {
    return equipArray->inUso;
}

void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray) {
    int i;
    printf("\tEquipaggiamento in uso\n");
    for (i = 0; i < equipArray->inUso; i++) {
        printf("\t %d. ", i+1);
        invArray_printByIndex(fp, invArray, invArray_searchByName(invArray, inv_getName(equipArray->vettEq[i])));
    }
    if (i == 0)
        printf("\t[VUOTO]");
    printf("\n");

}

void equipArray_update(equipArray_t equipArray, invArray_t invArray) {
    char name[LEN]; int c, i, f;
    printf("Aggiungere [1] o rimuovere [0] equipaggiamento? >");
    scanf("%d", &c);
    printf("\nInserire nome dell'oggetto >");
    scanf("%s", name);

    if (c == 0) {
        for (i = 0, f = 0; !f && i < equipArray->inUso; i++)
            if (strcmp(name, inv_getName(equipArray->vettEq[i])) == 0) {
                equipArray->inUso--;
                f = 1;
            }
        for (; i < equipArray->inUso; i++)
            equipArray->vettEq[i] = equipArray->vettEq[i + 1];
        equipArray->vettEq = (inv_t **) realloc(equipArray->vettEq, (equipArray->inUso) * sizeof(inv_t *));
    } else {
        if (equipArray->inUso != equipArray->nMax) {
            if ((i = invArray_searchByName(invArray, name))!=-1) {
                equipArray->vettEq = (inv_t **) realloc(equipArray->vettEq, (equipArray->inUso + 1) * sizeof(inv_t *));
                equipArray->vettEq[equipArray->inUso] = invArray_getByIndex(invArray,i);
                equipArray->inUso++;
            }
        }
    }
}

inv_t *equipArray_getEquipByIndex(equipArray_t equipArray, int index) {
    return equipArray->vettEq[index];
}
