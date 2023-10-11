#include "equipArray.h"

struct equipArray_s {
    int vettEq[EQUIP_SLOT];
    int inUso;
};

equipArray_t equipArray_init() {
    equipArray_t p = (equipArray_t) malloc(sizeof(* p));
    p->inUso = 0;
    return p;
}

void equipArray_free(equipArray_t equipArray) {
    free(equipArray);
}

int equipArray_inUse(equipArray_t equipArray) {
    return equipArray->inUso;
}

void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray) {
    int i;
    printf("\tEquipaggiamento in uso\n");
    for (i = 0; i < equipArray->inUso; i++) {
        printf("\t %d. ", i + 1);
        invArray_printByIndex(fp, invArray, equipArray->vettEq[i]);
    }
    if (i == 0)
        printf("\t[VUOTO]");
    printf("\n");

}

void equipArray_update(equipArray_t equipArray, invArray_t invArray) {
    char name[LEN]; int c, i;
    printf("Aggiungere [1] o rimuovere [0] equipaggiamento? >");
    scanf("%d", &c);
    printf("\nInserire nome dell'oggetto >");
    scanf("%s", name);

    if (!c) {
        i = invArray_searchByName(invArray, name);
        for (c = 0; c < equipArray->inUso; c++) {
            if (i == equipArray->vettEq[c]) {
                equipArray->inUso--; break;
            }
        }
        for (; c < equipArray->inUso; c++) {
            equipArray->vettEq[c] = equipArray->vettEq[c+1];
        }
    } else {
        if (equipArray->inUso != EQUIP_SLOT) {
            if ((i = invArray_searchByName(invArray, name)) != -1) {
                equipArray->vettEq[equipArray->inUso] = i;
                equipArray->inUso++;
            }
        }
    }
}

int equipArray_getEquipByIndex(equipArray_t equipArray, int index) {
    return equipArray->vettEq[index];
}
