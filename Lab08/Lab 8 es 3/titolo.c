#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "titolo.h"

struct ttl_s {
    char codice[MAXC+1];
    quotBST_t quotazioni;
};

typedef struct LISTnode *LISTlink;
struct LISTnode {
    ttl_t titolo;
    LISTlink next;
};
struct ttlList_s {
    LISTlink head;
};

LISTlink LISTNODEnew() {
    LISTlink x = malloc (sizeof *x);
    x->titolo = malloc(sizeof(struct ttl_s));
    x->titolo->quotazioni = BSTinit();
    return x;
}
LISTlink LISTNODEinsert(LISTlink head, char *codice, FILE *fin, int transizioni) {
    LISTlink x = head, t, p = NULL;

    while (x != NULL) {
        if (strcmp(x->titolo->codice, codice) >= 0)
            break;
        p = x;
        x = x->next;
    }

    if (x == NULL) {
        t = LISTNODEnew();
        strcpy(t->titolo->codice, codice);
        t->next = NULL;
        if (p != NULL) p->next = t;
        else { head = t; }
        BSTupdate(t->titolo->quotazioni, fin, transizioni);
        return head;
    }

    if (strcmp(x->titolo->codice, codice) != 0) {
        t = LISTNODEnew();
        strcpy(t->titolo->codice, codice);
        t->next = x;
        if (p != NULL) p->next = t;
        else { head = t; }

    }
    else { t = x; }

    BSTupdate(t->titolo->quotazioni, fin, transizioni);
    return head;

}

ttlList_t LISTinit() {
    ttlList_t L = malloc(sizeof *L);
    L->head = NULL;
    return L;
}
void LISTfree(ttlList_t L) {
    LISTlink x = L->head, t;
    while (x != NULL) {
        t = x;
        BSTfree(x->titolo->quotazioni);
        free(x->titolo);
        x = x->next;
        free(t);
    }
    free(L);
}

void LISTload(FILE *fin, ttlList_t L) {
    int ttlN;
    char codice[MAXC+1];
    if (fin == NULL) return;
    fscanf(fin, "%d\n", &ttlN);
    for (int t = 0, n; t < ttlN; t++) {
        fscanf(fin, "%s %d\n", codice, &n);
        L->head = LISTNODEinsert(L->head, codice, fin, n);
    }
}

void LISTstore(FILE *fout, ttlList_t L) {
    LISTlink x = L->head;
    while (x != NULL) {
        fprintf(fout, "%s\n", x->titolo->codice);
        BSTstore(fout, x->titolo->quotazioni);
        x = x->next;
    }
}

ttl_t LISTsearch(ttlList_t L, char *codice) {
    LISTlink x = L->head; int search;
    while (x != NULL && (search = strcmp(x->titolo->codice, codice)) <= 0) {
        if (!search) return x->titolo;
        x = x->next;
    }

    return NULL;
}

void TTLsearchQ(FILE *fout, ttl_t titolo, int data) {
    int q = BSTsearchQ(titolo->quotazioni, data);
    if (q >= 0)
        fprintf(fout, "Quotazione giornaliera nella data selezionata: %d\n", q);
    else
        fprintf(fout, "Data selezionata non presente\n");
}

void TTLsearchMaxMin(FILE *fout, ttl_t titolo, int data1, int data2) {
    int max = -1, min = INT_MAX;
    BSTsearchMaxMin(titolo->quotazioni, data1, data2, &max, &min);
    if (max != -1) fprintf(fout, "Quotazione massima: %d \t Quotazione minima: %d\n", max, min);
    else { fprintf(fout, "Nessuna quotazione tra le date selezionate\n"); }
}

void TTLbalance(ttl_t titolo) {
    BSTbalance(titolo->quotazioni);
}

int TTLrapport(ttl_t titolo) {
    return BSTrapport(titolo->quotazioni);
}

