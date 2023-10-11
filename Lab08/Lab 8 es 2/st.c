#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "st.h"

struct symboltable {Item *a; int maxN; int size;};

ST STinit(int maxN) {
    ST st; int i;
    st = malloc(sizeof(*st));
    st->a = malloc(maxN * sizeof(Item));
    for (i = 0; i < maxN; i++)
        st->a[i].nome[0] = '\0';
    st->maxN = maxN;
    st->size = 0;
    return st;
}
int STcount(ST st) {
    return st->size;
}
void STfree(ST st) {
    free(st->a); free(st);
}

void STinsert(ST st, Item val) {
    int i = st->size++;
    if (st->size > st->maxN) {
        st->a=realloc(st->a,(2*st->maxN)*sizeof(Item));
        if (st->a == NULL) return;
        st->maxN = 2*st->maxN;
    }
    while((i>0)&&strcmp(val.nome,st->a[i-1].nome)<0){
        st->a[i] = st->a[i-1]; i--;
    }
    st->a[i] = val;
}

int searchR(ST st, int l, int r, Key k) {
    int m;
    m = (l + r)/2;
    if (l > r) return -1;
    if (strcmp(k,st->a[m].nome)==0) return m;
    if (l == r) return -1;
    if (strcmp(k, st->a[m].nome)<0)
        return searchR(st, l, m-1, k);
    else
        return searchR (st, m+1, r, k);
}
int STsearch(ST st, Key k) {
    return searchR(st, 0, st->size-1, k);
}

Item STsearchByIndex(ST st, int i) {
    return st->a[i];
}
