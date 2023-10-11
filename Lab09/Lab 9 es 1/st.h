#ifndef ST_H
#define ST_H

typedef struct st_s* st_t;

/* la chiave è una stringa */

/* inizializzazione e deallocazione tabella */
st_t STinit(int maxN);
void STfree(st_t ST);
/* indice in tabella a partire da chiave */
int STgetIndex(st_t ST, char *name);
/* chiave a partire da indice in tabella */
char *STgetName(st_t ST, int index);
/* inserimento in tabella */
int STinsert(st_t ST, char *name);


#endif