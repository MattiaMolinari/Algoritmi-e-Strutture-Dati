#ifndef LAB_8_ES_2_ST_H
#define LAB_8_ES_2_ST_H

#include <stdio.h>

#define MAXC 30

typedef struct item {
    char nome[MAXC], rete[MAXC];
} Item;
typedef char *Key;

typedef struct symboltable *ST;

ST STinit(int maxN);
void STfree(ST st);
int STcount(ST st);
void STinsert(ST st, Item val);
int STsearch(ST st, char *k);
Item STsearchByIndex(ST st, int i);

#endif //LAB_8_ES_2_ST_H
