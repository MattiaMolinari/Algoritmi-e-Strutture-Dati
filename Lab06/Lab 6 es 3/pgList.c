#include "pgList.h"

typedef struct pgNodo_s *link;
typedef struct pgNodo_s {
    pg_t pg;
    link prec;
    link next;
} pgNodo_t;
struct pgList_s {
    link head;
    link tail;
};

pgList_t pgList_init() {
    pgList_t p = (pgList_t) malloc(sizeof(*p));
    p->head = NULL;
    p->tail = NULL;
    return p;
}
void pgList_free(pgList_t pgList) {
    free(pgList);
}

void pgList_read(FILE *fp, pgList_t pgList){
    pg_t pg;
    while (pg_read(fp, &pg) != EOF)
        pgList_insert(pgList, pg);
}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray) {
    link x = pgList->head;
    while (x != NULL) {
        pg_print(fp, &x->pg, invArray);
        x = x->next;
    }
}

void pgList_insert(pgList_t pgList, pg_t pg) {
    link x = (link) malloc(sizeof(pgNodo_t));
    x->pg = pg;
    if (pgList->head == NULL) {
        pgList->head = x;
        pgList->tail = x;
        x->prec = NULL;
    }
    else {
        x->prec = pgList->tail;
        pgList->tail->next = x;
        pgList->tail = x;
    }
    x->next = NULL;
}

void pgList_remove(pgList_t pgList, char* cod) {
    link x = pgList->head;
    while (x != NULL && strcmp(cod, pg_getCod(&x->pg)) != 0)
        x = x->next;
    if (x != NULL) {
        if (pgList->head == x)
            pgList->head = x->next;
        else
            x->prec->next = x->next;
        if (pgList->tail == x)
            pgList->tail = x->prec;
        else
            x->next->prec = x->prec;
        pg_clean(&x->pg);
        free(x);
    }
}

pg_t *pgList_searchByCode(pgList_t pgList, char* cod) {
    link x = pgList->head;
    while (x != NULL && strcmp(cod, pg_getCod(&x->pg)) != 0)
        x = x->next;
    if (x == NULL)
        return NULL;
    return &x->pg;
}