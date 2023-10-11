#include "invArray.h"

struct invArray_s {
    inv_t *vettInv;
    int nInv;
};

invArray_t invArray_init() {
    invArray_t p = (invArray_t) malloc(sizeof(* p));
    return p;
}

void invArray_free(invArray_t invArray) {
    free(invArray->vettInv);
    free(invArray);
}

void invArray_read(FILE *fp, invArray_t invArray) {
    fscanf(fp, "%d", &invArray->nInv);
    invArray->vettInv = (inv_t *) malloc(invArray->nInv*sizeof(inv_t));
    for (int i = 0; i < invArray->nInv; i ++)
        inv_read(fp, &invArray->vettInv[i]);
}

void invArray_print(FILE *fp, invArray_t invArray) {
    for (int i = 0; i < invArray->nInv; i++)
        inv_print(fp, &invArray->vettInv[i]);
}

void invArray_printByIndex(FILE *fp, invArray_t invArray, int index) {
    inv_print(fp, &invArray->vettInv[index]);
}

inv_t *invArray_getByIndex(invArray_t invArray, int index) {
    return &invArray->vettInv[index];
}

int invArray_searchByName(invArray_t invArray, char *name) {
    for (int i = 0; i < invArray->nInv; i++)
        if (strcmp(name, invArray->vettInv[i].nome) == 0)
            return i;
    return -1;
}